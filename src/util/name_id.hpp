/*
 * author: hisanori kiryu
 */
#ifndef UTIL__NAME_ID_HPP
#define UTIL__NAME_ID_HPP
#include "util/util.hpp"
template <typename T = int> class NameId { // requirement: string have no new line chars
public:
  typedef NameId                 Self;
  typedef T                      IntT;
  typedef hash_map<string, IntT> Table;
  typedef typename hash_map<string, IntT>::const_iterator TIter;
  typedef vector<string>         Names;
  typedef Names::const_iterator  iterator;
  typedef Names::const_reference reference;
  enum { NULL_ID = (IntT)(-1)};
  Table _table;
  Names _names;
  NameId() {}
  template <typename Iter> NameId(Iter b, Iter e) { append(b, e);}
  NameId(const vector<string>& names) { assign(names);}
  static bool is_null_id(IntT id) { return (id == NULL_ID);}
  static IntT null_id() { return NULL_ID;}
  bool empty() const { return _names.empty();}
  IntT size() const { return _names.size();}
  void clear() { _table.clear(); _names.clear();}
  bool has_name(const string& s) const { return (_table.find(s) != _table.end());}
  IntT operator[](const string& s) const { 
    TIter i = _table.find(s); 
    Check(i != _table.end(), "cannot find %s", s.c_str());
    return i->second;
  }
  IntT find(const string& s) const {
    TIter i = _table.find(s);
    return (i == _table.end() ? NULL_ID : i->second);
  }
  Self& assign(const vector<string>& names) { return assign(names.begin(), names.end());}
  const string& operator[](const IntT& id) const { return _names[id];}
  void push_back(const string& s) {
    const pair<typename Table::iterator, bool>& p = _table.insert(make_pair(s, _names.size()));
    Check(p.second, "entry %s exists", s.c_str());
    _names.push_back(s);
  }
  Self& operator<<(const string& s) { push_back(s); return *this;}
  void sort() { sort(less<string>());}
  template <typename Comp> void sort(Comp comp) {
    std::sort(_names.begin(), _names.end(), comp);
    for (IntT i = 0; i < (IntT)_names.size(); ++i) { _table[_names[i]] = i;}
  }
  iterator begin() const { return _names.begin();}
  iterator end() const { return _names.end();}
  iterator iter(const IntT& id) const { return (_names.begin() + id);}
  reference front() const { return _names.front();}
  reference back() const { return _names.back();}
  template <typename Iter> Self& assign(Iter b, Iter e) { clear(); append(b, e); return *this;}
  template <typename Iter> void append(Iter b, Iter e) { for (; b != e; ++b) { push_back(*b);}}
  void erase(const IntT& id) {
    const string& s = _names[id];
    _table.erase(s);
    _names.erase(_names.begin() + id);
    for (IntT i = id; i < (IntT)_names.size(); ++i) { _table[_names[i]] = i;}
  }
  void erase(const string& s) { IntT id = _table[s]; /*local copy*/; erase(id);}
  void save(const string& fname) const { ofstream_(fo, fname); save(fo);}
  void load(const string& fname) { ifstream_(fi, fname); load(fi);}
  void save(ostream& fo) const 
  { for (iterator i = begin(); i != end(); ++i) { fo << *i << '\n';}}
  void load(istream& fi) 
  { clear(); for (LineIter i = LineIter(fi); i != LineIter(); ++i) { push_back(*i);}}
  void load_from_col(const string& fname, IntT col) { ifstream_(fi, fname); load_from_col(fi, col);}
  void load_from_col(istream& fi, IntT col) { 
    clear(); 
    for (LineIter i = LineIter(fi); i != LineIter(); ++i) {
      const vector<string>& a = splitv(*i, "\t"); Check(col < (IntT)a.size());
      push_back(a[col]);
    }
  }
};
#endif
