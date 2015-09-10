// originally
// http://www.informit.com/store/beyond-the-c-plus-plus-standard-library-an-introduction-9780321133540

#ifndef ANY_OUT_H
#define ANY_OUT_H

#include <iostream>
#include <boost/any.hpp>
#include <memory>

struct streamer{
  virtual void print(std::ostream &o, const boost::any &a) const =0;
  virtual ~streamer() {}
};

template <class T>
struct streamer_impl: streamer{
  void print(std::ostream &o, const boost::any &a) const { o <<  boost::any_cast<T>(a);
                                                      //   o << *boost::any_cast<T>(&a);
  }
};

class any_out {
  boost::any o_;
  std::shared_ptr<streamer> streamer_;
  void swap(any_out & r){
    std::swap(streamer_, r.streamer_);
    std::swap(o_, r.o_);
  }
public:
  any_out(): streamer_() {}

  template<class T> any_out(const T& value)
    : o_(value),
      streamer_(new streamer_impl<typename std::decay<decltype(value)>::type>)
  {
  }

  any_out(const any_out& a)
    : o_(a.o_), streamer_(a.streamer_) {}

  template <class T>
  any_out & operator=(const T& r) {
    any_out(r).swap(*this);
    
    return *this;
  }
  ~any_out() { }

  boost::any       &get()       { return o_; }
  const boost::any &get() const { return o_; }

  friend std::ostream &operator<<(std::ostream& o, const any_out & a);
};

std::ostream &operator<<(std::ostream& o, const any_out & a) {
  if(a.streamer_)
    a.streamer_->print(o, a.o_);
  return o;
}

#endif
