#include "any_out.hpp"

#define GEN_DEFINITION(TYPE, VAR) streamer_impl<TYPE>  static_streamer::VAR;

FOREACH_STATIC_ROW(GEN_DEFINITION)

#if 0
// generates the following

streamer_impl<const char *const>   static_streamer::streamer_char_p;
streamer_impl<const std::string&>  static_streamer::streamer_string;
streamer_impl<const int>           static_streamer::streamer_int;
#endif



#define GEN_MAP_ENRY(TYPE, VAR) {std::type_index(typeid(TYPE)), &static_streamer::VAR},

const std::unordered_map<std::type_index, streamer*> streamer::type_mapper{
  {
    FOREACH_STATIC_ROW(GEN_MAP_ENRY)
  }
};

#if 0
// generates the following

const std::unordered_map<std::type_index, streamer*> streamer::type_mapper{
  {{std::type_index(typeid(const char *const)),  &static_streamer::streamer_char_p},
   {std::type_index(typeid(const std::string&)), &static_streamer::streamer_string},
   {std::type_index(typeid(const int)),          &static_streamer::streamer_int}
  }
};
#endif

std::ostream &operator<<(std::ostream& o, const any_out & a) {
  if(a.streamer_)
    a.streamer_->print(o, a.o_);
  return o;
}
