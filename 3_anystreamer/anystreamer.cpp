#include "anystreamer.hpp"

streamer_impl<const char *const>   static_streamer::streamer_char_p;
streamer_impl<const std::string&>  static_streamer::streamer_string;
streamer_impl<const int>           static_streamer::streamer_int;

const std::unordered_map<std::type_index, streamer*> streamer::type_mapper{
  {{std::type_index(typeid(const char *const)),  &static_streamer::streamer_char_p},
   {std::type_index(typeid(const std::string&)), &static_streamer::streamer_string},
   {std::type_index(typeid(const int)),          &static_streamer::streamer_int}
  }
};

std::ostream &operator<<(std::ostream &os, const AnyStreamer &ay) {
  if(ay.streamer_)
    ay.streamer_->print(os, ay);
  return os;
}
