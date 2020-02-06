/*
 * tensor_index.hpp
 */
#ifndef TENSOR_INDEX_HPP
#define TENSOR_INDEX_HPP
#include "array.hpp"
template <typename T = int, bool CheckRange = false>
class TensorIndex {
public:
  enum { MAX_RANK = 4};
  typedef T                      IntT;
  typedef CArray<IntT, MAX_RANK> Strides;
  typedef CArray<IntT, MAX_RANK> Ranges;
  typedef vector<IntT>           Idxs;
  Strides _strides;
  Ranges  _ranges;
  IntT    _rank;
  IntT    _size;
  TensorIndex() : _rank(0), _size(0) {_strides.fill(0); _ranges.fill(0);}
  bool empty() const { return (size() == 0);}
  IntT max_rank() const { return MAX_RANK;}
  IntT rank() const { return _rank;}
  IntT size() const { return _size;}
  IntT length(IntT ith) const { return _ranges[ith];}
  void clear() { _rank = 0; _size = 0; _strides.fill(0), _ranges.fill(0);}
  template <typename Iter> void set_ranges(Iter b, Iter e) {
    _rank = distance(b, e);
    Check(_rank <= MAX_RANK);
    copy(b, e, _ranges.begin());
    IntT n = 1;
    for (IntT k = (_rank - 1); k >= 0; --k) {
      IntT l = _ranges[k];
      if (l > 0) {
	_strides[k] = n;
	n *= l;
      } else {
	_strides[k] = 0;
      }
    }
    _size = n;	  
  }
  Idxs indexes(IntT n) const {
    Idxs a(rank());
    for (IntT k = (rank() - 1); k >= 0; --k) {
      IntT l = _ranges[k];
      IntT r = (-1);
      if (l > 0) {
	r = n % l;
	n /= l;
      }
      a[k] = r;
    }
    return a;
  }
  template <typename Block> int each(Block block) const {
    Idxs a(_rank);
    a.fill(0);
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
  template <typename Iter> IntT index(Iter b, Iter e) const { 
    check_rank(distance(b, e)); 
    check_range(b); 
    return inner_product(b, (b + _rank), _strides.begin(), 0);
  }
  void check_rank(IntT n) const {if (CheckRange){Check(rank() == n);}}
  template <typename Iter> void check_range(Iter b) const { 
    if (CheckRange) {
      for (IntT k = 0; k < _rank; ++k) { Check(0 <= (*b) && (*b) < _ranges[k]); ++b;}
    }
  } 
  void set_size(IntT n0) { IntT t[] = {n0}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1) 
  { IntT t[] = {n0, n1}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1, IntT n2) 
  { IntT t[] = {n0, n1, n2}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  void set_size(IntT n0, IntT n1, IntT n2, IntT n3) 
  { IntT t[] = {n0, n1, n2, n3}; set_ranges(t, t + sizeof(t)/sizeof(t[0]));}
  IntT ref() const {check_rank(0); return 0;}
  IntT ref(IntT i0) const { check_rank(1); return (i0);}
  IntT ref(IntT i0, IntT i1) const { check_rank(2); return (i0*_strides[0] + i1);}
  IntT ref(IntT i0, IntT i1, IntT i2) const 
  { check_rank(3); return (i0*_strides[0] + i1*_strides[1] + i2);}
  IntT ref(IntT i0, IntT i1, IntT i2, IntT i3) const 
  { check_rank(4); return (i0*_strides[0] + i1*_strides[1] + i2*_strides[2] + i3);}
};

template <typename T> class TensorIndexBase {
public:
  typedef T            IntT;
  typedef vector<IntT> Idxs;
  virtual ~TensorIndexBase() {}
  virtual bool empty() const                  = 0;
  virtual IntT size() const                      = 0;
  virtual IntT rank() const                      = 0;
  virtual IntT max_rank() const                  = 0;
  virtual IntT length(IntT i) const                 = 0;
  virtual IntT ref() const                       = 0;
  virtual IntT ref(IntT i0) const                   = 0;
  virtual IntT ref(IntT i0, IntT i1) const             = 0;
  virtual IntT ref(IntT i0, IntT i1, IntT i2) const       = 0;
  virtual IntT ref(IntT i0, IntT i1, IntT i2, IntT i3) const = 0;
  virtual Idxs indexes(IntT n) const         = 0;
  // member template cannot be virtual.
  // virtual template <typename Block> int each(Block block) const
};

template <typename T = int, 
	  T L0 = (-1), T L1 = (-1), T L2 = (-1), T L3 = (-1), bool CheckRange = false>
class CTensorIndex {
public:
#define L_P_(k) (L##k != (-1))
#define LEN_(k) (L##k > 0 ? L##k : 1)
  enum {
    MAX_RANK = 4,
    RANK     = (L_P_(0) + L_P_(1) + L_P_(2) + L_P_(3)),
    SIZE     = (LEN_(0) * LEN_(1) * LEN_(2) * LEN_(3) * 1),
    RANGE0   = (L0 <= 0 ? 0 : L0),
    RANGE1   = (L1 <= 0 ? 0 : L1),
    RANGE2   = (L2 <= 0 ? 0 : L2),
    RANGE3   = (L3 <= 0 ? 0 : L3),
    STRIDE0  = (L0 <= 0 ? 0 : (LEN_(1) * LEN_(2) * LEN_(3) * 1)),
    STRIDE1  = (L1 <= 0 ? 0 : (LEN_(2) * LEN_(3) * 1)),
    STRIDE2  = (L2 <= 0 ? 0 : (LEN_(3) * 1)),
    STRIDE3  = (L3 <= 0 ? 0 : (1))
  };
#undef L_P_
#undef LEN_
  typedef T            IntT;
  typedef vector<IntT> Idxs;
  bool empty() const { return (size() == 0);}
  IntT max_rank() const { return MAX_RANK;}
  IntT rank() const { return RANK;}
  IntT size() const { return SIZE;}
  IntT length(IntT i) const { return ranges()[i];}
  const CArray<IntT, MAX_RANK>& ranges() const 
  { static const CArray<IntT, MAX_RANK> a = {{RANGE0, RANGE1, RANGE2, RANGE3}}; return a;}
  Idxs lengths() const { return Idxs(ranges().begin(), ranges().begin() + rank());}
  Idxs strides() const {
    static const CArray<IntT, MAX_RANK> a = {{STRIDE0, STRIDE1, STRIDE2, STRIDE3}};
    return Idxs(a.begin(), a.begin() + rank());
  }
  template <typename Iter> IntT index(Iter b, Iter e) const 
  { return inner_product(b, e, strides().begin(), 0);}
  Idxs indexes(IntT n) const {
    Idxs a(rank());
    for (IntT k = (rank() - 1); k >= 0; --k) {
      IntT l = ranges()[k];
      IntT r = (-1);
      if (l > 0) {
	r = n % l;
	n /= l;
      }
      a[k] = r;
    }
    return a;
  }
  template <typename Block> int each(Block block) const {
    Idxs a(rank());
    a.fill(0);
    IntT n = 0;
    IntT i = 0;
    int st = 0;
    bool upward = false;
    while (true) {
      if (upward) {
	++a[i];
	if (ranges()[i] <= a[i]) {
	  if (i == 0) break;
	  a[i] = 0;
	  --i;
	} else {
	  ++i;
	  upward = false;
	}
      } else {
	if (i == rank()) {
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
  void check_rank(IntT rank) const {if (CheckRange){Check(rank == RANK);}}
#define CHECK_RANGE(k) do { if (CheckRange){ Check(0 <= i##k && i##k < RANGE##k);}} while(0)
#if 1
#define PROD(k) (STRIDE##k * i##k)
#define GET_INDEX_0()               (0)
#define GET_INDEX_1(i0)             (PROD(0))
#define GET_INDEX_2(i0, i1)         (PROD(0)+PROD(1))
#define GET_INDEX_3(i0, i1, i2)     (PROD(0)+PROD(1)+PROD(2))
#define GET_INDEX_4(i0, i1, i2, i3) (PROD(0)+PROD(1)+PROD(2)+PROD(3))
#else
#define LEN_(k) (L##k > 0 ? L##k : 1)
#define IDX_(k) (L##k > 0 ? i##k : 0)
#define GET_INDEX_0()               (0)
#define GET_INDEX_1(i0)             (GET_INDEX_0() * 1 + IDX_(0))
#define GET_INDEX_2(i0, i1)         (GET_INDEX_1(IDX_(0)) * LEN_(1) + IDX_(1))
#define GET_INDEX_3(i0, i1, i2)     (GET_INDEX_2(IDX_(0), IDX_(1)) * LEN_(2) + IDX_(2))
#define GET_INDEX_4(i0, i1, i2, i3) (GET_INDEX_3(IDX_(0), IDX_(1), IDX_(2)) * LEN_(3) + IDX_(3))
#undef IDX_
#undef LEN_
#endif
  IntT ref() const { check_rank(0); return GET_INDEX_0();}
  IntT ref(IntT i0) const { check_rank(1); return GET_INDEX_1(i0);}
  IntT ref(IntT i0, IntT i1) const { check_rank(2); return GET_INDEX_2(i0, i1);}
  IntT ref(IntT i0, IntT i1, IntT i2) const { check_rank(3); return GET_INDEX_3(i0, i1, i2);}
  IntT ref(IntT i0, IntT i1, IntT i2, IntT i3) const 
  { check_rank(4); return GET_INDEX_4(i0, i1, i2, i3);}
#undef PROD
#undef GET_INDEX_0
#undef GET_INDEX_1
#undef GET_INDEX_2
#undef GET_INDEX_3
#undef GET_INDEX_4
#undef CHECK_RANGE
};
#endif
