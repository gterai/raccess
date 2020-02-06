/*
 * author: hisanori kiryu
 */
#ifndef RACCESS__DP_MATRIX_HPP
#define RACCESS__DP_MATRIX_HPP
#include "util/util.hpp"
#include "raccess/score.hpp"
namespace Raccess { class DpMatrix;}
class Raccess::DpMatrix {
public:
  typedef long           IntT;
  typedef vector<ScoreT> V;
  typedef vector<IntT>   VI;
  V p;
  V s;
  V s1;
  V o;
  void set_size(IntT psize, IntT ssize, IntT s1size, IntT osize) {
    p.resize(psize);
    s.resize(ssize);
    s1.resize(s1size);
    o.resize(osize);
  }
  void clean() { 
    vfill(s , NEG_INF());
    vfill(s1, NEG_INF());
    vfill(p , NEG_INF());
    vfill(o , NEG_INF());
  }
};
#endif
