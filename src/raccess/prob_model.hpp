/*
 * author: hisanori kiryu
 */
#ifndef RACCESS__PROB_MODEL_HPP
#define RACCESS__PROB_MODEL_HPP
#include "util/util.hpp"
#include "util/tensor_index.hpp"
#include "raccess/tri_matrix_index.hpp"
#include "raccess/score.hpp"
#include "raccess/dp_matrix.hpp"
namespace Raccess { template <typename ScoreModel> class ProbModel;}
template <typename ScoreModel> class Raccess::ProbModel {
public:
  typedef ProbModel               Self;
  typedef ProbModel               PM;
  typedef ScoreModel              SM;
  typedef Alpha::CodeT            CodeT;
  typedef typename SM::IntT       IntT;
  typedef typename SM::Transition Transition;
  typedef typename SM::TrType     TrType;
  typedef vector<ScoreT>           V;
  typedef vector<IntT>             VI;
  typedef TriMatrixIndex<IntT>     TMI;
  typedef TensorIndex<IntT, false> Tensor;
  template <bool fwd> class FnInside {
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnInside(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {
      for (IntT s = 0; s < _pm.sm().nstate(); ++s) { _pm.cur_sc(s) = NEG_INF();}
      // _pm.cur_sc(SM::HAIRPIN_END) = 0;
    }
    void after_transition(IntT i, IntT j) {
      for (IntT s = 0; s < _pm.sm().nstate(); ++s) {
	ScoreT sc = _pm.cur_sc(s);
	if (impossible(sc)) { sc = NEG_INF();}
	_pm.inside_s(s, (j - i)) = sc;
	IntT k = _pm.sm().layer_in(s, _h);
	if (k >= 0) { _pm.inside_p(k, i, j) = sc;}
      }
    }
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) {
      const Transition& t  = _pm.sm().transitions(type);
      ScoreT            sc = NEG_INF();
      if (t.bf()) {
	// for (IntT m = k; m <= l; ++m) {
        //   ScoreT sc1 = (_pm.inside_p(t.to(), i, m) + _pm.inside_p(t.to1(), m, j, false));
	//   LOGADD(sc, sc1);
        // }
	IntT m = (l - k + 1);
	if (m > 0) {
	  if (fwd) { 
	    _pm.log_dot_pm(sc, m, &_pm.inside_p(_pm.sm().layer_in(t.to(), _h), i, k),
			   &_pm.inside_s(t.to1(), (j - k)));
	  } else {
	    _pm.log_dot_pp(sc, m, &_pm.inside_s(t.to(), (k - i)),
			   &_pm.inside_p(_pm.sm().layer_in(t.to1(), _h), k, j));
	  }
	}
      } else { // same for both fwd and bwd
	sc = (t.refer_cur_sc() ? _pm.cur_sc(t.to()) 
	      : _pm.inside_p(_pm.sm().layer_in(t.to(), _h), k, l));
	sc += _pm.sm().tsc(type, i, j, k, l);
      }
      LOGADD(_pm.cur_sc(t.from()), sc);
    }
  };
  class FnForward {
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnForward(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {
      if ((j - i) <= _pm.width()) { 
	_pm.cur_sc(SM::INNER_BEG) = _pm.inside_s(SM::INNER_BEG, (j - i));
      } else { 
	_pm.cur_sc(SM::INNER_BEG) = NEG_INF();
      }
      _pm.cur_sc(SM::OUTER)     = NEG_INF();
      // _pm.cur_sc(SM::OUTER_END) = 0;
    }
    void after_transition(IntT i, IntT j) {
      ScoreT sc = _pm.cur_sc(SM::OUTER);
      if (impossible(sc)) { sc = NEG_INF();}
      _pm.inside_o(j) = sc;
    }
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) {
      const Transition& t = _pm.sm().transitions(type);
      ScoreT sc = NEG_INF();
      if (t.bf()) {
	// for (IntT m = k; m <= l; ++m) {
        //   ScoreT sc1 = (_pm.inside_o(m) + _pm.inside_p(t.to1(), m, j, false));
        //   LOGADD(sc, sc1);
        // }
	IntT m = (l - k + 1);
	if (m > 0) { _pm.log_dot_pm(sc, m, &_pm.inside_o(k), &_pm.inside_s(t.to1(), (j - k)));}
      } else {
	sc = (t.refer_cur_sc() ? _pm.cur_sc(t.to()) : _pm.inside_o(l));
	sc += _pm.sm().tsc(type, i, j, k, l);
      }
      LOGADD(_pm.cur_sc(SM::OUTER), sc);
    }
  };
  class FnBackward {// only backward direction is considered
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnBackward(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {
      _pm.cur_sc(SM::OUTER) = NEG_INF();
      // _pm.cur_sc(OUTER_BEG) = 0;
    }
    void after_transition(IntT i, IntT j) {
      ScoreT sc = _pm.cur_sc(SM::OUTER);
      if (impossible(sc)) { sc = NEG_INF();}
      _pm.outside_o(j) = sc;
    }
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) {
      const Transition& t = _pm.sm().transitions(type);
      IntT s = (-1);
      ScoreT sc = NEG_INF();
      if (t.bf()) { //TR_O_BFL
        s = t.to();
        // for (IntT m = k; m <= l; ++m) {
        //   ScoreT sc1 = (_pm.inside_p(t.to1(), j, m) + _pm.outside_o(m));
        //   LOGADD(sc, sc1);
        // }
	IntT m = (l - k + 1);
	if (m > 0) { _pm.log_dot_pp(sc, m, &_pm.inside_s1(t.to1(), (k - j)), &_pm.outside_o(k));}
      } else {
        s   = t.to();
	sc  = (t.refer_cur_sc() ? _pm.cur_sc(t.from()) : _pm.outside_o(l));
        sc += _pm.sm().tsc(type, k, l, i, j);
      }
      LOGADD(_pm.cur_sc(s), sc); 
    }
  };
  template <bool fwd> class FnOutside {
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnOutside(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {
      for (IntT s = 0; s < _pm.sm().nstate(); ++s) { _pm.cur_sc(s) = NEG_INF();}
      if (i == 0) { _pm.cur_sc(SM::OUTER) = _pm.outside_o(j);}	  
    }
    void after_transition(IntT i, IntT j) {
      for (IntT s = 0; s < _pm.sm().nstate(); ++s) {
	ScoreT sc = _pm.cur_sc(s);
	if (impossible(sc)) { sc = NEG_INF();}
	_pm.outside_s(s, (j - i)) = sc;
	IntT k = _pm.sm().layer_out(s, _h);
	if (k >= 0) { _pm.outside_p(k, i, j) = sc;}
      }
    }
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) { 
      const Transition& t  = _pm.sm().transitions(type);
      IntT               s  = (-1);
      ScoreT            sc = NEG_INF();
      if (t.from() == SM::OUTER) {
        if (t.bf()) {
          s  = t.to1();
          sc = (_pm.inside_o(i) + _pm.outside_o(j));
        } else {
          s = t.to();
	  sc = (t.refer_cur_sc() ? _pm.cur_sc(t.from()) : _pm.outside_o(l));
          sc += _pm.sm().tsc(type, k, l, i, j);
        }
      } else {
	IntT m = (l - k + 1);
        switch (t.bf()) {
        case SM::BF_LEFT:
          s = t.to();
          // for (IntT m = k; m <= l; ++m) {
          //   ScoreT sc1 = _pm.sm().tsc((type+n), i, m, i, j);
          //   sc1 += (_pm.outside_p(t.from(), i, m) + _pm.inside_p(t.to1(), j, m));
          //   LOGADD(sc, sc1);
          // }
	  if (m > 0) {
	    if (fwd) {
	      _pm.log_dot_pp(sc, m, &_pm.outside_s(t.from(), (k - i)), 
			     &_pm.inside_p(_pm.sm().layer_in(t.to1(), _h), j, k));
	    } else { // inside.s1 should be maintained
	      _pm.log_dot_pp(sc, m, &_pm.outside_p(_pm.sm().layer_out(t.from(), _h), i, k), 
			     &_pm.inside_s1(t.to1(), (k - j)));
	    }
	  }
	  break;
	case SM::BF_RIGHT:
          s = t.to1();
          // for (IntT m = k; m <= l; ++m) {
          //   ScoreT sc1 = _pm.sm().tsc(type, m, j, i, j);
          //   sc1 += (_pm.inside_p(t.to(), m, i, false) + _pm.outside_p(t.from(), m, j, false));
          //   LOGADD(sc, sc1);
          // }
	  if (m > 0) {
	    if (fwd) { // inside.s1 should be maintained
	      _pm.log_dot_mp(sc, m, &_pm.inside_s1(t.to(), (i - k)),
			     &_pm.outside_p(_pm.sm().layer_out(t.from(), _h), k, j));
	    } else {
	      _pm.log_dot_pm(sc, m, &_pm.inside_p(_pm.sm().layer_in(t.to(), _h), k, i),
			     &_pm.outside_s(t.from(), (j - k)));
	    }
	  }
	  break;
        default:
          s = t.to();
	  sc = (t.refer_cur_sc() ? _pm.cur_sc(t.from()) 
		: _pm.outside_p(_pm.sm().layer_out(t.from(), _h), k, l));
          sc += _pm.sm().tsc(type, k, l, i, j);
	  break;
        }
      }
      LOGADD(_pm.cur_sc(s), sc);      
    }
  };
  class FnProbInside {
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnProbInside(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {}
    void after_transition(IntT i, IntT j) {}
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) {
      const Transition& t = _pm.sm().transitions(type);
      ScoreT sc_out = _pm.cur_sc(t.from()); 
      ScoreT sc_in  = (t.refer_cur_sc() ? _pm.cur_sc(t.to()) 
		       : _pm.inside_p(_pm.sm().layer_in(t.to(), _h), k, l));
      ScoreT dsc    = _pm.sm().tsc(type, i, j, k, l);
      ScoreT w      = (dsc + ((sc_in - _pm.partition_coeff()) + sc_out));
      Assert(w < 1.0e-5, "type:%s,[%d,%d,%d,%d],w:%e,sc_in:%e,sc_out:%e,dsc:%e", 
	     t.name().c_str(), i, j, k, l, w, sc_in, sc_out, dsc);
      _pm.add_prob(type, i, j, k, l, w);
    }
  };
  class FnProbForward {
  public:
    PM& _pm;
    enum { _h = SM::OT_NONE};
    FnProbForward(PM& pm) : _pm(pm) {}
    void before_transition(IntT i, IntT j) {}
    void after_transition(IntT i, IntT j) {}
    void on_transition(TrType type, IntT i, IntT j, IntT k, IntT l) {
      const Transition& t = _pm.sm().transitions(type);
      Assert(!t.refer_cur_sc());
      ScoreT sc_in  = _pm.inside_o(l);
      ScoreT dsc    = _pm.sm().tsc(type, i, j, k, l);
      ScoreT sc_out = _pm.cur_sc(t.from());
      ScoreT w      = (dsc + ((sc_in - _pm.partition_coeff()) + sc_out));
      Assert(w < 1.0e-5, "type:%s,[%d,%d,%d,%d],w:%e,sc_in:%e,sc_out:%e,dsc:%e", 
	     t.name().c_str(), i, j, k, l, w, sc_in, sc_out, dsc);
      _pm.add_prob(type, i, j, k, l, w);
    }
  };
  SM*              _sm;
  IntT             _max_span;
  IntT             _seqlen;
  IntT             _width;
  IntT             _period;
  IntT             _block_len;
  IntT             _acc_lmin;
  VI               _acc_lens;
  VI               _acc_len_id;
  ScoreT           _partition_coeff;
  ScoreT           _dp_score;
  TMI              _tmx_in;
  TMI              _tmx_out;
  TMI              _tmx_prob;
  Tensor           _tsr;
  DpMatrix         _inside;
  DpMatrix         _outside;
  V                _cur_sc;
  V                _probs;
  IntT             _cur_prob_row;
  double           _prob_thr;
  double           _length_factor;
  FnForward        _fn_forward;
  FnInside<true>   _fn_inside_fwd;
  FnInside<false>  _fn_inside_bwd;
  FnBackward       _fn_backward;
  FnOutside<false> _fn_outside;
  FnProbForward    _fn_prob_forward;
  FnProbInside     _fn_prob_inside;
  ProbModel() : _fn_forward(*this), _fn_inside_fwd(*this), _fn_inside_bwd(*this),
		_fn_backward(*this), _fn_outside(*this),
		_fn_prob_forward(*this), _fn_prob_inside(*this) {}
  ScoreModel& sm() { return *_sm;}
  void set_score_model(ScoreModel& z) { _sm = &z;}
  void set_max_span(const IntT& z) { _max_span = z;}
  void set_prob_thr(double z) { _prob_thr = z;}
  void set_length_factor(double z) { _length_factor = z;}
  void set_acc_lens(const VI& acc_lens) {
    _acc_lens = acc_lens;
    vsort(_acc_lens); Check(_acc_lens.size() > 0);
    _acc_lmin = _acc_lens[0];
    _acc_len_id.assign((_acc_lens.back() + 1), (-1));
    for (IntT i = 0; i < (IntT)_acc_lens.size(); ++i) { _acc_len_id[_acc_lens[i]] = i;}
  }
  IntT seqlen() { return _seqlen;}
  IntT width() { return _width;}
  IntT period() { return _period;}
  IntT block_len() { return _block_len;}
  CodeT seq(IntT i) { return _sm->seq(i);}
  template <typename T, typename Iter> void log_dot_pm(T& t, IntT n, Iter i1, Iter i2) 
  { while (n-- > 0) { LOGADD(t, (*i1++) + (*i2--));}}
  template <typename T, typename Iter> void log_dot_pp(T& t, IntT n, Iter i1, Iter i2) 
  { while (n-- > 0) { LOGADD(t, (*i1++) + (*i2++));}}
  template <typename T, typename Iter> void log_dot_mp(T& t, IntT n, Iter i1, Iter i2) 
  { while (n-- > 0) { LOGADD(t, (*i1--) + (*i2++));}}
  template <typename T, typename Iter> void log_dot_mm(T& t, IntT n, Iter i1, Iter i2) 
  { while (n-- > 0) { LOGADD(t, (*i1--) + (*i2--));}}
  ScoreT& inside_p(IntT s, IntT i, IntT j) { return _inside.p[_tmx_in.ref(s, i, j)];}
  ScoreT& inside_s(IntT s, IntT i) { return _inside.s[_tsr.ref(s, i)];}
  ScoreT& inside_s1(IntT s, IntT i) { return _inside.s1[_tsr.ref(s, i)];}
  ScoreT& inside_o(IntT j) { return _inside.o[j];}
  ScoreT& outside_p(IntT s, IntT i, IntT j) { return _outside.p[_tmx_out.ref(s, i, j)];}
  ScoreT& outside_s(IntT s, IntT i) { return _outside.s[_tsr.ref(s, i)];}
  ScoreT& outside_s1(IntT s, IntT i) { return _outside.s1[_tsr.ref(s, i)];}
  ScoreT& outside_o(IntT j) { return _outside.o[j];}
#if false
  ScoreT& prob(IntT i, IntT j) // for test
  { IntT k = _tmx_prob.ref(0, i, j); Check(0 <= k && k < (IntT)_probs.size()); return _probs[k];}
#else
  ScoreT& prob(IntT i, IntT j) { return _probs[_tmx_prob.ref(0, i, j)];}
#endif
  V& cur_sc() { return _cur_sc;}
  ScoreT& cur_sc(IntT s) { return _cur_sc[s];}
  ScoreT& partition_coeff() { return _partition_coeff;}
  bool allow_pair(IntT i, IntT j) { return Alpha::is_canonical(seq(i + 1), seq(j));}
  bool allow_inner_loop(IntT i, IntT j) { return true;}
  bool allow_outer_loop(IntT i, IntT j) { return true;}
#if false
  IntT max_block() { return 100;} // for test
#else
  IntT max_block() { return 10000;}
#endif
  IntT compute_width(IntT max_span, IntT seqlen)
  { return (max_span > 0 ? std::min(max_span, seqlen) : seqlen);}
  IntT compute_period(IntT max_acc, IntT width, IntT seqlen) 
  { return min(seqlen, max(width, max_acc));}
  IntT compute_block_len(IntT max_block, IntT period, IntT width, IntT seqlen) 
  { return min(seqlen, max(period, max_block));}
  void set_seqlen_width_period_block_len() {
    _seqlen    = _sm->seqlen();
    _width     = compute_width(_max_span, _seqlen);
    _period    = compute_period((_acc_lens.empty() ? 0 : _acc_lens.back()), _width, _seqlen);
    _block_len = compute_block_len(max_block(), _period, _width, _seqlen);
  }
  void set_tr_acc_lens() {
    vidx(_acc_lens, (IntT)0, (_width + 1)); // 0.._width
    _acc_lmin = _acc_lens[0];
    _acc_len_id.assign((_acc_lens.back() + 1), (-1)); 
    for (IntT i = 0; i < (IntT)_acc_lens.size(); ++i) { _acc_len_id[_acc_lens[i]] = i;}
  }
  void init_probs(IntT length, IntT period) {
    _tmx_prob.set_size(1, length, period, true);
    _probs.assign(_tmx_prob.size(), 0); // exp value
  }
  void compute_prob_init() {
    set_seqlen_width_period_block_len();
    init_probs(_acc_lens.size(), (_period + 1));
  }
  template <typename Block> void compute_prob(Block block) {
    compute_prob_init();
    compute_forward_backward(proc_l(&Self::template 
				    prob_forward_transitions<FnProbForward>, 
				    *this, _fn_prob_forward),
			     proc_l(&Self::template 
				    prob_inside_transitions<FnProbInside>, 
				    *this, _fn_prob_inside),
			     proc_l(&Self::template 
				    print_prob<Block>, *this, block));
  }
  template <typename Block1, typename Block2, typename Block3>
  void compute_forward_backward(Block1 forward_transitions1, Block2 inside_transitions1,
				Block3 print_row) {
    compute_forward();
    Check(possible(_dp_score), "could not parse sequence");
    compute_backward(forward_transitions1, inside_transitions1, print_row);
  }
  void compute_forward() {
    _cur_sc.assign(_sm->nstate0(), 0);
    _tmx_in.set_size(_sm->nlayer_in(), (_width + 1), (_width + 1), true);
    _tsr.set_size(_sm->nstate(), (_width + 1));
    _inside.set_size(_tmx_in.size(), _tsr.size(), _tsr.size(), (_seqlen + 1));
    _inside.clean();
    for (IntT b = 0; b <= _seqlen; b += (_block_len + 1)) {
      IntT e  = min(b + _block_len, _seqlen);
      IntT b0 = max((IntT)0, (b - _width));
      IntT e0 = e;
      _tmx_in.set_range(b0, (e0 + 1));
      for (IntT j = b; j <= e; ++j) {
	IntT ib = max((IntT)0, (j - _width));
	for (IntT i = j; i >= ib; --i) { inside_transitions(_fn_inside_fwd, i, j);}
	forward_transitions(_fn_forward, 0, j);
      }
    }
    _partition_coeff = compute_partition_coeff();
    _dp_score        = _partition_coeff;
  }
  template <typename Block1, typename Block2, typename Block3>
  void compute_backward(Block1 forward_transitions1, Block2 inside_transitions1,
		        Block3 print_row) {
    _tmx_in.flip_order_backward(_inside.p, _seqlen);
    _tmx_in.flip_order();
    vfill(_inside.s.begin(), _inside.s.end(), NEG_INF());
    _tmx_out.set_size(_sm->nlayer_out(), (_width + 1), (_width + 1), true);
    _outside.set_size(_tmx_out.size(), _tsr.size(), 0, (_seqlen + 1));
    _outside.clean();
    for (IntT e = (_seqlen - _width); e >= (0 - _width); e -= (_block_len + 1)) {
      IntT b  = max((0 - _width), (e - _block_len));
      IntT b0 = min(b, (b + _width - _period));
      IntT e0 = min((e + 2 * _width), _seqlen);
      _tmx_in.set_range(b0, (e0 + 1));
      _tmx_out.set_range(b0, (e0 + 1));
      _tmx_prob.set_range(b0, (e0 + 1));
      for (IntT i = e; i >= b; --i) {
	IntT k = (i + _width);
	Check(0 <= k && k <= _seqlen);
	if (0 <= i) { for (IntT j = i; j <= k; ++j) { inside_transitions(_fn_inside_bwd, i, j);}}
	IntT je = min((k + _width), _seqlen);
	for (IntT j = k; j <= je; ++j) { set_inside_s1_for_outside(k, j);}
	backward_transitions(_fn_backward, 0, k);
	forward_transitions1(0, k);
	IntT ib = max((IntT)0, i);
	for (IntT i1 = ib; i1 <= k; ++i1) {
	  outside_transitions(_fn_outside, i1, k);
	  inside_transitions1(i1, k);
	}
	print_row(k);
      }
    }
  }
  ScoreT compute_partition_coeff() { return _inside.o[_seqlen];}
  void set_inside_s1_for_outside(IntT i, IntT j) {
    for (IntT s = 0; s < _sm->nstate(); ++s) {
      IntT t = _sm->layer_in(s, SM::OT_NONE);
      if (t >= 0) { inside_s1(s, (j - i)) = inside_p(t, i, j);} 
      else { inside_s1(s, (j - i)) = NEG_INF();}
    }
  }
  template <typename Block> void print_prob(Block& block, IntT i) {
    for (IntT u = 0; u < (IntT)_acc_lens.size(); ++u) {
      IntT w = _acc_lens[u];
      if (_seqlen < (i + w)) break;
      double& p0 = prob(i, i + u);
      double  p  = p0; // have local copy
      p0 = 0;  // clean up
      if (p < _prob_thr) continue;
      block(i, w, _sm->score_to_energy(log(p))); // zero based seq coord
    }
  }
  void add_prob(TrType t, IntT i, IntT j, IntT k, IntT l, ScoreT w) {
    w = exp(w);
    switch (t) {
      //case SM::TR_S_S:break;
      //case SM::TR_S_E:break;
      // case SM::TR_M_BF: case TR_M_BFL: case TR_M_BFR:break;
      //case SM::TR_M2_S:break;
    case SM::TR_M2_M2: prob(l, l + _acc_len_id[j - l]) += w; break;
      // case SM::TR_M1_M2:break;
      // case SM::TR_M1_MBF:break;
    case SM::TR_M_M: prob(i, i + _acc_len_id[k - i]) += w; break;
      // case SM::TR_M_MBF:break;
    case SM::TR_E_H: // hairpin is left emitted
      for (IntT b = i; b <= (k - _acc_lmin); ++b) {
	for (IntT u = 0; u < (IntT)_acc_lens.size() && ((b + _acc_lens[u]) <= k); ++u) { 
	  prob(b, b + u) += w;
	}
      }
      break;
    case SM::TR_E_I:
      for (IntT b = i; b <= (k - _acc_lmin); ++b) {
	for (IntT u = 0; u < (IntT)_acc_lens.size() && ((b + _acc_lens[u]) <= k); ++u) {
	  prob(b, b + u) += w;
	}
      }
      for (IntT b = l; b <= (j - _acc_lmin); ++b) {
	for (IntT u = 0; u < (IntT)_acc_lens.size() && ((b + _acc_lens[u]) <= j); ++u) { 
	  prob(b, b + u) += w;
	}
      }
      break;
      // case SM::TR_E_M:break;
      // case SM::TR_IB_S:break;
      // case SM::TR_O_X:break;
      // case SM::TR_X_O:break;
    case SM::TR_O_O: prob(l, l + _acc_len_id[j - l]) += w; break; 
      // case SM::TR_O_IB:break;
      // case SM::TR_O_BF: case SM::TR_O_BFL: case SM::TR_O_BFR:break;
    default: Die("bad type %d", t); break;
    }
  }
  template <typename Fn> void forward_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    //Outer->
    if (i == j) {
      //->OuterEnd
      f.on_transition(SM::TR_O_X, i, j, i, j);
    } else {//if i < j
      //->Outer
      if (allow_outer_loop(j - 1, j)) { f.on_transition(SM::TR_O_O, i, j, i, j - 1);}
      //->InnerBeg
      if (j <= (i + _width)) { f.on_transition(SM::TR_O_IB, i, j, i, j);}
      //OuterBF -> (Outer + InnerBeg) ( (i + 1) because i is included in TR_O_IB)
      f.on_transition(SM::TR_O_BF, i, j, max((i + 1), (j - _width)), (j - 1)); 
    }
    f.after_transition(i, j);
  }
  template <typename Fn> void backward_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    //->Outer
    if (j == _seqlen) {
      //OuterBeg->
      f.on_transition(SM::TR_X_O, i, j, i, j);
    } else {//if j < _seqlen
      if (allow_outer_loop(j, j + 1)) { f.on_transition(SM::TR_O_O, i, j, i, j + 1);}
      //OuterBFL->
      f.on_transition(SM::TR_O_BFL, i, j, (j + 1), min((j + _width), _seqlen));
    }
    f.after_transition(i, j);
  }
  template <typename Fn> void inside_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    //Stem->
    IntT k = (i + 1);
    IntT l = (j - 1);
    if (k <= l) {
      if (allow_pair(i, j)) {
	//->Stem
	if (k + 2 <= l) { if (allow_pair(k, l)) { f.on_transition(SM::TR_S_S, i, j, k, l);}}
	//->StemEnd
	f.on_transition(SM::TR_S_E, i, j, k, l);
      }
    }
    //Multi->
    if (0 < i && j < _seqlen) {
      // MultiBF->(Multi1 + Multi2)
      f.on_transition(SM::TR_M_BF, i, j, (i + 1), (j - 1));
      //Multi2->
      //->Stem
      if ((i + 2 <= j) && allow_pair(i, j)) { f.on_transition(SM::TR_M2_S, i, j, i, j);}
      //->Multi2
      if (i <= (j - 1)) {
	if (allow_inner_loop(j - 1, j)) { f.on_transition(SM::TR_M2_M2, i, j, i, j - 1);}
      }
      //Multi1->
      //->Multi2
      f.on_transition(SM::TR_M1_M2, i, j, i, j);
      //->MultiBF
      f.on_transition(SM::TR_M1_MBF, i, j, i, j);
      //Multi->
      //->Multi
      if ((i + 1) <= j) { 
	if (allow_inner_loop(i, i + 1)) { f.on_transition(SM::TR_M_M, i, j, i + 1, j);}
      }
      //->MultiBF
      f.on_transition(SM::TR_M_MBF, i, j, i, j);
    }
    //StemEnd->
    if (0 < i && j < _seqlen) {
      if (allow_pair(i - 1, j + 1)) {
	//->Hairpin
	if ((i + SM::MINHPIN <= j) && allow_inner_loop(i, j)) { 
	  f.on_transition(SM::TR_E_H, i, j, j, j);
	}
	//->Interior Loop (includes Bulge)->Stem
	for (IntT li = 0; li <= min((j - i), (IntT)SM::MAXLOOP); ++li) {
	  IntT ip = (i + li);
	  if (allow_inner_loop(i, ip)) {
	    IntT ljb = max((IntT)0, 1 - li);
	    IntT lje = min((j - (ip + 2)), (SM::MAXLOOP - li));
	    for (IntT lj = ljb; lj <= lje; ++lj) { 
	      // (li + lj) >= 1
	      IntT jp = (j - lj);
	      if (allow_inner_loop(jp, j)) {
		if (allow_pair(ip, jp)) { f.on_transition(SM::TR_E_I, i, j, ip, jp);}
	      }
	    }
	  }
	}
	//->Multi
	f.on_transition(SM::TR_E_M, i, j, i, j);
      }
    }
    //InnerBeg->
    //->Stem
    if ((k <= l) && allow_pair(i, j)) { f.on_transition(SM::TR_IB_S, i, j, i, j);}
    f.after_transition(i, j);
  }
  template <typename Fn> void outside_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    //->InnerBeg
    if (i == 0) {
      //Outer->
      f.on_transition(SM::TR_O_IB, i, j, i, j);
    } else if (i < j) {
      //Outer->(Outer + .)
      f.on_transition(SM::TR_O_BFR, i, j, 0, 0);
    }
    //->StemEnd
    IntT k = (i - 1);
    IntT l = (j + 1);
    if (0 <= k && l <= _seqlen && (l - k) <= _width) {
      if (allow_pair(k, l)) {
	//Stem->
	f.on_transition(SM::TR_S_E, i, j, k, l);
      }
    }
    IntT i0 = max((IntT)0, (j - _width));
    IntT j0 = min((i + _width), _seqlen);
    //->Multi
    //StemEnd->
    if (0 < i && j < _seqlen) {
      if (allow_pair(k, l)) { f.on_transition(SM::TR_E_M, i, j, i, j);}
      //Multi->
      if (max((IntT)0, (j - _width)) < i) {
	if (allow_inner_loop(i - 1, i)) { f.on_transition(SM::TR_M_M, i, j, (i - 1), j);}
      }
      //->Multi1
      //MultiBF->(. + Multi2)
      f.on_transition(SM::TR_M_BFL, i, j, (j + 1), (j0 - 1)); // (-1) for STEM state
      //->Multi2
      //Multi2->
      if (j < j0) {
	if (allow_inner_loop(j, (j + 1))) { f.on_transition(SM::TR_M2_M2, i, j, i, (j + 1));}
      }
      //Multi1->
      f.on_transition(SM::TR_M1_M2, i, j, i, j);
      //MultiBF->(Multi1 + .)
      f.on_transition(SM::TR_M_BFR, i, j, (i0 + 1), (i - 1)); // (+1) for STEM state
      //->MultiBF
      //Multi1->
      f.on_transition(SM::TR_M1_MBF, i, j, i, j);
      //Multi->
      f.on_transition(SM::TR_M_MBF, i, j, i, j);
    }
    //->Stem
    if ((i + 2 <= j) && allow_pair(i, j)) {
      //->InnerBeg
      f.on_transition(SM::TR_IB_S, i, j, i, j);
      //Stem->Interior->
      // IntT lie = min((i - (max(0, (j - _width) + 1))), (IntT)SM::MAXLOOP);
      IntT lie = min((i - i0 - 1), (IntT)SM::MAXLOOP); // (-1) for STEM state
      for (IntT li = 0; li <= lie; ++li) {
	IntT ip = (i - li);
	if (allow_inner_loop(ip, i)) {
	  IntT jp0 = min((ip + _width), _seqlen);
	  IntT ljb = max((IntT)0, (1 - li));
	  // int lje = min((min(_seqlen, ((ip - 1) + _width)) - 1 - j), (SM::MAXLOOP - li));
	  IntT lje = min((jp0 - j - 1), ((IntT)SM::MAXLOOP - li)); // (-1) for STEM state
	  for (IntT lj = ljb; lj <= lje; ++lj) {
	    // (li+lj) > 0
	    IntT jp = (j + lj);
	    if (allow_inner_loop(j, jp)) {
	      if (allow_pair(ip - 1, jp + 1)) { f.on_transition(SM::TR_E_I, i, j, ip, jp);}
	    }
	  }
	}
      }
      if (0 < i && j < _seqlen) {
	//Multi2->
	f.on_transition(SM::TR_M2_S, i, j, i, j);
      }
      IntT k = (i - 1);
      IntT l = (j + 1);
      if (0 <= k && l <= _seqlen && (l - k) <= _width) {
	//->STEM
	if (allow_pair(k, l)) { f.on_transition(SM::TR_S_S, i, j, k, l);}
      }
    }
    f.after_transition(i, j);
  }
  template <typename Fn> void prob_forward_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    Assert(i == 0);
    //Outer->
    //->Outer
    for (IntT u = 0; u < (IntT)_acc_lens.size(); ++u) {
      IntT j1 = (j - _acc_lens[u]);
      if (j1 < i) break;
      if (allow_outer_loop(j1, j)) { f.on_transition(SM::TR_O_O, i, j, i, j1);}
    }
    f.after_transition(i, j);
  }
  template <typename Fn> void prob_inside_transitions(Fn& f, IntT i, IntT j) {
    f.before_transition(i, j);
    if ((i + _acc_lmin <= j) && 0 < i && j < _seqlen) {
      //Multi2->
      //->Multi2
      for (IntT u = 0; u < (IntT)_acc_lens.size(); ++u) {
	IntT j1 = (j - _acc_lens[u]);
	if (j1 < i) break;
	if (allow_inner_loop(j1, j)) { f.on_transition(SM::TR_M2_M2, i, j, i, j1);}
      }
      //Multi->
      //->Multi
      for (IntT u = 0; u < (IntT)_acc_lens.size(); ++u) {
	IntT i1 = (i + _acc_lens[u]);
	if (j < i1) break;
	if (allow_inner_loop(i, i1)) { f.on_transition(SM::TR_M_M, i, j, i1, j);}
      }
      //StemEnd->
      if (allow_pair(i - 1, j + 1)) {
	//->Hairpin       # (i, j, k, l) = (i, j, j, j) left emission
	if ((i + SM::MINHPIN <= j) && allow_inner_loop(i, j)) { 
	  f.on_transition(SM::TR_E_H, i, j, j, j);
	}
	//->Interior Loop (includes Bulge)->Stem
	IntT lie = min((j - i), (IntT)SM::MAXLOOP);
	for (IntT li = 0; li <= lie; ++li) {
	  IntT ip = (i + li);
	  if (allow_inner_loop(i, ip)) {
	    IntT ljb = max((IntT)0, 1 - li);
	    IntT lje = min((j - (ip + 2)), (SM::MAXLOOP - li));
	    for (IntT lj = ljb; lj <= lje; ++lj) { 
	      // (li + lj) >= 1
	      IntT jp = (j - lj);
	      if ((li >= _acc_lmin || lj >= _acc_lmin) && allow_inner_loop(jp, j)) {
		if (allow_pair(ip, jp)) { f.on_transition(SM::TR_E_I, i, j, ip, jp);}
	      }
	    }
	  }
	}
      }
    }
    f.after_transition(i, j);
  }
  void null_transitions(IntT i, IntT j) {}
};
#endif
