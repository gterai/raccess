/*
 * author: hisanori kiryu
 */
#ifndef STR_CONV_HPP
#define STR_CONV_HPP
#include "splitter.hpp"
template <typename T> inline T stot(string const& s) { return T(s);}
template<> inline string stot<string>(string const& s) { return s;}
template<> inline int stot<int>(string const& s) { return atoi(s.c_str());}
template<> inline long stot<long>(string const& s){ return atol(s.c_str());}
template<> inline signed char stot<signed char>(string const& s) 
{ return (signed char)stot<int>(s);}
template<> inline short stot<short>(string const& s) { return (short)stot<int>(s);}
template<> inline unsigned long stot<unsigned long>(string const& s)
{ return strtoul(s.c_str(), NULL, 10);}
template<> inline unsigned char stot<unsigned char>(string const& s) 
{ return stot<unsigned long>(s);}
template<> inline unsigned short stot<unsigned short>(string const& s) 
{ return stot<unsigned long>(s);}
template<> inline unsigned int stot<unsigned int>(string const& s) 
{ return stot<unsigned long>(s);}
template<> inline double stot<double>(string const& s) { return atof(s.c_str());}
template<> inline float stot<float>(string const& s) { return (float)stot<double>(s);}
template<> inline bool stot<bool>(string const& s) {
  if(s.empty()) { cerr <<  "empty bool string\n"; exit(EXIT_FAILURE);};
  string::size_type n = 0;
  while (n < s.size() && isalnum((unsigned char)s[n])) { ++n;}
#define _stot_eql_(t) (n == strlen(t) && strncmp(s.c_str(), t, n) == 0)
  switch (s[0]) {
  case '0': if (n == 1) return false; break;
  case '1': if (n == 1) return true; break;
  case 't': if ((n == 1) || _stot_eql_("true")) return true;   break;
  case 'f': if ((n == 1) || _stot_eql_("false")) return false; break;
  case 'T': if ((n == 1) || _stot_eql_("True") || _stot_eql_("TRUE")) return true; break;
  case 'F': if ((n == 1) || _stot_eql_("False") || _stot_eql_("FALSE")) return false; break;
  case 'y': if ((n == 1) || _stot_eql_("yes")) return true; break;
  case 'n': if ((n == 1) || _stot_eql_("no")) return false; break;
  case 'Y': if ((n == 1) || _stot_eql_("Yes") || _stot_eql_("YES")) return true; break;
  case 'N': if ((n == 1) || _stot_eql_("No") || _stot_eql_("NO")) return false; break;
  case 'o': 
    if (_stot_eql_("on")) return true; 
    if (_stot_eql_("off")) return false; 
    break;
  case 'O': 
    if (_stot_eql_("On") || _stot_eql_("ON")) return true;
    if (_stot_eql_("Off") || _stot_eql_("OFF")) return false;
    break;
  default: break;
  }
  cerr << "bad bool string: " << s.c_str() << '\n'; exit(EXIT_FAILURE);
  return false;
#undef _stot_eql_
}

// stot does not use template
#define _stot_(T) inline T stot(T const& t, string const& s) { return stot<T>(s);}
_stot_(bool)
_stot_(signed char)
_stot_(unsigned char)
_stot_(short)
_stot_(unsigned short)
_stot_(int)
_stot_(unsigned int)
_stot_(long)
_stot_(unsigned long)
_stot_(float)
_stot_(double)
_stot_(string)
#undef _stot_
// ttos does not use template
#define _ttos_(T) inline ostream& ttos(ostream& os, T const& t) { os << t; return os;}
_ttos_(bool)
  // _ttos_(signed char)
  // _ttos_(unsigned char)
inline ostream& ttos(ostream& os, signed char const& t) { os << (int)t; return os;}
inline ostream& ttos(ostream& os, unsigned char const& t) { os << (int)t; return os;}
_ttos_(short)
_ttos_(unsigned short)
_ttos_(int)
_ttos_(unsigned int)
_ttos_(long)
_ttos_(unsigned long)
inline ostream& ttos(ostream& os, double const& t) { os << setprecision(17) << t; return os;}
inline ostream& ttos(ostream& os, float const& t) { return ttos(os, (double)t);}
inline ostream& ttos(ostream& os, const char* t) { os << t; return os;}
_ttos_(string)
#undef _ttos_

// ttos does not use template
#define _ttos_(T) inline string ttos(T const& t) { ostringstream os; ttos(os, t); return os.str();}
_ttos_(bool)
_ttos_(signed char)
_ttos_(unsigned char)
_ttos_(short)
_ttos_(unsigned short)
_ttos_(int)
_ttos_(unsigned int)
_ttos_(long)
_ttos_(unsigned long)
_ttos_(float)
_ttos_(double)
  // _ttos_(string)
inline string const& ttos(string const& t) { return t;}
#undef _ttos_

// partial specialization is not allowed
#define _stot_(T)                                                                               \
template<> inline vector<T> stot< vector<T> >(string const& s) {                                \
  vector<T> v; Splitter spl(s, ",:;");                                                          \
  for (Splitter::iterator i = spl.begin(); i != spl.end(); ++i) { v.push_back(stot<T>(*i));}    \
  return v;                                                                                     \
}

_stot_(bool)
_stot_(signed char)
_stot_(unsigned char)
_stot_(short)
_stot_(unsigned short)
_stot_(int)
_stot_(unsigned int)
_stot_(long)
_stot_(unsigned long)
_stot_(float)
_stot_(double)
_stot_(string)
#undef _stot_

#define _stot_(T) \
inline vector<T> stot(vector<T> const& t, string const& s) { return stot< vector<T> >(s);}

_stot_(bool)
_stot_(signed char)
_stot_(unsigned char)
_stot_(short)
_stot_(unsigned short)
_stot_(int)
_stot_(unsigned int)
_stot_(long)
_stot_(unsigned long)
_stot_(float)
_stot_(double)
_stot_(string)
#undef _stot_

// partial specialization is not allowed
#define _ttos_(T) \
inline ostream& ttos(ostream& os, vector<T> const& t, string const& delim = ",") { \
  for (vector<T>::const_iterator i = t.begin(); i != t.end(); ++i) { ttos(os, *i); os << delim;} \
  return os; \
}

_ttos_(bool)
_ttos_(signed char)
_ttos_(unsigned char)
_ttos_(short)
_ttos_(unsigned short)
_ttos_(int)
_ttos_(unsigned int)
_ttos_(long)
_ttos_(unsigned long)
_ttos_(float)
_ttos_(double)
#undef _ttos_

#define _ttos_(T) \
inline string ttos(vector<T> const& t, string const& delim = ",") \
{ ostringstream os; ttos(os, t, delim); return os.str();}

_ttos_(bool)
_ttos_(signed char)
_ttos_(unsigned char)
_ttos_(short)
_ttos_(unsigned short)
_ttos_(int)
_ttos_(unsigned int)
_ttos_(long)
_ttos_(unsigned long)
_ttos_(float)
_ttos_(double)
#undef _ttos_

template <typename T> struct stot_fun_t {
  typedef T result_type;
  T operator()(string const& s) const { return stot(T(), s);}
};
template <typename T> struct ttos_fun_t {
  typedef string result_type;
  string operator()(T const& t) const {return ttos(t);}
};
template <typename T> stot_fun_t<T> stot_fun(T const& t) { return stot_fun_t<T>();}
template <typename T> ttos_fun_t<T> ttos_fun(T const& t) { return ttos_fun_t<T>();}

inline void print_row(ostream& fo) 
{ ttos(fo, "\n");}
template <typename T1>
void print_row(ostream& fo, const T1& t1) 
{ ttos(fo, t1); ttos(fo, "\n");}
template <typename T1, typename T2>
void print_row(ostream& fo, const T1& t1, const T2& t2) { 
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3) { 
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
	       const T11& t11) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\t");
  ttos(fo, t11); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
	       const T11& t11, const T12& t12) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\t");
  ttos(fo, t11); ttos(fo, "\t");
  ttos(fo, t12); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
	       const T11& t11, const T12& t12, const T13& t13) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\t");
  ttos(fo, t11); ttos(fo, "\t");
  ttos(fo, t12); ttos(fo, "\t");
  ttos(fo, t13); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13, typename T14>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
	       const T11& t11, const T12& t12, const T13& t13, const T14& t14) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\t");
  ttos(fo, t11); ttos(fo, "\t");
  ttos(fo, t12); ttos(fo, "\t");
  ttos(fo, t13); ttos(fo, "\t");
  ttos(fo, t14); ttos(fo, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13, typename T14, typename T15>
void print_row(ostream& fo, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
	       const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15) {
  ttos(fo, t1); ttos(fo, "\t"); 
  ttos(fo, t2); ttos(fo, "\t"); 
  ttos(fo, t3); ttos(fo, "\t");
  ttos(fo, t4); ttos(fo, "\t");
  ttos(fo, t5); ttos(fo, "\t");
  ttos(fo, t6); ttos(fo, "\t");
  ttos(fo, t7); ttos(fo, "\t");
  ttos(fo, t8); ttos(fo, "\t");
  ttos(fo, t9); ttos(fo, "\t");
  ttos(fo, t10); ttos(fo, "\t");
  ttos(fo, t11); ttos(fo, "\t");
  ttos(fo, t12); ttos(fo, "\t");
  ttos(fo, t13); ttos(fo, "\t");
  ttos(fo, t14); ttos(fo, "\t");
  ttos(fo, t15); ttos(fo, "\n");
}

#endif




