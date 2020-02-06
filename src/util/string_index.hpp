/*
 * string_index.hpp
 */
#ifndef STRING_INDEX_HPP
#define STRING_INDEX_HPP
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <ext/hash_map>
template <typename T> class OSet {
public:
  typedef T                                       value_type;
  typedef T&                                      reference;
  typedef const T&                                const_reference;
  typedef typename std::vector<T>::iterator       iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef std::size_t                             size_type;
  typedef std::ptrdiff_t                          difference_type;
private:
  typedef __gnu_cxx::hash_map<value_type, difference_type> Hash;
  typedef std::vector<value_type> Vector;
  Hash   _h;
  Vector _v;
public:
  enum {null_index = (difference_type)(-1)};
  bool empty() const {return _v.empty();}
  size_type size() const {return _v.size();}
  const_iterator begin() const {return _v.begin();}
  const_iterator end() const {return _v.end();}
  const_reference front() const {return _v.front();}
  const_reference back() const {return _v.back();}
  void clear() {_h.clear(); _v.clear();}
  bool include(const_reference s) const {return _h.find(s) != _h.end();}
  const_reference operator[](difference_type i) const {return _v[i];}
  difference_type index(const_reference s) const {
    typename Hash::const_iterator i = _h.find(s);
    return (i == _h.end() ? null_index : i->second);
  }
  void push_back(const_reference s) {
    if (include(s)) return;
    _h.insert(std::make_pair(s, _v.size()));
    _v.push_back(s);
  }
  void pop_back() {
    _h.erase(_v.back());
    _v.pop_back();
  }
  OSet& operator<<(const_reference s) {push_back(s); return *this;}
  OSet& operator=(const OSet& other) {
    _h = other._h;
    _v = other._v;
    return *this;
  }
  bool operator==(const OSet& other) const {
    return (size() == other.size() && std::equal(_v.begin(), _v.end(), other._v.begin()));
  }
  bool operator!=(const OSet& other) const {return !(*this == other);}
  template <typename InpIt>
  void assign(InpIt first, InpIt last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first); // includes uniqueness check
    }
  }
  void swap(OSet& other) {
    _h.swap(other._h);
    _v.swap(other._v);
  }
  const_iterator erase(const_iterator b, const_iterator e) {
    for (const_iterator i = b; i != e; ++i) {
      _h.erase(*i);
    }
    iterator i = _v.erase(_v.begin() + std::distance(const_iterator(_v.begin()), b), 
			  _v.begin() + std::distance(const_iterator(_v.begin()), e));
    difference_type k = std::distance(_v.begin(), i);
    const_iterator j = (const_iterator(_v.begin()) + k);
    for (; i != _v.end(); ++i) {
      _h[*i] = k++;
    }
    return j;
  }
  void sort() {
    std::sort(_v.begin(), _v.end());
    difference_type k = 0;
    for (const_iterator i = _v.begin(); i != _v.end(); ++i) {
      _h[*i] = k++;
    }
  }
};
typedef OSet<std::string> StringIndex;
#endif
