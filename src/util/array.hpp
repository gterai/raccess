/*
 * Array.hpp
 */
#ifndef ARRAY_HPP
#define ARRAY_HPP
// c_array class: fixed array. size is statically determined.
template <typename T, std::size_t SIZE> class CArray {
public:
  //type definitions
  typedef CArray         Self;
  typedef T              value_type;
  typedef T*             iterator;
  typedef const T*       const_iterator;
  typedef T&             reference;
  typedef const T&       const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

  bool operator==(const Self& other) const { return std::equal(begin(), end(), other.begin());}
  bool operator!=(const Self& other) const { return !(*this == other);}

  //iterator support
  iterator begin() {return v;}
  const_iterator begin() const {return v;}
  iterator end() {return v + SIZE;}
  const_iterator end() const {return v + SIZE;}

  reference front() {return get(0);}
  const_reference front() const {return get(0);}
  reference back() {return get(SIZE - 1);}
  const_reference back() const {return get(SIZE - 1);}
  
  //direct element access
  reference operator[](size_type i) {return get(i);}
  const_reference operator[](size_type i) const {return get(i);}

  //size is constant
  static bool empty() {return (SIZE==0);}
  static size_type size() {return SIZE;}
  static size_type max_size() {return SIZE;}
  static size_type bytesize() {return SIZE * sizeof(value_type);}

  //conversion to ordinary array
  T* data() {return v;}

  void clear() {}
  void fill(const T& t) { std::fill(begin(), end(), t);}
  void swap(CArray& carray) { std::swap_ranges(begin(), end(), carray.begin());}
  template <typename InpIt> void assign(InpIt first, InpIt last) 
  { for (iterator i = begin(); (first != last) && (i != end()); ++first, ++i) { (*i) = *first;}}
  template <typename V> void assign(const V& v) {assign(v.begin(), v.end());}

  //protected: make public to enable assignment initialization
  T v[SIZE]; // fixed-size array of elements of type T
  reference get(size_type i) { Assert(0 < SIZE && i < SIZE); return v[i];}
  const_reference get(size_type i) const { Assert(0 < SIZE && i < SIZE); return v[i];}
};

template <typename T> class CArray<T, 0> {
public:
  enum {SIZE = 0};
  enum {v = 0};
  //type definitions
  typedef CArray         Self;
  typedef T              value_type;
  typedef T*             iterator;
  typedef const T*       const_iterator;
  typedef T&             reference;
  typedef const T&       const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

  bool operator==(const Self& other) const {return true;}
  bool operator!=(const Self& other) const {return false;}

  //iterator support
  iterator begin() {return v;}
  const_iterator begin() const {return v;}
  iterator end() {return v + SIZE;}
  const_iterator end() const {return v + SIZE;}

  reference front() {return get(0);}
  const_reference front() const {return get(0);}
  reference back() {return get(SIZE - 1);}
  const_reference back() const {return get(SIZE - 1);}
  
  //direct element access
  reference operator[](size_type i) {return get(i);}
  const_reference operator[](size_type i) const {return get(i);}

  //size is constant
  static bool empty() {return (SIZE==0);}
  static size_type size() {return SIZE;}
  static size_type max_size() {return SIZE;}
  static size_type bytesize() { return SIZE * sizeof(value_type);}

  //conversion to ordinary array
  T* data() {return v;}

  void clear() {}
  void fill(const T& t) {}
  void swap(CArray& carray) {}
  
  reference get(size_type i) { Die("size == 0"); static T t; return t;}
  const_reference get(size_type i) const { Die("size == 0"); static T t; return t;}
};

// array class: size is dynamically determined.
// needs default constructer T()
template <typename T> class Array {
public:
  //type definitions
  typedef T              value_type;
  typedef T*             iterator;
  typedef const T*       const_iterator;
  typedef T&             reference;
  typedef const T&       const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

  Array() 
    : _data(new T[0]), _size(0) {}
  Array(size_type size0)
    : _data(new T[size0]), _size(size0) {}
  Array(size_type size0, const T& t)
    : _data(new T[size0]), _size(size0) {fill(t);}
  Array(const Array& array) 
    : _data(new T[array._size]), _size(array._size) {
    std::copy(array.begin(), array.end(), begin());
  }
  template <typename InpIt>
  Array(InpIt first, InpIt last) 
    : _data(new T[distance(first, last)]), _size(distance(first, last)) {
    std::copy(first, last, begin());
  }
  Array& operator=(const Array& array) {
    if (size() != array.size()) { resize(array.size());}
    std::copy(array.begin(), array.end(), begin());
    return *this;
  }
  virtual ~Array() {delete[] _data;}
  bool operator==(const Array& array) const {
    if (size() != array.size()) return false;

    return std::equal(begin(), end(), array.begin());
  }
  bool operator!=(const Array& array) const {
    return !(*this == array);
  }

  //iterator support
  iterator begin() {return _data;}
  const_iterator begin() const {return _data;}
  iterator end() {return (_data + _size);}
  const_iterator end() const {return (_data + _size);}

  reference front() {return get(0);}
  const_reference front() const {return get(0);}
  reference back() {return get(_size - 1);}
  const_reference back() const {return get(_size - 1);}
  
  //direct element access
  reference operator[](size_type i) {return get(i);}
  const_reference operator[](size_type i) const {return get(i);}

  bool empty() const {return (_size == 0);}
  size_type size() const {return _size;}
  //size_type max_size() const {return _size;}

  //conversion to ordinary array
  T* data() {return _data;}

  void clear() {
    delete[] _data;
    _data = new T[0];
    _size = 0;
  }
  void fill(const T& t) {std::fill(begin(), end(), t);}
  void swap(Array& array) {
    std::swap(_data, array._data);
    std::swap(_size, array._size);
  }
  void resize(size_type new_size, const T& t = T()) {
    if (size() == new_size) return;

    // T*const& v = new T[new_size];
    // const size_type& n = std::min(_size, new_size);
    // std::copy(begin(), (begin() + n), v);
    // std::fill((v + n), (v + new_size), t);
    // delete[] _data;
    // _data = v;
    // _size = new_size;
    Array a(new_size);
    const size_type& n = std::min(size(), new_size);
    std::copy(begin(), (begin() + n), a.begin());
    std::fill((a.begin() + n), a.end(), t);
    swap(a);
  }
  void erase(iterator b, iterator e) {
    if (b == e) return;

    Array a(size() - (e - b));
    std::copy(begin(), b, a.begin());
    std::copy(e, end(), (a.begin() + (e - b)));
    swap(a);
  }
  template <typename InpIt>
  void assign(InpIt first, InpIt last) {
    const int& n = std::distance(first, last);
    if (n != (int)size()) {
      resize(n);
    }
    std::copy(first, last, begin());
  }
  template <typename ArrayT>
  void assign(const ArrayT& a) {assign(a.begin(), a.end());}

protected:
  T* _data;
  size_type _size;
  reference get(size_type i) { Assert(0 < _size && i < _size); return _data[i];}
  const_reference get(size_type i) const { Assert(0 < _size && i < _size); return _data[i];}
};

// l_array class: size is dynamically determined.
// needs default constructer T()
template <typename T>
class LArray {
public:
  //type definitions
  typedef Array<T>       Data;
  typedef T              value_type;
  typedef T*             iterator;
  typedef const T*       const_iterator;
  typedef T&             reference;
  typedef const T&       const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;
  enum {SIZE_FACTOR = 2};

  LArray() 
    : _data(), _size(0) {}
  LArray(size_type size0)
    : _data(size0), _size(size0) {}
  LArray(size_type size0, const_reference t)
    : _data(size0, t), _size(size0) {}
  LArray(const LArray& array) 
    : _data(array._data), _size(array._size) {
  }
  template <typename InpIt>
  LArray(InpIt first, InpIt last) 
    : _data(first, last), _size(distance(first, last)) {
  }
  LArray& operator=(const LArray& array) {
    resize(array.size());
    std::copy(array.begin(), array.end(), begin());
    return *this;
  }
  virtual ~LArray() {}
  bool operator==(const LArray& array) const {
    if (size() != array.size()) return false;

    return std::equal(begin(), end(), array.begin());
  }
  bool operator!=(const LArray& array) const {
    return !(*this == array);
  }

  //iterator support
  iterator begin() {return _data.begin();}
  const_iterator begin() const {return _data.begin();}
  iterator end() {return (_data.begin() + _size);}
  const_iterator end() const {return (_data.begin() + _size);}

  reference front() {return get(0);}
  const_reference front() const {return get(0);}
  reference back() {return get(_size - 1);}
  const_reference back() const {return get(_size - 1);}
  
  iterator data_begin() {return _data.begin();}
  const_iterator data_begin() const {return _data.begin();}
  iterator data_end() {return _data.end();}
  const_iterator data_end() const {return _data.end();}

  //direct element access
  reference operator[](size_type i) {return get(i);}
  const_reference operator[](size_type i) const {return get(i);}

  void push_back(const_reference t) {
    _size++;
    if (_data.size() < _size) {
      _data.resize(SIZE_FACTOR * (_size));
    }
    back() = t;
  }
  void pop_back() {Assert(_size > 0); _size--;}

  size_type size() const {return _size;}

  bool empty() const {return (_size == 0);}
  const Data& data() {return _data;}

  void clear() {_size = 0;}
  void clear_data() {_data.clear(); _size = 0;}
  void fill(const_reference t) {std::fill(begin(), end(), t);}
  void swap(LArray& array) {
    std::swap(_data, array._data);
    std::swap(_size, array._size);
  }
  void resize(size_type new_size) {
    size_type n = _data.size();
    std::fill(end(), begin() + std::min(new_size, n), T());
    _size = new_size;
    if (n < _size) {
      _data.resize(SIZE_FACTOR * _size);
    }
  }
  void reserve(size_type new_size) {
    if (_data.size() < new_size) {
      _data.resize(new_size);
    }
  }
  template <typename InpIt>
  void assign(InpIt first, InpIt last) {
    resize(std::distance(first, last));
    std::copy(first, last, begin());
  }

protected:
  Data _data;
  size_type _size;

  reference get(size_type i) { Assert(0 < _size && i < _size); return _data[i];}
  const_reference get(size_type i) const { Assert(0 < _size && i < _size); return _data[i];}
};
#endif
