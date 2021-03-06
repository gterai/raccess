/*
 * matrix.hpp
 */
#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "util.hpp"
template <typename T, int LEN> class FixedSquareMatrix {
public:
  typedef T value_type;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

  FixedSquareMatrix() {fill(T());}
  FixedSquareMatrix(const FixedSquareMatrix& m) {
    std::copy(m.begin(), m.end(), begin());
  }
  ~FixedSquareMatrix() {}
  FixedSquareMatrix& operator=(const FixedSquareMatrix& m) {
    std::copy(m.begin(), m.end(), begin());
    return *this;
  }
  bool operator==(const FixedSquareMatrix& m) {
    return std::equal(begin(), end(), m.begin());
  }
  static int size() {return SIZE;}
  static int length() {return LEN;}
  const_reference ref(int i, int j) const {return data[get_index(i, j)];}
  reference ref(int i, int j) {return data[get_index(i, j)];}
  const_reference get(int i, int j) const {return ref(i, j);}
  void set(int i, int j, const_reference t) {ref(i, j) = t;}
  const_iterator begin() const {return data;}
  const_iterator end() const {return (data + SIZE);}
  const_iterator iter(int i, int j) const {return (begin() + get_index(i, j));}
  iterator begin() {return data;}
  iterator end() {return (data + SIZE);}
  iterator iter(int i, int j) {return (begin() + get_index(i, j));}
  void fill(const T& t) {std::fill(begin(), end(), t);}

private:
  enum {SIZE = (LEN * LEN)};
  T data[SIZE];
  static int get_index(int i, int j) {
    Assert(0 <= i && i < LEN && 0 <= j && j < LEN);
    return (i * LEN + j);
  }
};

template <typename T, typename Data = Array<T> > class Matrix {
public:
  typedef typename Data::value_type value_type;
  typedef typename Data::iterator iterator;
  typedef typename Data::const_iterator const_iterator;
  typedef typename Data::reference reference;
  typedef typename Data::const_reference const_reference;
  typedef typename Data::size_type size_type;
  typedef typename Data::difference_type difference_type;

  Matrix(int nrow, int ncol) : _data(nrow * ncol), _nrow(nrow), _ncol(ncol) {
    Assert(nrow >= 0 && ncol >= 0);
  }
  Matrix(int nrow, int ncol, const_reference t) 
    : _data(nrow * ncol, t), _nrow(nrow), _ncol(ncol) {
    Assert(nrow >= 0 && ncol >= 0);
  }
  Matrix() : _data(), _nrow(0), _ncol(0) {}
  Matrix(const Matrix& mtx) : _data(mtx._data), _nrow(mtx._nrow), _ncol(mtx._ncol) {}
  virtual ~Matrix() {}
  Matrix& operator=(const Matrix& mtx) {
    _data = mtx._data;
    _nrow = mtx._nrow;
    _ncol = mtx._ncol;
    return *this;
  }
  bool empty() const {return _data.empty();}
  int size() const {return _data.size();}
  int nrow() const {return _nrow;}
  int ncol() const {return _ncol;}
  const Data& data() const {return _data;}
  void set_size(int nrow, int ncol) {
    Assert(nrow >= 0 && ncol >= 0);
    _data.resize(nrow * ncol);
    _nrow = nrow;
    _ncol = ncol;
  }
  void fill(const_reference val) {_data.fill(val);}
  void clear() {_data.clear(); _nrow = 0; _ncol = 0;}
  void swap(Matrix& mtx) {_data.swap(mtx); swap(_nrow, mtx._nrow); swap(_ncol, mtx._ncol);}
  const_reference ref(int row, int col) const {return _data[get_index(row, col)];}
  reference ref(int row, int col) {return _data[get_index(row, col)];}
  const_iterator begin() const {return _data.begin();}
  const_iterator end() const {return _data.end();}
  iterator begin() {return _data.begin();}
  iterator end() {return _data.end();}
  const_reference get(int i, int j) const {return ref(i, j);}
  void set(int i, int j, const_reference t) {ref(i, j) = t;}
  const_iterator iter(int i, int j) const {return (_data.begin() + get_index(i, j));}
  iterator iter(int i, int j) {return (_data.begin() + get_index(i, j));}
  const_iterator operator[](int row) const {return &ref(row, 0);}
  iterator operator[](int row) {return &ref(row, 0);}
  Matrix transpose() const {
    static Data v;
    if (empty()) return Matrix();

    v.resize(_nrow * _ncol);
    const T* p0 = &_data.front();
    T*const& p_beg = &v.front();
    T* p;
    for (int i = 0; i < _nrow; i++) {
      p = (p_beg + i);
      Assert(p0 == &ref(i, 0));
      for (int j = 0; j < _ncol; j++) {
	*p = *p0++;
	p += _nrow;
      }
      Assert(p == p_beg + (_ncol * _nrow + i));
    }
    return Matrix(_ncol, _nrow, v);
  }
  template <typename InpIt>
  void assign(InpIt first, InpIt last) {
    Assert((int)size() == (int)std::distance(first, last));
    _data.assign(first, last);
  }
  // void set_from_vector(const Vector<T>& v) {
  //   Assert((int)size() == (int)v.size());
  //   _data.assign(v.begin(), v.end());
  // }
  // Vector<T> to_vector() const {return Vector<T>(_data.begin(), _data.end());}
  Array<T> to_a() const {return Array<T>(_data.begin(), _data.end());}
  class Iterator;
protected:
  Data _data;
  int _nrow;
  int _ncol;

  Matrix(int nrow, int ncol, Data& v) : _data(), _nrow(0), _ncol(0) {swap(nrow, ncol, v);}
  void swap(int nrow, int ncol, Data& v) {
    Assert(nrow >= 0 && ncol >= 0);
    Assert(nrow * ncol == (int)v.size());
    _data.swap(v);
    _nrow = nrow;
    _ncol = ncol;
  }
  inline int get_index(int row, int col) const {
    Assert(0 <= row && row < _nrow && 0 <= col && col < _ncol);
    return (row * _ncol + col);
  }
};

template <typename T, int NROW, int NCOL> struct CMatrix {
  enum { SIZE = NROW * NCOL};
  typedef CArray<T, SIZE> Data;
  Data data_;
  typedef typename Data::value_type      value_type;
  typedef typename Data::iterator        iterator;
  typedef typename Data::const_iterator  const_iterator;
  typedef typename Data::reference       reference;
  typedef typename Data::const_reference const_reference;
  typedef typename Data::size_type       size_type;
  typedef typename Data::difference_type difference_type;
  bool empty() const {return data_.empty();}
  int size() const {return data_.size();}
  const_iterator begin() const {return data_.begin();}
  iterator begin() {return data_.begin();}
  const_iterator end() const {return data_.end();}
  iterator end() {return data_.end();}
  void fill(const T& t) {data_.fill(t);}
  T* data() const {return data_.data();}
  int nrow() const {return NROW;}
  int ncol() const {return NCOL;}
  void swap(CMatrix& mtx) {data_.swap(mtx.data_);}
  const_reference ref(int row, int col) const {return data_[get_index(row, col)];}
  reference ref(int row, int col) {return data_[get_index(row, col)];}
  const_reference get(int i, int j) const {return ref(i, j);}
  void set(int i, int j, const_reference t) {ref(i, j) = t;}
  const_iterator iter(int i, int j) const {return (begin() + get_index(i, j));}
  iterator iter(int i, int j) {return (begin() + get_index(i, j));}
  CMatrix<T,NCOL,NROW> transpose() const {
    CMatrix<T,NCOL,NROW> m;
    for (int i = 0; i < NCOL; i++) {
      for (int j = 0; j < NROW; j++) {
	m.ref(i, j) = ref(j, i);
      }
    }
    return m;
  }
  template <typename InpIt>
  void assign(InpIt first, InpIt last) {data_.assign(first, last);}
  Array<T> to_a() const { return Array<T>(data_.begin(), data_.end());}
  inline int get_index(int row, int col) const {
    Assert(0 <= row && row < NROW && 0 <= col && col < NCOL);
    return (row * NCOL + col);
  }
};

template <typename T, typename Data>
class 
Matrix<T, Data>::
Iterator
{
public:
  int row;
  int col;
  T* curr;

  Iterator(Matrix& mtx) 
    : row(),
      col(),
      curr(NULL),
      mtx(mtx),
      col_end(),
      row_lim(),
      next_col(0) {
    reset();
  }
  ~Iterator() {}
  void reset() {
    if (mtx.empty()) {
      curr = NULL;
      row_lim = 0;
      row = 0;
      col_end = 0;
      next_col = 0;
      col = -1;
    } else {
      curr = (&mtx.front() - 1);
      row_lim = (mtx.nrow() - 1);
      row = 0;
      col_end = mtx.ncol();
      next_col = 0;
      col = -1;
    }
  }
  bool get_next() {
    if (col_end <= next_col) {
      if (row_lim <= row) return false;

      row++;
      next_col = 0;
    }
    col = next_col++;
    curr++;
    return true;
  }
private:
  Matrix& mtx;
  int col_end;
  int row_lim;
  int next_col;
};

template <typename T>
class 
NMatrix 
  : public Matrix<T>
{
  typedef Array<T> Data;
  typedef Matrix<T, Data> Super;
public:
  NMatrix(int nrow, int ncol) 
    : Super(nrow, ncol) {
  }
  NMatrix(int nrow, int ncol, const T& t) 
    : Super(nrow, ncol, t) {
  }
  NMatrix() : Super() {}
  NMatrix(const Super& mtx)
    : Super(mtx)
  {
  }
  ~NMatrix() {}
  NMatrix& operator=(const Super& mtx) {
    Super::operator=(mtx);
    return *this;
  }
  string to_s() const {
    ostringstream oss;
    for (int i = 0; i < Super::nrow(); i++) {
      for (int j = 0; j < Super::ncol(); j++) {
	oss << Super::ref(i, j) << " ";
      }
      oss << "\n";
    }
    return oss.str();
  }
  void print() const {cout << to_s() << flush;};
  T quadratic_form(const T*const& u, const T*const& v) const {
    static const T zero = 0;
    T z = zero;
    for (int i = 0; i < Super::_nrow; i++) {
      for (int j = 0; j < Super::_ncol; j++) {
	z += u[i] * v[j] * Super::ref(i, j);
      }
    }
    return z;
  }
  void scale(const T& t) {
    std::transform(Super::begin(), Super::end(), Super::begin(),
		   std::bind2nd(multiplies<T>(), t));
  }
  template <typename array_t>
  array_t multiply(const array_t& v) const {
    static const T zero = 0;
    Assert((int)Super::_ncol == (int)v.size());
    array_t u(Super::_nrow);
    typename array_t::iterator i = u.begin();
    int nrow = Super::_nrow;
    typename Super::const_iterator k = Super::begin();
    const int ncol0 = Super::_ncol;
    int ncol = ncol0;
    typename array_t::const_iterator const& j0 = v.begin(); 
    typename array_t::const_iterator j = j0; 
    while (nrow-- > 0) {
      T z = zero;
      while (ncol-- > 0) {
	z += (*k++) * (*j++);
      }
      (*i++) = z;
      ncol = ncol0;
      j = j0;
    }
    return u;
  }
  template <typename array_t>
  array_t multiply_transposed(const array_t& v) const {
    static const T zero = 0;
    Assert((int)Super::_nrow == (int)v.size());
    array_t u(Super::_ncol);
    typename array_t::iterator j = u.begin();
    const int ncol0 = Super::_ncol;
    int ncol = ncol0;
    const int nrow0 = Super::_nrow;
    int nrow = nrow0;
    typename array_t::const_iterator const& i0 = v.begin();
    typename array_t::const_iterator i = i0;
    typename Super::const_iterator k0 = Super::begin();
    typename Super::const_iterator k = (k0++);
    
    while (ncol-- > 0) {
      T z = zero;
      while (nrow-- > 0) {
	z += (*i++) * (*k);
	k += ncol0;
      }
      (*j++) = z;
      nrow = nrow0;
      i = i0;
      k = (k0++);
    }
    return u;
  }
  NMatrix multiply_transposed_mtx(const NMatrix& mtx) const {
    static const T zero = 0; // needed
    Assert(Super::_ncol == mtx._ncol);
    if (Super::empty()) return NMatrix();

    Data v(Super::_nrow * mtx._nrow);
    T* p = &v.front();
    const T* p1 = &Super::front();
    const T* p2 = &Super::front();
    for (int i = 0; i < Super::_nrow; i++) {
      const T*const& b = p1;
      const T*const& e = (b + Super::_ncol);
      Assert(p1 == &Super::ref(i, 0));
      for (int j = 0; j < mtx._nrow; j++) {
	Assert(p2 == &mtx.ref(j, 0));
	*p++ = std::inner_product(b, e, p2, zero);
      }
      p1 = e;
      p2 += Super::_ncol;
    }
    return NMatrix(Super::_nrow, mtx._nrow, v);
  }
  NMatrix multiply(const NMatrix& mtx) const {
    return multiply_transposed_mtx(mtx.transpose());
  }
  void add(const NMatrix& mtx) {
    Assert(Super::_nrow == mtx._nrow && Super::_ncol == mtx._ncol);
    std::transform(Super::begin(), Super::end(), mtx.begin(), 
		   Super::begin(), plus<T>());
  }
  void subtract(const NMatrix& mtx) {
    Assert(Super::_nrow == mtx._nrow && Super::_ncol == mtx._ncol);
    std::transform(Super::begin(), Super::end(), mtx.begin(), 
		   Super::begin(), minus<T>());
  }
private:
  NMatrix(int nrow, int ncol, Data& v)
    : Super(nrow, ncol, v) {// destroy v
  }
};

#endif
