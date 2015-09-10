// originally
// http://www.informit.com/store/beyond-the-c-plus-plus-standard-library-an-introduction-9780321133540

#ifndef ANY_OUT_H
#define ANY_OUT_H

#include <iostream>
#include <boost/any.hpp>
#include <typeindex>
#include <unordered_map>
#include <memory>

template <typename T>
struct streamer_impl;

struct streamer {
  virtual void print(std::ostream &o, const boost::any &a) const =0;
  virtual ~streamer() {}

  static streamer *ptr_static_streamer_if_exist(std::type_index idx) {
    const auto it = type_mapper.find(idx);
    return ((it != streamer::type_mapper.cend()) ? it->second : nullptr);
  }
private:
  static const std::unordered_map<std::type_index, streamer*> type_mapper;
};

template <typename T>
struct streamer_impl: streamer{

  void print(std::ostream &o, const boost::any &a) const {   o <<  boost::any_cast<T>(a);
                                                      //     o << *boost::any_cast<T>(&a);
  }
};

#define FOREACH_STATIC_ROW(X)             \
  X(const char *const,  streamer_char_p)  \
  X(const std::string&, streamer_string)  \
  X(const int,          streamer_int)

// note the const ref with the string!

#define GEN_DECLARATION(TYPE, VAR) static streamer_impl<TYPE> VAR;

struct static_streamer {
  FOREACH_STATIC_ROW(GEN_DECLARATION)
};


#if 0
// generates the following

struct static_streamer {
  static streamer_impl<const char * const>  streamer_char_p;
  static streamer_impl<const std::string&>   streamer_string;
  static streamer_impl<const int>           streamer_int;
};
#endif




class any_out {
  boost::any o_;
  std::shared_ptr<streamer> streamer_heap;
  streamer                 *streamer_;

  void swap(any_out & r){
    std::swap(streamer_, r.streamer_);
    std::swap(streamer_heap, r.streamer_heap);
    std::swap(o_, r.o_);
  }
public:
  any_out(): streamer_{nullptr} {}

  template<class T> any_out(const T& value)
    : o_(value)
      ,streamer_(streamer::ptr_static_streamer_if_exist(std::type_index(typeid(typename std::decay<const T&>::type ))))
  {
    if (streamer_ == nullptr) {
      streamer_heap.reset(static_cast<streamer *>(new streamer_impl<           typename std::decay<const T&>::type >));
      streamer_ = streamer_heap.get();
      std::cout << "#  no static for " << value << std::endl;
    } else {
      std::cout << "#  using static!!!" << std::endl;
    }
  }
    
  any_out(const any_out& a)
    : o_(a.o_), streamer_heap{a.streamer_heap}, streamer_{a.streamer_} {}

  template <class T>
  any_out & operator=(const T& r) {
    any_out(r).swap(*this);
    return *this;
  }
  ~any_out() { }

  boost::any       &get_any()       { return o_; }
  const boost::any &get_any() const { return o_; }
  
  friend std::ostream &operator<<(std::ostream& o, const any_out & a);
};

#endif
