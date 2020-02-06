/*
 * tensor.hpp
 *
 */
#ifndef TENSOR_HPP
#define TENSOR_HPP
#include "tensor_index.hpp"
// fixed rank tensor may be convenient
template <typename T, typename Data = vector<T> > class Tensor {
public:
  typedef Tensor                         Self;
  typedef typename Data::value_type      value_type;
  typedef typename Data::iterator        iterator;
  typedef typename Data::const_iterator  const_iterator;
  typedef typename Data::reference       reference;
  typedef typename Data::const_reference const_reference;
  typedef typename Data::size_type       size_type;
  typedef typename Data::difference_type difference_type;
  typedef difference_type                IntT;
  typedef TensorIndex<IntT, true>        Tidx;
  typedef vector<IntT>                   Idxs;
  Data _data;
  Tidx _tidx;
  bool empty() const { return _data.empty();}
  size_type size() const { return _data.size();}
  iterator begin() { return _data.begin();}
  const_iterator begin() const { return _data.begin();}
  iterator end() { return _data.end();}
  const_iterator end() const { return _data.end();}
  reference front() { return _data.front();}
  const_reference front() const { return _data.front();}
  reference back() { return _data.back();}
  const_reference back() const { return _data.back();}
  void fill(const_reference t) { std::fill(_data.begin(), _data.end(), t);}
  void clear() { _data.clear(); _tidx.clear();}
  IntT max_rank() const { return _tidx.max_rank();}
  IntT rank() const { return _tidx.rank();}
  IntT length(IntT i) const { return _tidx.length(i);}
  template <typename Iter> void set_size(Iter b, Iter e) 
  { _tidx.set_ranges(b, e); _data.resize(_tidx.size());}
  template <typename Iter> IntT index(Iter b, Iter e) const { return _tidx.index(b, e);}
  Idxs indexes(IntT n) const { return _tidx.indexes(n);}
  reference ref() { return _data[_tidx.ref()];}
  reference ref(IntT i0) { return _data[_tidx.ref(i0)];}
  reference ref(IntT i0, IntT i1) { return _data[_tidx.ref(i0, i1)];}
  reference ref(IntT i0, IntT i1, IntT i2) { return _data[_tidx.ref(i0, i1, i2)];}
  reference ref(IntT i0, IntT i1, IntT i2, IntT i3) { return _data[_tidx.ref(i0, i1, i2, i3)];}
  const_reference ref() const { return _data[_tidx.ref()];}
  const_reference ref(IntT i0) const{ return _data[_tidx.ref(i0)];}
  const_reference ref(IntT i0, IntT i1) const{ return _data[_tidx.ref(i0, i1)];}
  const_reference ref(IntT i0, IntT i1, IntT i2) const{ return _data[_tidx.ref(i0, i1, i2)];}
  const_reference ref(IntT i0, IntT i1, IntT i2, IntT i3) const
  { return _data[_tidx.ref(i0, i1, i2, i3)];}
  template <typename Block> IntT each(Block block)
  { return _tidx.each(proc_l(&Self::each_block, *this, block));}
  template <typename Block> IntT each_block(Block block, const vector<T>& idxs, T offset)
  { return block(idxs, offset, _data[offset]);}
  std::string to_s() const {
    std::ostringstream oss;
    for (IntT i = 0; i < (IntT)_data.size(); i++) {
      const Idxs& idxs = indexes(i);
      oss << "[ ";
      for (IntT k = 0; k < (IntT)idxs.size(); k++) {
	oss << idxs[k] << (k == ((IntT)idxs.size() - 1) ? " " : ", ");
      }
      oss << "]: " << _data[i] << "\n";
    }
    return oss.str();
  }
  void print() const {std::cout << to_s() << std::flush;}
};

template <typename T, typename T1> class TensorBase {
public:
  typedef T1           IntT;
  typedef T            value_type;
  typedef T*           iterator;
  typedef T const*     const_iterator;
  typedef T&           reference;
  typedef T const&     const_reference;
  typedef std::size_t  size_type;
  typedef IntT         difference_type;
  typedef vector<IntT> Idxs;

  virtual ~TensorBase() {}
  virtual bool empty() const = 0;
  virtual size_type size() const = 0;
  virtual iterator begin() = 0;
  virtual const_iterator begin() const = 0;
  virtual iterator end() = 0;
  virtual const_iterator end() const = 0;
  virtual reference front() = 0;
  virtual const_reference front() const = 0;
  virtual reference back() = 0;
  virtual const_reference back() const = 0;
  virtual void fill(const_reference t) = 0;
  virtual void clear() = 0;
  virtual size_type rank() const = 0;
  virtual size_type  max_rank() const = 0;
  virtual size_type length(IntT i) const = 0;
  virtual Idxs lengths() const = 0;
  virtual Idxs strides() const = 0;
  virtual IntT index(IntT* b, IntT* e) const = 0;
  virtual IntT index(typename Idxs::const_iterator b, typename Idxs::const_iterator e) const = 0;
  virtual Idxs indexes(IntT n) const = 0;
  virtual reference ref() = 0;
  virtual reference ref(IntT i0) = 0;
  virtual reference ref(IntT i0, IntT i1) = 0;
  virtual reference ref(IntT i0, IntT i1, IntT i2) = 0;
  virtual reference ref(IntT i0, IntT i1, IntT i2, IntT i3) = 0;
  virtual const_reference ref() const = 0;
  virtual const_reference ref(IntT i0) const = 0;
  virtual const_reference ref(IntT i0, IntT i1) const = 0;
  virtual const_reference ref(IntT i0, IntT i1, IntT i2) const = 0;
  virtual const_reference ref(IntT i0, IntT i1, IntT i2, IntT i3) const = 0;
  virtual std::string to_s() const {return "";}
  virtual void print() const { std::cout << to_s() << std::flush;}
};

template <typename T, int L0 = (-1), int L1 = (-1), int L2 = (-1), int L3 = (-1)>
class CTensor : public TensorBase<T, int> {// Lk should be set to non negative values
public:
  typedef CTensor                         Self;
  typedef TensorBase<T, int>              Super;
  typedef typename Super::value_type      value_type;
  typedef typename Super::iterator        iterator;
  typedef typename Super::const_iterator  const_iterator;
  typedef typename Super::reference       reference;
  typedef typename Super::const_reference const_reference;
  typedef typename Super::size_type       size_type;
  typedef typename Super::difference_type difference_type;
  typedef difference_type                 IntT;
  typedef CTensorIndex<IntT,L0,L1,L2,L3>  Tidx;
  typedef CArray<T, Tidx::SIZE>           Data;
  typedef vector<IntT>                    Idxs;
  Data _data;
  Tidx _tidx;
  bool empty() const { return _data.empty();}
  size_type size() const { return _data.size();}
  iterator begin() { return _data.begin();}
  const_iterator begin() const { return _data.begin();}
  iterator end() { return _data.end();}
  const_iterator end() const { return _data.end();}
  reference front() { return _data.front();}
  const_reference front() const { return _data.front();}
  reference back() { return _data.back();}
  const_reference back() const { return _data.back();}
  void fill(const_reference t) { _data.fill(t);}
  void clear() { _data.clear();}
  size_type rank() const { return _tidx.rank();}
  size_type max_rank() const { return _tidx.max_rank();}
  size_type length(IntT i) const { return _tidx.length(i);}
  Idxs lengths() const { return _tidx.lengths();}
  Idxs strides() const { return _tidx.strides();}
  template <typename Iter> IntT index(Iter b, Iter e) const { return _tidx.index(b, e);}
  IntT index(IntT* b, IntT* e) const { return _tidx.index(b, e);}
  IntT index(typename Idxs::const_iterator b, typename Idxs::const_iterator e) const 
  { return _tidx.index(b, e);}
  Idxs indexes(IntT n) const { return _tidx.indexes(n);}
  reference ref() { return _data[_tidx.ref()];}
  reference ref(IntT i0) { return _data[_tidx.ref(i0)];}
  reference ref(IntT i0, IntT i1) { return _data[_tidx.ref(i0, i1)];}
  reference ref(IntT i0, IntT i1, IntT i2) { return _data[_tidx.ref(i0, i1, i2)];}
  reference ref(IntT i0, IntT i1, IntT i2, IntT i3) { return _data[_tidx.ref(i0, i1, i2, i3)];}
  const_reference ref() const { return _data[_tidx.ref()];}
  const_reference ref(IntT i0) const{ return _data[_tidx.ref(i0)];}
  const_reference ref(IntT i0, IntT i1) const{ return _data[_tidx.ref(i0, i1)];}
  const_reference ref(IntT i0, IntT i1, IntT i2) const{ return _data[_tidx.ref(i0, i1, i2)];}
  const_reference ref(IntT i0, IntT i1, IntT i2, IntT i3) const
  { return _data[_tidx.ref(i0, i1, i2, i3)];}
  iterator iter() { return (begin() + _tidx.ref());}
  iterator iter(IntT i0) { return (begin() + _tidx.ref(i0));}
  iterator iter(IntT i0, IntT i1) { return (begin() + _tidx.ref(i0, i1));}
  iterator iter(IntT i0, IntT i1, IntT i2) { return (begin() + _tidx.ref(i0, i1, i2));}
  iterator iter(IntT i0, IntT i1, IntT i2, IntT i3) 
  { return (begin() + _tidx.ref(i0, i1, i2, i3));}
  const_iterator iter() const { return (begin() + _tidx.ref());}
  const_iterator iter(IntT i0) const{ return (begin() + _tidx.ref(i0));}
  const_iterator iter(IntT i0, IntT i1) const{ return (begin() + _tidx.ref(i0, i1));}
  const_iterator iter(IntT i0, IntT i1, IntT i2) const{ return (begin() + _tidx.ref(i0, i1, i2));}
  const_iterator iter(IntT i0, IntT i1, IntT i2, IntT i3) const
  { return (begin() + _tidx.ref(i0, i1, i2, i3));}
  template <typename Block> IntT each(Block block) 
  { return _tidx.each(proc_l(&Self::each_block, *this, block));}
  template <typename Block> IntT each_block(Block block, const Idxs& idxs, T offset)
  { return block(idxs, offset, _data[offset]);}
  std::string to_s() const {
    std::ostringstream oss;
    for (IntT i = 0; i < (IntT)_data.size(); i++) {
      const Idxs& idxs = indexes(i);
      oss << "[ ";
      for (IntT k = 0; k < (IntT)idxs.size(); k++) {
	oss << idxs[k] << (k == ((IntT)idxs.size() - 1) ? " " : ", ");
      }
      oss << "]: " << _data[i] << "\n";
    }
    return oss.str();
  }
  void print() const { std::cout << to_s() << std::flush;}
};

#endif
