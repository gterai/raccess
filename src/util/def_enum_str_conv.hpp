/*
 * author: hisanori kiryu
 */
// #ifndef UTIL__DEF_ENUM_STR_CONV_HPP
// #define UTIL__DEF_ENUM_STR_CONV_HPP
#define enum_item(t) t,
enum Enum { enum_items()};
#undef enum_item
#define enum_item(t) if (str == #t) { return t;}; 
friend Enum str_to_enum(const Enum& t, const string& str)  // conflict with stot if inside namespace
{ enum_items(); Die("bad str:", str); return (Enum)0;}
#undef enum_item
#define enum_item(t) if (type == t) { return #t;};
friend string enum_to_str(const Enum& type) { enum_items(); Die("bad type:", type); return "";}
#undef enum_item
// #endif
