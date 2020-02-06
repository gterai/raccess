/*
 * author: hisanori kiryu
 */
#ifndef TENSOR_RING_INDEX_HPP
#define TENSOR_RING_INDEX_HPP
#include "util/util.hpp"
#include "util/array.hpp"
template <typename T = int, T MaxRank = 4> class TensorRingIndex {
public:
  typedef T                     IntT;
  typedef CArray<IntT, MaxRank> Arr;
  typedef Arr                   Ranges;
  typedef vector<IntT>          Idxs;
  Ranges _ranges;
  IntT   _rank;
  IntT   _size;
  TensorRingIndex() : _rank(0), _size(0) { _ranges.fill(0);}
  bool empty() const { return (size() == 0);}
  IntT max_rank() const { return _ranges.size();}
  IntT rank() const { return _rank;}
  IntT size() const { return _size;}
  IntT length(IntT ith) const { return _ranges[ith];}
  void clear() { _rank = 0; _size = 0; _ranges.fill(0);}
  template <typename Iter> void set_ranges(Iter b, Iter e) {
    _rank = distance(b, e); Check(_rank <= max_rank());
    std::copy(b, e, _ranges.begin());
    _size = 1;
    for (IntT k = 0; k != _rank; ++k) { const IntT& l = _ranges[k]; Check(l > 0); _size *= l;}
  }
  void check_rank(IntT n) const { Assert(n == _rank);}
  void check_range() {} // check if 0 <= i_k < _ranges[k] for debug
  IntT cidx(IntT i, IntT k) const { return cyclic_index(i, _ranges[k]);}
  template <typename Iter> IntT index(Iter b, Iter e) const {
    check_rank(distance(b, e));
    IntT z = 0;
    for (IntT k = 0; k != _rank; ++k, ++b) { z *= _ranges[k]; z += cidx(*b, k);}
    return z;
  }
  void set_size() { IntT t[] = {}; set_ranges(t, t);}
  void set_size(IntT n0) { IntT t[] = {n0}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1) 
  { IntT t[] = {n0, n1}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1, IntT n2) 
  { IntT t[] = {n0, n1, n2}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1, IntT n2, IntT n3) 
  { IntT t[] = {n0, n1, n2, n3}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  IntT ref() const { check_rank(0); return ref0();}
  IntT ref(IntT i0) const { check_rank(1); return ref1(i0);}
  IntT ref(IntT i0, IntT i1) const { check_rank(2); return ref2(i0, i1);}
  IntT ref(IntT i0, IntT i1, IntT i2) const { check_rank(3); return ref3(i0, i1, i2);}
  IntT ref(IntT i0, IntT i1, IntT i2, IntT i3) const { check_rank(4); return ref4(i0, i1, i2, i3);}
  IntT ref0() const { return 0;}
  IntT ref1(IntT i0) const { return (ref0() * _ranges[0] + cidx(i0, 0));}
  IntT ref2(IntT i0, IntT i1) const { return (ref1(i0) * _ranges[1] + cidx(i1, 1));}
  IntT ref3(IntT i0, IntT i1, IntT i2) const 
  { return (ref2(i0, i1) * _ranges[2] + cidx(i2, 2));}
  IntT ref4(IntT i0, IntT i1, IntT i2, IntT i3) const 
  { return (ref3(i0, i1, i2) * _ranges[3] + cidx(i3, 3));}
  Idxs indexes(IntT n) const {
    Idxs a(_rank);
    for (IntT k = _rank; k != 0;) {
      --k;
      const IntT& l = _ranges[k];
      a[k] = (n % l);
      n /= l;
    }
    return a;
  }
  template <typename Block> int each(Block block) const {
    Idxs a(_rank); Check(_rank > 0);
    IntT n = 0;
    IntT i = 0;
    int st = 0;
    bool upward = false;
    while (true) {
      if (upward) {
	++a[i];
	if (_ranges[i] <= a[i]) {
	  if (i == 0) break;
	  a[i] = 0;
	  --i;
	} else {
	  ++i;
	  upward = false;
	}
      } else {
	if (i == _rank) {
	  st = block(a, n);
	  if (st) return st;
	  ++n;
	  --i;
	  upward = true;
	} else {
	  ++i;
	}
      }
    }
    return 0;
  }
};
#endif
