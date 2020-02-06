/*
 * bucket.hpp
 */
#ifndef BUCKET_HPP
#define BUCKET_HPP
#include "util.hpp"
class Bucket1 {// filling strategy 1   |xxx|xx_|xx_|xx_|xx_| for n = 11, m = 5 (d = 3)
public:
  typedef long IntT;
  IntT _n;
  IntT _m;
  IntT _qm;
  IntT _rm;
  IntT _d;
  Bucket1() {reset(0, 1);}
  Bucket1(IntT n, IntT m) { reset(n, m);}
  void reset(IntT n, IntT m) {
    Check(n >= 0 && m > 0);
    _n = n;                   // 0 <= n should be satisfied                   
    _m = m;		      // 0 < m should be satisfied
    _qm = (n / m);	      // 0 <= _qm                                     
    _rm = (n % m);	      // 0 <= _rm < _m        
    _d = ((n + m - 1) / m);   // = _qm (for _rm = 0), = (_qm+1) (for _rm > 0) 
  }
  IntT size() { return number_of_non_empty_buckets();}
  IntT number_of_items() {return _n;}
  IntT number_of_buckets() {return _m;}
  IntT bucket_size() {return _d;}
  IntT number_of_filled_buckets() {return (0 < _rm ? _rm : _m);}
  IntT number_of_partially_filled_buckets() {return ((0 < _rm && 0 < _qm) ? (_m - _rm) : 0);}
  IntT number_of_non_empty_buckets() {return (0 < _qm ? _m : _rm);}
  IntT number_of_empty_buckets() {return ((0 < _qm) ? 0 : (_m - _rm));}
  IntT number_of_items_in_filled_bucket() {return _d;}
  IntT number_of_items_in_partially_filled_bucket() {return _qm;}
  IntT find_bucket_index_from_item_index(IntT i) {// 0 <= i < _n
    if (i < ((_qm + 1) * _rm)) {// (_qm + 1) can be replaced with _d
      return (i / (_qm + 1));// (_qm + 1) can be replaced with _d
    } else {
      return ((i - _rm) / _qm);
    }
  }
  typedef std::pair<IntT, IntT> ItemRange; // exclusive end
  ItemRange item_range_for_bucket_index(IntT k) {// 0 <= k < _m
    if (k < _rm) {
      // (_qm + 1) can be replaced with _d
      return make_pair((k * (_qm + 1)), (k + 1) * (_qm + 1));
    } else {
      return make_pair((_rm + (k * _qm)), (_rm + ((k + 1) * _qm)));
    }
  }
  IntT item_begin_for_bucket_index(IntT k) 
  { return (k < _rm ? (k * (_qm + 1)) : (_rm + (k * _qm)));}
  IntT item_end_for_bucket_index(IntT k) 
  { return (k < _rm ? ((k + 1) * (_qm + 1)) : (_rm + ((k + 1) * _qm)));}
  ItemRange item_range_for_bucket_index_1(IntT k) {// 0 <= k < _m
    // (_qm + 1) can be replaced with _d
    return make_pair(min((k * (_qm + 1)),       (_rm + (k * _qm))), 
		     min(((k + 1) * (_qm + 1)), (_rm + ((k + 1) * _qm))));
  }
  IntT item_begin(IntT k) { return item_begin_for_bucket_index(k);}
  IntT item_end(IntT k) { return item_end_for_bucket_index(k);}
  class Iter 
    : public std::iterator<input_iterator_tag, IntT, std::ptrdiff_t, const IntT*, const IntT&> {
  public:
    typedef Iter               Self;
    Iter() : _bucket(0), _idx(-1) {}
    Iter(Bucket1& bucket) : _bucket(&bucket), _idx(-1) { get_next(); }
    reference operator*() { return _idx;}
    pointer operator->() { return &operator*();}
    Self& operator++() { get_next(); return *this;}
    Self operator++(int) { Self tmp(*this); operator++(); return tmp;}
    bool operator==(const Self& it) const { return is_end() == it.is_end();}
    bool operator!=(const Self& it) const { return !(*this == it);}
    bool is_end() const { return (_idx == (-1));}
    IntT begin() const { return _bucket->item_begin_for_bucket_index(_idx);}
    IntT end() const { return _bucket->item_end_for_bucket_index(_idx);}
  private:
    Bucket1* _bucket;
    IntT      _idx;
    void get_next() {
      ++_idx;
      if (_idx == 0 || _bucket->number_of_non_empty_buckets() <= _idx) { _idx = (-1);}
    }
  };
  Iter begin() { return Iter(*this);}
  Iter end() { return Iter();}
};

class Bucket2 {// filling strategy 2   |xxx|xxx|xxx|xx_|___| for n = 11, m = 5 (d = 3)
public:
  typedef long IntT;
  IntT _n;
  IntT _m;
  IntT _d;
  IntT _qm;
  IntT _rm;
  IntT _qd;
  IntT _rd;
  IntT _u;
  Bucket2() { reset(1, 1);}
  Bucket2(IntT n, IntT m) { reset(n, m);}
  void reset(IntT n, IntT m) {
    Check (n > 0 && m > 0);
    _n = n;                  // 0 <= n should be satisfied                   
    _m = m;		     // 0 < m should be satisfied                    
    _d = ((n + m - 1) / m);  // = _qm (for _rm = 0), = (_qm+1) (for _rm > 0) 
    _qm = (n / m);	     // 0 <= _qm          _n = _qm * _m + _rm        
    _rm = (n % m);	     // 0 <= _rm < _m                                
    _qd = (n / _d);	     // 0 <= _qd          _n = _qd * _d + _rd        
    _rd = (n % _d);	     // 0 <= _rd < _d                                
    _u = ((n + _d - 1) / _d);// = _qd (for _rd = 0), = (_qd+1) (for _rd > 0) 
  }
  IntT number_of_items() {return _n;}
  IntT number_of_buckets() {return _m;}
  IntT bucket_size() {return _d;}
  IntT number_of_filled_buckets() {return _qd;}
  IntT number_of_partially_filled_buckets() {return ((0 < _rd) ? 1 : 0);}
  IntT number_of_non_empty_buckets() {return _u;}
  IntT number_of_empty_buckets() {return (_m - _u);} // = ((_m*_d-_n) / _d), which is in (0.._m-1)
  IntT number_of_items_in_filled_bucket() {return _d;}
  IntT number_of_items_in_partially_filled_bucket() {return _rd;}
  IntT find_bucket_index_from_item_index(IntT i) {// 0 <= i < _n
    return (i / _d);
  }
  typedef std::pair<IntT, IntT> ItemRange; // exclusive end
  ItemRange item_range_for_bucket_index(IntT k) {// 0 <= k < _m
    if (k < _qd) {
      return make_pair((k * _d), ((k + 1) * _d));
    } else if (k == _qd) {
      return make_pair((k * _d), _n);
    } else {
      return make_pair(_n, _n);
    }  
  }
  ItemRange item_range_for_bucket_index_1(IntT k) {// 0 <= k < _m
    if (k < _u) {
      return make_pair((k * _d), min(((k + 1) * _d), _n));
    } else {
      return make_pair(_n, _n);
    }  
  }
  ItemRange item_range_for_bucket_index_2(IntT k) {// 0 <= k < _m
    return make_pair(min((k * _d), _n), min(((k + 1) * _d), _n));
  }
};
typedef Bucket1 Bucket;
#endif
