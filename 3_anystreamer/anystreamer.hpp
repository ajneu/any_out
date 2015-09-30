// adapted from any_out type described here
// http://www.informit.com/store/beyond-the-c-plus-plus-standard-library-an-introduction-9780321133540

#ifndef ANYSTREAMER_H
#define ANYSTREAMER_H

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <memory>

#include <boost/type_traits/has_left_shift.hpp>

#include <boost/any.hpp>
using boost::any;
using boost::any_cast;



struct streamer {
  virtual void print(std::ostream& os, const any& ay) const =0;
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
  void print(std::ostream &os, const any &ay) const {   os <<  any_cast<T>(ay);
                                                 //     os << *any_cast<T>(&ay);
  }
};


struct static_streamer {
  static streamer_impl<const char * const>  streamer_char_p;
  static streamer_impl<const std::string&>  streamer_string;
  static streamer_impl<const int>           streamer_int;
};



class AnyStreamer : public any {
private:
  std::shared_ptr<streamer> streamer_heap;
  streamer                 *streamer_;
public:
  AnyStreamer()
    : any(), streamer_{nullptr}
  {}

  AnyStreamer(const AnyStreamer& rhs)
    : any{rhs}, streamer_heap{rhs.streamer_heap}, streamer_{rhs.streamer_}
  {}

  AnyStreamer(AnyStreamer&& rhs)
    : any{std::move(rhs)}, streamer_heap{std::move(rhs.streamer_heap)}, streamer_{rhs.streamer_}
  {}
  
  template <typename ValueType>
  explicit AnyStreamer(ValueType&& value)  /* must be explicit, in order for compiler error to occur when operator<< does not exist
                                              (see static_assert below!) */
    : any(std::forward<ValueType>(value)),
      streamer_{streamer::ptr_static_streamer_if_exist(std::type_index(typeid(typename std::decay<ValueType>::type )))}
  {
    static_assert(sizeof(decltype(std::cout << value)) > 0, "cannot print"); // force compiler error, if operator<< does not exist for this type
    if (streamer_ == nullptr) {
      std::cout << "#  no static for " << value << std::endl;
      streamer_ = get_streamer_from_heap<ValueType>();
    }
  }

  AnyStreamer& operator=(const AnyStreamer& rhs) {
    static_cast<any&>(*this).operator=(rhs);
    streamer_heap = rhs.streamer_heap;
    streamer_ = rhs.streamer_;
    return *this;
  }

  AnyStreamer& operator=(AnyStreamer&& rhs) {
    static_cast<any&>(*this).operator=(std::move(rhs));
    streamer_heap = std::move(rhs.streamer_heap);
    streamer_ = rhs.streamer_;
    return *this;
  }

  template<typename ValueType>
  AnyStreamer& operator=(ValueType&& rhs)
  {
    static_assert(sizeof(decltype(std::cout << rhs)) > 0, "cannot print"); // force compiler error, if operator<< does not exist for this type
    
    std::cout << "here2" << std::endl;
    streamer_ = streamer::ptr_static_streamer_if_exist(std::type_index(typeid(typename std::decay<ValueType>::type )));
    if (streamer_ == nullptr) {
      std::cout << "#  no static for " << rhs << std::endl;
      streamer_ = get_streamer_from_heap<ValueType>();
    }
    static_cast<any&>(*this).operator=(std::forward<ValueType>(rhs));
    return *this;
  }
  
private:
  template<typename ValueType>
  streamer *get_streamer_from_heap()
  {
    streamer_heap.reset(static_cast<streamer *>(new streamer_impl<          typename std::decay<ValueType>::type >));
    return streamer_heap.get();
  }
  
  friend std::ostream &operator<<(std::ostream &os, const AnyStreamer &ay);
};

std::ostream &operator<<(std::ostream &os, const AnyStreamer &ay);

#endif
