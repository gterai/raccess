/*
 * author: hisanori kiryu
 */
#ifndef ID_CONV_HPP
#define ID_CONV_HPP
class IdConv {
public:
  typedef long         IntT;
  typedef vector<IntT> V;
  enum { null_val = (-1)}
  V _x2y;
  V _y2x;
  void clear() { _x2y.clear(); _y2x.clear();}
  bool empty() const { return _x2y.empty();}
  IntT x_size() const { return _x2y.size();}
  IntT y_size() const { return _y2x.size();}
  bool has_x(IntT x) const { return (x < (IntT)_x2y.size() && _x2y[x] != null_val);}
  bool has_y(IntT y) const { return (y < (IntT)_y2x.size() && _y2x[y] != null_val);}
  IntT x2y(IntT x) const { Assert(has_x(x)); return _x2y[x];}
  IntT y2x(IntT y) const { Assert(has_y(y)); return _y2x[y];}
  void add_xy(IntT x, IntT y) {
    Check(x >= 0 && y >= 0 && !has_x(x) && !has_y(y));
    if ((IntT)_x2y.size() <= x) { _x2y.resize(x + 1, null_val);}
    if ((IntT)_y2x.size() <= y) { _y2x.resize(y + 1, null_val);}
    _x2y[x] = y;
    _y2x[y] = x;
  }
  IntT add_x(IntT x) {
    Check(x >= 0 && !has_x(x));
    if ((IntT)_x2y.size() <= x) { _x2y.resize(x + 1, null_val);}
    IntT y = _y2x.size();
    _y2x.push_back(x);
    _x2y[x] = y;
    return y;
  }
  IntT add_y(IntT y) {
    Check(y >= 0 && !has_y(y));
    if ((IntT)_y2x.size() <= y) { _y2x.resize(y + 1, null_val);}
    IntT x = _x2y.size();
    _x2y.push_back(y);
    _y2x[y] = x;
    return x;
  }
};
#endif
