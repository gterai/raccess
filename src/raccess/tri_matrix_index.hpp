/*
 * tri_matrix_index.hpp
 */
#ifndef TRI_MATRIX_INDEX_HPP
#define TRI_MATRIX_INDEX_HPP
template <typename T> class TriMatrixIndex {
public:
  typedef TriMatrixIndex Self;
  typedef T              IntT;
  typedef vector<IntT>   V;
  typedef V              Rows;
  IntT _nlayer;
  IntT _length;
  IntT _period;
  IntT _area;
  IntT _size;
  bool _row_major;
  IntT _range_b;
  IntT _range_e; // exclusive end
  Rows _rows;
  TriMatrixIndex() : _nlayer(0), _length(0), _period(0), _area(0), _size(0), 
		     _row_major(true), _range_b(0), _range_e(0) {}
  ~TriMatrixIndex() {}
  IntT nlayer() const { return _nlayer;}
  IntT length() const { return _length;}
  IntT period() const { return _period;}
  IntT area() const { return _area;}
  IntT size() const { return _size;}
  bool row_major() const { return _row_major;}
  IntT range_b() const { return _range_b;}
  IntT range_e() const { return _range_e;}
  void flip_order() { _row_major = !_row_major;}
  void set_size(IntT nlayer, IntT length, IntT period, bool row_major) {
    Check(nlayer >= 0 && length >= 0 && period >= 0);
    _nlayer    = nlayer;
    _length    = length;
    _period    = period;
    _area      = (_length * _period);
    _size      = (_nlayer * _length * _period);
    _row_major = row_major;
  }
  IntT ref(IntT s, IntT i, IntT j, bool row_major) const {
    Assert(((0 <= s && s < _nlayer) && (i <= j && (j - i) < _length)), 
	   "[%d,%d,%d,%d] out of range [%d,%d]", s, i, j, _row_major, _nlayer, _length);
    if (row_major) { 
      return (s * _area + _length * cyclic_index(i, _period) + (j - i));
    } else { 
      return (s * _area + _length * cyclic_index(j, _period) + ((_length - 1) - (j - i)));
    }
  }
  void set_range(IntT b, IntT e) {
    _range_b = b;
    _range_e = e;
    _rows.resize(e - b);
    if (_row_major) { 
      for (IntT i = b; i < e; ++i) { 
	_rows[i - _range_b] = (_length * cyclic_index(i, _period) - i);
      }
    } else {
      for (IntT j = b; j < e; ++j) {
	_rows[j - _range_b] = (_length * cyclic_index(j, _period) + ((_length - 1) - j));
      }
    }
  }
  IntT ref(IntT s, IntT i, IntT j) const {
    if (_row_major) {
      Assert(_range_b <= i && i < _range_e && i <= j && j < (i + _length),
	    "s=%d,i=%d,j=%d,rb=%d,re=%d,l=%d", s, i, j, _range_b, _range_e, _length);
      return (s * _area + _rows[i - _range_b] + j);
    } else {
      Assert(_range_b <= j && j < _range_e && i <= j && (j - _length) < i,
	    "s=%d,i=%d,j=%d,rb=%d,re=%d,l=%d", s, i, j, _range_b, _range_e, _length);
      return (s * _area + _rows[j - _range_b] + i);
    }
  }
  template <typename Data> void flip_order_forward(Data& data, IntT i0) {
    if (_row_major) { flip_order_row_major(data, (i0 + _period - 1));}
    else { flip_order_col_major(data, i0);}
  }
  template <typename Data> void flip_order_backward(Data& data, IntT j0) {
    if (_row_major) { flip_order_row_major(data, j0);}
    else { flip_order_col_major(data, (j0 - _period + 1));}
  }
  template <typename Data> void flip_order_row_major(Data& data, IntT j0) {
    for (IntT s = 0; s < _nlayer; ++s) {
      for (IntT j = j0; j > (j0 - _period); --j) {
	for (IntT i = j; i > max((j - _length), (j0 - _period)); --i) {
	  data[ref(s, i, j, false)] = data[ref(s, i, j, true)];
	}
      }
    }
  }
  template <typename Data> void flip_order_col_major(Data& data, IntT i0) {
    for (IntT s = 0; s < _nlayer; ++s) {
      for (IntT i = i0; i < (i0 + _period); ++i) {
	for (IntT j = i; j < min((i + _length), (i0 + _period)); ++j) {
	  data[ref(s, i, j, true)] = data[ref(s, i, j, false)];
	}
      }
    }
  }
};
#endif
