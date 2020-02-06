/*
 * error.hpp
 */
#ifndef UTIL__ERROR_HPP
#define UTIL__ERROR_HPP
#include "str_conv.hpp"
class Break { public: static int Here() { static volatile int a; return a;}};
#define Die(...) Error("error", false, false, true, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#define Warn(...) Error("warning", false, false, false, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#define Check(expr, ...) Error("error", true, expr, true, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
#define Assert(expr, ...) Error("error", true, expr, true, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Assert(expr, ...) 
#endif
#define Error(type, check, expr, die, func, file, line, ...) do {                                                          \
 if (!check) {                                                                                                             \
  cerr << "\n---\n" << type << ":\n  in " << func << " at " << file << ':' << line << '\n';                                  \
  ErrorMessage(__VA_ARGS__); Break::Here(); if (die) { exit(EXIT_FAILURE);}                                                \
 } else if (!(expr)) {                                                                                                     \
  cerr << "\n---\n" << type << ":\n  in " << func << " at " << file << ':' << line << "\nexpr (" << #expr << ") failed.\n";  \
  ErrorMessage(__VA_ARGS__); Break::Here(); if (die) { exit(EXIT_FAILURE);}                                                \
 }                                                                                                                         \
} while (0)

inline void ErrorMessage() 
{ ttos(cerr, "\n");}
template <typename T1>
void ErrorMessage(const T1& t1) 
{ ttos(cerr, t1); ttos(cerr, "\n");}
template <typename T1, typename T2>
void ErrorMessage(const T1& t1, const T2& t2) { 
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3) { 
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5); 
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	       const T6& t6, const T7& t7, const T8& t8, const T9& t9) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
		  const T11& t11) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10);
  ttos(cerr, t11); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
		  const T11& t11, const T12& t12) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10);
  ttos(cerr, t11); ttos(cerr, t12); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
		  const T11& t11, const T12& t12, const T13& t13) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10);
  ttos(cerr, t11); ttos(cerr, t12); ttos(cerr, t13); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13, typename T14>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
		  const T11& t11, const T12& t12, const T13& t13, const T14& t14) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10);
  ttos(cerr, t11); ttos(cerr, t12); ttos(cerr, t13); ttos(cerr, t14); ttos(cerr, "\n");
}
template <typename T1, typename T2, typename T3, typename T4, typename T5,
	  typename T6, typename T7, typename T8, typename T9, typename T10, 
	  typename T11, typename T12, typename T13, typename T14, typename T15>
void ErrorMessage(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
		  const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10,
		  const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15) {
  ttos(cerr, t1); ttos(cerr, t2); ttos(cerr, t3); ttos(cerr, t4); ttos(cerr, t5);
  ttos(cerr, t6); ttos(cerr, t7); ttos(cerr, t8); ttos(cerr, t9); ttos(cerr, t10);
  ttos(cerr, t11); ttos(cerr, t12); ttos(cerr, t13); ttos(cerr, t14); ttos(cerr, t15); 
  ttos(cerr, "\n");
}
#endif
