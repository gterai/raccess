/*
 * author: hisanori kiryu
 */
// #ifndef UTIL__DEF_OPT_MAIN_HPP
// #define UTIL__DEF_OPT_MAIN_HPP
#define opt_item_(_name, _type, _value, _desc) opt_item(_name, _type, _value, _desc)
#define opt_item(_name, _type, _value, _desc) attr_accessor(_type, _name)
  opt_items()
#undef opt_item
#undef opt_item_
  static int main(int argc, char** argv, Self& obj) {
#define opt_item_(_name, _type, _value, _desc)
#define opt_item(_name, _type, _value, _desc) obj.set_##_name(stot(_type(), _value)); \
    { os << ("-"#_name"=<"#_type":"_value">\t:\t"_desc"\n");};
    ostringstream os; os << "options: -name=<type:default>\t:\tdescription\n\n"; opt_items();
#undef opt_item
#undef opt_item_
#define opt_item_(_name, _type, _value, _desc) opt_item(_name, _type, _value, _desc)
#define opt_item(_name, _type, _value, _desc)				\
    if (i.name() == #_name) { obj.set_##_name(stot(_type(), i.value())); continue;};
    for (OptIter i = OptIter(argc, argv); i != OptIter(); ++i)
      { opt_items(); Die("unknown option: -", i.name(), "=", i.value());}
    if (obj.help()) { cout << os.str();} else { obj.run();}; return 0;}
#undef opt_item
#undef opt_item_
// #endif
