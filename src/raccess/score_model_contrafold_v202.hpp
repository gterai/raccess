/*
 * author: hisanori kiryu
 */
#ifndef RACCESS__SCORE_MODEL_CONTRAFOLD_V202_HPP
#define RACCESS__SCORE_MODEL_CONTRAFOLD_V202_HPP
#include "util/util.hpp"
#include "util/tensor.hpp"
#include "raccess/score.hpp"
#include "raccess/score_model_contrafold_v202_default_params.hpp"
namespace Raccess { class ScoreModelContrafoldV202;}
class Raccess::ScoreModelContrafoldV202 {
// #define CHECK_CANONICAL
public:
  typedef ScoreModelContrafoldV202 Self;
  typedef long                 IntT;
  static ScoreT GAS_CONST_KCAL_MOL_K() { return (1.9872 * 0.001);}
  static ScoreT TEMPERATURE_K0() { return 273.15;}
  static ScoreT TEMPERATURE_K() { return (TEMPERATURE_K0() + 37.0);}
  static ScoreT RT_KCAL_MOL(ScoreT temperature_k) { return GAS_CONST_KCAL_MOL_K() * temperature_k;}
  static ScoreT RT_KCAL_MOL() { return GAS_CONST_KCAL_MOL_K() * TEMPERATURE_K();} // =0.61633008
  typedef Alpha::CodeT CodeT;
  typedef Alpha::Codes Seq;
  typedef vector<IntT> VI;
  // contrafold version 2.02 default model.
  // const int C_MIN_HAIRPIN_LENGTH = 0;
  // const int C_MAX_SINGLE_LENGTH = 30;
  // const int D_MAX_HAIRPIN_LENGTH = 30;
  // const int D_MAX_BP_DIST_THRESHOLDS = 10;
  // const int D_MAX_BULGE_LENGTH = 30;
  // const int D_MAX_INTERNAL_LENGTH = 30;
  // const int D_MAX_INTERNAL_SYMMETRIC_LENGTH = 15;
  // const int D_MAX_INTERNAL_ASYMMETRY = 28;
  // const int D_MAX_INTERNAL_EXPLICIT_LENGTH = 4;
  // const int D_MAX_HELIX_LENGTH = 30;
  // const int BP_DIST_LAST_THRESHOLD = 132;
  // const int BP_DIST_THRESHOLDS[D_MAX_BP_DIST_THRESHOLDS] = 
  //   { 3, 9, 12, 16, 21, 26, 34, 47, 71, BP_DIST_LAST_THRESHOLD };
  // const std::string alphabet = "ACGU";    // allowed symbols -- all other letters ignored
  // const int M = 4;                        // number of alphabet symbols
  // const int MAX_DIMENSIONS = 4;
  // #define PARAMS_BASE_PAIR                           1
  // #define PARAMS_BASE_PAIR_DIST                      0
  // #define PARAMS_TERMINAL_MISMATCH                   1
  // #define PARAMS_HAIRPIN_LENGTH                      1
  // #define PARAMS_HAIRPIN_3_NUCLEOTIDES               0
  // #define PARAMS_HAIRPIN_4_NUCLEOTIDES               0
  // #define PARAMS_HELIX_LENGTH                        0
  // #define PARAMS_ISOLATED_BASE_PAIR                  0
  // #define PARAMS_INTERNAL_EXPLICIT                   1
  // #define PARAMS_BULGE_LENGTH                        1
  // #define PARAMS_INTERNAL_LENGTH                     1
  // #define PARAMS_INTERNAL_SYMMETRY                   1
  // #define PARAMS_INTERNAL_ASYMMETRY                  1
  // #define PARAMS_BULGE_0x1_NUCLEOTIDES               1
  // #define PARAMS_BULGE_0x2_NUCLEOTIDES               0
  // #define PARAMS_BULGE_0x3_NUCLEOTIDES               0
  // #define PARAMS_INTERNAL_1x1_NUCLEOTIDES            1
  // #define PARAMS_INTERNAL_1x2_NUCLEOTIDES            0
  // #define PARAMS_INTERNAL_2x2_NUCLEOTIDES            0
  // #define PARAMS_HELIX_STACKING                      1
  // #define PARAMS_HELIX_CLOSING                       1
  // #define PARAMS_MULTI_LENGTH                        1
  // #define PARAMS_DANGLE                              1
  // #define PARAMS_EXTERNAL_LENGTH                     1
  enum {
    NNCODE  = Alpha::NNCODE,
    NNCODEN = Alpha::NNCODEN,
    NSCODEN = Alpha::NSCODEN,
    NPCODEN = Alpha::NPCODEN,
    MAXLOOP = 30, // maximal size of interior loop
    MINHPIN = 0
  };
  enum BfType {
    BF_NONE  ,
    BF_PARENT,
    BF_LEFT  ,
    BF_RIGHT
  };
  enum IdxType {
    IT_NONE  ,
    IT_ALPHA ,
    IT_LENGTH
  };
  enum AlType {
    AT_NONE ,
    AT_NCODE,
    AT_PCODE,
    AT_SCODE
  };
  class State {
  public:
    attr(char  , type     );
    attr(char  , layer_in );
    attr(char  , layer_out);
    attr(bool  , sc_dep_outer);
    attr(bool  , sc_dep_inner);
    attr(string, name     );
  };
#if true
  // todo: check sc_dep
#define STATES_()                        \
  ST_(STEM       ,  0,  0, false, true ) \
  ST_(STEM_END   ,  1,  1, true , false) \
  ST_(MULTI      ,  2,  2, true , true ) \
  ST_(MULTI1     ,  3, -1, true , true ) \
  ST_(MULTI2     ,  4,  3, true , true ) \
  ST_(MULTI_BF   , -1,  4, true , true ) \
  ST_(INNER_BEG  ,  5, -1, true , true ) \
  ST_(OUTER      , -1, -1, true , true ) \
  ST_(HAIRPIN_END, -1, -1, false, false) \
  ST_(OUTER_BEG  , -1, -1, false, false) \
  ST_(OUTER_END  , -1, -1, false, false)

#define ST_(type, layer_in, layer_out, sc_dep_outer, sc_dep_inner) type,
  enum StType { STATES_() STATE_X = (-1)};
#undef ST_
  enum {
    NSTATE     = (INNER_BEG - STEM + 1), // WARN: not including OUTER
    NSTATE1    = (OUTER     - STEM + 1),
    NSTATE0    = (OUTER_END - STEM + 1),
    NSTATE_IN  = 6,
    NSTATE_OUT = 5
  };
#else
#define STATES_()                        \
  ST_(STEM       ,  0,  0, false, true ) \
  ST_(STEM_END   ,  1,  1, true , false) \
  ST_(MULTI      ,  2,  2, true , true ) \
  ST_(MULTI1     ,  3,  3, true , true ) \
  ST_(MULTI2     ,  4,  4, true , true ) \
  ST_(MULTI_BF   ,  5,  5, true , true ) \
  ST_(INNER_BEG  ,  6,  6, true , true ) \
  ST_(OUTER      , -1, -1, true , true ) \
  ST_(HAIRPIN_END, -1, -1, false, false) \
  ST_(OUTER_BEG  , -1, -1, false, false) \
  ST_(OUTER_END  , -1, -1, false, false)

#define ST_(type, layer_in, layer_out, sc_dep_outer, sc_dep_inner) type,
  enum StType { STATES_() STATE_X = (-1)};
#undef ST_
  enum {
    NSTATE     = (INNER_BEG - STEM + 1), // WARN: not including OUTER
    NSTATE1    = (OUTER     - STEM + 1),
    NSTATE0    = (OUTER_END - STEM + 1),
    NSTATE_IN  = 7,
    NSTATE_OUT = 7
  };
#endif
  // #define ST_(type, layer_in, layer_out) 
  //  {type, layer_in, layer_out, #type},
  // static const State& states(int type) { static const State a[] = {STATES_()}; return a[type];}
  // #undef ST_
  static const State STATES[NSTATE0];
  static const State& states(IntT type) { return STATES[type];}
  IntT nstate() { return NSTATE;} // WARN: not including OUTER
  IntT nstate1() { return NSTATE1;}
  IntT nstate0() { return NSTATE0;}

  enum OpType {
    OT_NONE
  };
  enum {
    NOP_TYPE        = 1
  };
  static IntT nop_type() { return NOP_TYPE;}

  static IntT layer_in(IntT s, IntT h) { 
    const IntT& l = states(s).layer_in(); 
    if (l == (-1)) return (-1);
    return (l * NOP_TYPE + h);
  }
  static IntT layer_out(IntT s, IntT h)  { 
    const IntT& l = states(s).layer_out(); 
    if (l == (-1)) return (-1);
    return (l * NOP_TYPE + h);
  }
  static IntT nlayer_in() { return (NSTATE_IN * NOP_TYPE);}
  static IntT nlayer_out() { return (NSTATE_OUT * NOP_TYPE);}

  class Transition {
  public:
    attr(char  , type        );
    attr(char  , bf          );
    attr(char  , from        );
    attr(char  , to          );
    attr(char  , to1         );
    attr(bool  , refer_cur_sc);
    attr(bool  , tsc_dep_nuc );
    attr(bool  , emits_nuc   );
    attr(string, name        );
    bool emits_pair() const { return (_from == STEM);}
  };
#define TRANSITIONS_()                                                               \
  TR_(TR_S_S   ,  BF_NONE  , STEM     , STEM       , STATE_X  , false, true , true ) \
  TR_(TR_S_E   ,  BF_NONE  , STEM     , STEM_END   , STATE_X  , false, true , true ) \
  TR_(TR_M_BF  ,  BF_PARENT, MULTI_BF , MULTI1     , MULTI2   , false, false, false) \
  TR_(TR_M_BFL ,  BF_LEFT  , MULTI_BF , MULTI1     , MULTI2   , false, false, false) \
  TR_(TR_M_BFR ,  BF_RIGHT , MULTI_BF , MULTI1     , MULTI2   , false, false, false) \
  TR_(TR_M2_S  ,  BF_NONE  , MULTI2   , STEM       , STATE_X  , true , true , false) \
  TR_(TR_M2_M2 ,  BF_NONE  , MULTI2   , MULTI2     , STATE_X  , false, false, true ) \
  TR_(TR_M1_M2 ,  BF_NONE  , MULTI1   , MULTI2     , STATE_X  , true , false, false) \
  TR_(TR_M1_MBF,  BF_NONE  , MULTI1   , MULTI_BF   , STATE_X  , true , false, false) \
  TR_(TR_M_M   ,  BF_NONE  , MULTI    , MULTI      , STATE_X  , false, false, true ) \
  TR_(TR_M_MBF ,  BF_NONE  , MULTI    , MULTI_BF   , STATE_X  , true , false, false) \
  TR_(TR_E_H   ,  BF_NONE  , STEM_END , HAIRPIN_END, STATE_X  , true , true , true ) \
  TR_(TR_E_I   ,  BF_NONE  , STEM_END , STEM       , STATE_X  , false, true , true ) \
  TR_(TR_E_M   ,  BF_NONE  , STEM_END , MULTI      , STATE_X  , true , true , false) \
  TR_(TR_IB_S  ,  BF_NONE  , INNER_BEG, STEM       , STATE_X  , true , true , false) \
  TR_(TR_O_X   ,  BF_NONE  , OUTER    , OUTER_END  , STATE_X  , true , false, false) \
  TR_(TR_X_O   ,  BF_NONE  , OUTER_BEG, OUTER      , STATE_X  , true , false, false) \
  TR_(TR_O_O   ,  BF_NONE  , OUTER    , OUTER      , STATE_X  , false, false, true ) \
  TR_(TR_O_IB  ,  BF_NONE  , OUTER    , INNER_BEG  , STATE_X  , true , false, false) \
  TR_(TR_O_BF  ,  BF_PARENT, OUTER    , OUTER      , INNER_BEG, false, false, false) \
  TR_(TR_O_BFL ,  BF_LEFT  , OUTER    , OUTER      , INNER_BEG, false, false, false) \
  TR_(TR_O_BFR ,  BF_RIGHT , OUTER    , OUTER      , INNER_BEG, false, false, false)
								     
#define TR_(type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, emits_nuc) type,   
  enum TrType { TRANSITIONS_() TR_TYPE_NULL = (-1)};		     
#undef TR_							     
  enum { NTRANSITION = (TR_O_BFR - TR_S_S + 1)};		     
  // #define TR_(type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc) 
  //     { type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, #type },	     
  //   static const Transition& transitions(int type) 			     
  //  {static const Transition a[] = {TRANSITIONS_()}; return a[type];}  
  // #undef TR_	
  static const Transition TRANSITIONS[NTRANSITION];
  static const Transition& transitions(IntT type) { return TRANSITIONS[type];}	

  class ParamGroup {
  public:
    enum { MAX_RANK = 4};
    typedef CArray<IntT , MAX_RANK> Lengths;
    typedef CArray<char, MAX_RANK> AlTypes;
    attr(char   , type     );
    attr(char   , idx_type );
    attr(Lengths, lengths  );
    attr(AlTypes, al_types );
    attr(char   , pg_pair  );
    attr(bool   , symmetric);
    attr(IntT    , base_idx );
    attr(IntT    , sum_bound);
    attr(string , name     );
    char al_types(IntT k) const { return _al_types[k];}
    IntT rank() const {
      for (IntT k = 0; k < (IntT)_lengths.size(); ++k) { if (_lengths[k] < 0) return k;}
      return _lengths.size();
    }
  };
#define PARAM_GROUPS_()                      \
  PG_(base_pair, 			     \
      IT_ALPHA, 			     \
      NNCODEN,NNCODEN,(-1),(-1),	     \
      AT_NCODE,AT_NCODE,(-1),(-1),	     \
      param_group_null,			     \
      true,(-1),(-1))			     \
  PG_(rna_rna_init        ,                  \
      IT_NONE             ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      param_group_null    ,                  \
      false,(-1),(-1)     )                  \
  PG_(stack_rna_rna       ,                  \
      IT_ALPHA            ,                  \
      NSCODEN,NSCODEN,(-1),(-1),             \
      AT_SCODE,AT_SCODE,(-1),(-1),           \
      param_group_null    ,                  \
      true,(-1),(-1)      )                  \
  PG_(stack_rna_dna       ,                  \
      IT_ALPHA            ,                  \
      NSCODEN,NSCODEN,(-1),(-1),             \
      AT_SCODE,AT_SCODE,(-1),(-1),           \
      param_group_null    ,                  \
      false,(-1),(-1)     )                  \
  PG_(rna_dna_init        ,                  \
      IT_NONE             ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      param_group_null    ,                  \
      false,(-1),(-1)     )                  \
    PG_(terminal_mismatch,		     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,NNCODEN,NNCODEN,     \
	AT_NCODE,AT_NCODE,AT_NCODE,AT_NCODE, \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(hairpin_length_at_least,	     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	cache_hairpin_length,		     \
	false,(-1),(-1))		     \
    PG_(internal_explicit,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(MAXLOOP+1),(-1),(-1),   \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	true, (-1),MAXLOOP)		     \
    PG_(bulge_length_at_least,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	cache_bulge_length,		     \
	false,(-1),(-1))		     \
    PG_(internal_length_at_least,	     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	cache_internal_length,		     \
	false,(-1),(-1))		     \
    PG_(internal_symmetric_length_at_least,  \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	cache_internal_symmetric_length,     \
	false,(-1),(-1))		     \
    PG_(internal_asymmetry_at_least,	     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	cache_internal_asymmetry,	     \
	false,(-1),(-1))		     \
    PG_(bulge_0x1_nucleotides,		     \
	IT_ALPHA,			     \
	NNCODEN,(-1),(-1),(-1),		     \
	AT_NCODE,(-1),(-1),(-1),	     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(internal_1x1_nucleotides,	     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,(-1),(-1),	     \
	AT_NCODE,AT_NCODE,(-1),(-1),	     \
	param_group_null,		     \
	true,(-1),(-1))			     \
    PG_(helix_stacking,			     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,NNCODEN,NNCODEN,     \
	AT_NCODE,AT_NCODE,AT_NCODE,AT_NCODE, \
	param_group_null,		     \
	true,(-1),(-1))			     \
    PG_(helix_closing,			     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,(-1),(-1),	     \
	AT_NCODE,AT_NCODE,(-1),(-1),	     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(multi_base,			     \
	IT_NONE,			     \
	(-1),(-1),(-1),(-1),		     \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(multi_unpaired,			     \
	IT_NONE,			     \
	(-1),(-1),(-1),(-1),		     \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(multi_paired,			     \
	IT_NONE,			     \
	(-1),(-1),(-1),(-1),		     \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(dangle_left,			     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,NNCODEN,(-1),	     \
	AT_NCODE,AT_NCODE,AT_NCODE,(-1),     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(dangle_right,			     \
	IT_ALPHA,			     \
	NNCODEN,NNCODEN,NNCODEN,(-1),	     \
	AT_NCODE,AT_NCODE,AT_NCODE,(-1),     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(external_unpaired,		     \
	IT_NONE,			     \
	(-1),(-1),(-1),(-1),		     \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(external_paired,		     \
	IT_NONE,			     \
	(-1),(-1),(-1),(-1),		     \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,		     \
	false,(-1),(-1))		     \
    PG_(external_paired_length_factor,       \
      IT_NONE             ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      (-1),(-1),(-1),(-1) ,                  \
      param_group_null    ,                  \
      false,(-1),(-1)     )                  \
    PG_(cache_hairpin_length,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	hairpin_length_at_least,	     \
	false,(-1),(-1))		     \
    PG_(cache_bulge_length,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	bulge_length_at_least,		     \
	false,(-1),(-1))		     \
    PG_(cache_internal_length,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	internal_length_at_least,	     \
	false,(-1),(-1))		     \
    PG_(cache_internal_symmetric_length,     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	internal_symmetric_length_at_least,  \
	false,(-1),(-1))		     \
    PG_(cache_internal_asymmetry,	     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(-1),(-1),(-1),	     \
	(-1),(-1),(-1),(-1),		     \
	internal_asymmetry_at_least,	     \
	false,(-1),(-1))		     \
    PG_(cache_single_length,		     \
	IT_LENGTH,			     \
	(MAXLOOP+1),(MAXLOOP+1),(-1),(-1),   \
	(-1),(-1),(-1),(-1),		     \
	param_group_null,                    \
	false,(-1),(-1))
	

#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3,pg_pair,symmetric, base_idx, sum_bound) type,
  enum PgType { PARAM_GROUPS_() param_group_null = (-1)};
#undef PG_
  enum {
    NPARAMGROUP  = (external_paired_length_factor - base_pair + 1),
    NPARAMGROUP1 = (cache_internal_asymmetry - base_pair + 1),
    NPARAMGROUP0 = (cache_single_length      - base_pair + 1)
  };
  // #define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound)
  //    {type,idx_type,{{l0,l1,l2,l3}},{{a0,a1,a2,a3}},pg_pair,symmetric,base_idx,sum_bound,#type},
  //  static const ParamGroup& param_groups(IntT t) 
  // { static const ParamGroup a[] = {PARAM_GROUPS_()}; return a[t];}
  // #undef PG_
  static const ParamGroup PARAM_GROUPS[NPARAMGROUP0];
  static const ParamGroup& param_group(IntT type) { return PARAM_GROUPS[type];}
  static const ParamGroup& param_group(const string& name) {
    for (IntT i = 0; i < NPARAMGROUP1; ++i) {
      const ParamGroup& pg = PARAM_GROUPS[i]; Check(pg.type() == i);
      if (pg.name() == name) { return pg;}
    }
    Die("no param_group for ", name);
    return PARAM_GROUPS[0];
  }
  static IntT nparam_group() { return NPARAMGROUP;}
  static IntT nparam_group1() { return NPARAMGROUP1;}
  static IntT nparam_group0() { return NPARAMGROUP0;}
  typedef TensorBase<ScoreT, int> TensorT;
#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) \
  CTensor<ScoreT, l0,l1,l2,l3> _##type;                              \
  CTensor<ScoreT, l0,l1,l2,l3> const& tsr_##type() const {return _##type;} \
  CTensor<ScoreT, l0,l1,l2,l3>& tsr_##type() {return _##type;}
  class Params { public: PARAM_GROUPS_(); attr(ScoreT, temperature_k);};
#undef PG_
#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) \
  case type: return params.tsr_##type();
  static TensorT& tensor(Params& params, IntT type) {
    static CTensor<ScoreT> tsr_null;
    switch (type) { PARAM_GROUPS_() default: Die("bad tensor:", type);} 
    return tsr_null;
  }
#undef PG_
  typedef CTensor<ScoreT>::Idxs Idxs;
  class ParamId {
  public:
    typedef ScoreModelContrafoldV202 SM;
    typedef vector< pair<IntT, Idxs> > Data;
    SM*   _sm;
    Data  _data;
    ParamId(SM& sm) : _sm(&sm) {}
    IntT size() const { return _data.size();}
    Data::const_reference operator[](IntT idx) const {return _data[idx];}
    IntT param_vector_size() const { return _data.size();}
    IntT param_index(const string& name) {
      for (IntT i = 0; i < (IntT)_data.size(); ++i) { if (param_name(i) == name) return i;}
      return (-1);
    }
    void run() {// cannot put this in constructor since this depends on other static values
      for (IntT i = 0; i < (IntT)_sm->nparam_group(); ++i) {// not PARAM_GROUP.size();
	const ParamGroup& pg  = _sm->param_group(i);
	const TensorT&    tsr = _sm->tensor(pg.type());
	for (IntT j = 0; j < (IntT)tsr.size(); ++j) {
	  const Idxs& idxs = tsr.indexes(j);
	  bool new_param = true;
	  switch (pg.idx_type()) {
	  case IT_NONE: break;
	  case IT_ALPHA:
	    for (IntT k = 0; k < (IntT)idxs.size(); ++k) {
	      IntT idx = idxs[k];
	      switch (pg.al_types(k)) {
	      case AT_NONE : Die("bad alpha type:", pg.al_types(k));  break;
	      case AT_NCODE: if (idx >= Alpha::N_N ) { new_param = false;} break;
	      case AT_PCODE: if (idx >= Alpha::P_NN) { new_param = false;} break;
	      case AT_SCODE: if (idx >= Alpha::S_MM) { new_param = false;} break;
	      default: Die("bad alpha type:", pg.al_types(k)); new_param = false; break;
	      }
	      if (!new_param) break;
	    }
	    break;
	  case IT_LENGTH:
	    if (pg.base_idx() >= 0) {
	      for (IntT k = 0; k < (IntT)idxs.size(); ++k) {
		if (idxs[k] < pg.base_idx()) { new_param = false; break;}
	      }
	    }
	    if (pg.sum_bound() >= 0) {
	      if (pg.sum_bound() < accumulate(idxs.begin(), idxs.end(), 0)) { new_param = false;}
	    }
	    break;
	  default:
	    Die("bad subscript type:", pg.idx_type());
	    break;
	  }
	  Idxs idxs1(idxs);
	  reverse(idxs1.begin(), idxs1.end());
	  if (pg.symmetric()) {
	    // idxs1 < idxs
	    if (vcmp(idxs1.begin(), idxs1.end(), idxs.begin(), idxs.end())) {
	      new_param = false;
	    }
	  }
	  if (!new_param) continue;
	
	  VI arr(1);
	  arr[0] = j;
	  // idxs < idxs1
	  bool b = vcmp(idxs.begin(), idxs.end(), idxs1.begin(), idxs1.end());
	  if (pg.symmetric() && b) {
	    IntT j1 = tsr.index(idxs1.begin(), idxs1.end());
	    arr.push_back(j1);
	  }
	  _data.push_back(make_pair(pg.type(), Idxs(arr.begin(), arr.end())));
	}
      }
    }
    string param_name(IntT idx) {
      const ParamGroup& pg   = _sm->param_group(_data[idx].first);
      const Idxs&       arr  = _data[idx].second;
      const TensorT&    tsr  = _sm->tensor(pg.type());
      const Idxs&       idxs = tsr.indexes(arr[0]);
      ostringstream oss;
      oss << pg.name();
      switch (pg.idx_type()) {
      case IT_NONE: break;
      case IT_ALPHA:
	oss << "_";
	for (IntT k = 0; k < (IntT)idxs.size(); ++k) {
	  switch (pg.al_types(k)) {
	  case AT_NONE: Die("bad alpha type:", pg.al_types(k)); break;
	  case AT_NCODE: oss << Alpha::ncode_to_rna_str(idxs[k]); break;
	  case AT_PCODE: oss << Alpha::pcode_to_rna_str(idxs[k]); break;
	  case AT_SCODE: oss << Alpha::scode_to_rna_str(idxs[k]); break;
	  default: Die("bad alpha type:", pg.al_types(k)); break;
	  }
	}
	break;
      case IT_LENGTH: for (IntT k = 0; k < (IntT)idxs.size(); ++k) { oss << "_" << idxs[k];} break;
      default: Die("bad subscript type:", pg.idx_type()); break;
      }
      return oss.str();
    }
  };
  Seq     _seq;
  IntT    _seqlen;
  Params  _params;
  Params  _params0;
  ParamId _param_id;
  ScoreModelContrafoldV202() : _seq(0), _seqlen(0), _param_id(*this) {}
  void initialize() {// cannot put this in constructor since this depends on other static values
    _param_id.run();
    init_tensor_values();
    set_default_params();
  }
  void set_seq(const Seq& s) {
    _seqlen = s.size();
    _seq.assign(1, Alpha::N_N); // pad at 0
    _seq.insert(_seq.end(), s.begin(), s.end());
    _seq.push_back(Alpha::N_N); // (_seqlen + 1) is accessible
  }
  IntT seqlen() { return _seqlen;}
  Seq& seq() { return _seq;}
  CodeT& seq(IntT i) { return _seq[i];}
  ScoreT tsc(TrType t, IntT i, IntT j, IntT k, IntT l) {
    switch (t) {
    case TR_S_S:    return score_stack(i, j);
    case TR_S_E:    return score_stem_close(i, j);
      // case TR_M_BF: case TR_M_BFL: case TR_M_BFR: return 0.0;
    case TR_M2_S:   return score_multi_open(i, j);
    case TR_M2_M2:  return score_multi_extend(l, j);
      // case TR_M1_M2:  return 0.0;
      // case TR_M1_MBF: return 0.0;
    case TR_M_M:    return score_multi_extend(i, k);
      // case TR_M_MBF:  return 0.0;
    case TR_E_H:    return score_hairpin(i, j);
    case TR_E_I:    return score_interior(i, j, k, l);
    case TR_E_M:    return score_multi_close(i, j);
    case TR_IB_S:   return score_outer_branch(k, l);
      // case TR_O_X:    return 0.0;
      // case TR_X_O:    return 0.0;
    case TR_O_O:    return score_outer_extend(l, j);
      // case TR_O_IB:   return 0.0;
      // case TR_O_BF: case TR_O_BFL: case TR_O_BFR: return 0.0;
    default: return 0.0;
    }
  }
  ScoreT tsc_enthalpy(TrType t, IntT i, IntT j, IntT k, IntT l) { // assuming all enthalpy
    Die("not implemented yet");
  }
  ScoreT tsc_dep_pos(TrType t, IntT i, IntT j, IntT k, IntT l, IntT pos) {
    switch (t) {
    case TR_E_H: return ! ((i + 1) < pos && pos < j); // different from energy model
    case TR_E_I: return ! (((i + 1) < pos && pos < k) || ((l + 1) < pos && pos < l));
    default: return true;
    }
  }
  void set_default_params() 
  { istringstream oss(SCORE_MODEL_CONTRAFOLD_V202_DEFAULT_PARAMS()); read_from_file(oss, true);}
  TensorT& tensor(IntT type) { return tensor(_params, type);}
  void init_tensor_values() { for (IntT i = 0; i < nparam_group0(); ++i) { tensor(i).fill(0);}}
  string param_name(IntT idx) { return _param_id.param_name(idx);}
  IntT param_index(const string& name) { return _param_id.param_index(name);}
  IntT param_vector_size() { return _param_id.param_vector_size();}
  IntT param_group_type(IntT idx) { return _param_id[idx].first;}
  const Idxs& offsets(IntT idx) { return _param_id[idx].second;}
  vector<ScoreT> param_vector() {
    vector<ScoreT> pv(param_vector_size(), 0);
    set_base_from_aux_param();
    for (IntT i = 0; i < param_vector_size(); ++i) {
      const TensorT& tsr = tensor(param_group_type(i));
      const Idxs& arr = offsets(i);
      ScoreT z = 0.0;
      for (IntT j = 0; j < (IntT)arr.size(); ++j) { z += *(tsr.begin() + arr[j]);}
      pv[i] = (z / (ScoreT)arr.size());
    }
    return pv;
  }
  void clean() {
    vector<ScoreT> cv(param_vector_size(), 0);
    set_count_vector(cv);
  }
  void set_param_vector(const vector<ScoreT>& pv) {
    Check((IntT)pv.size() == param_vector_size());
    init_tensor_values();
    for (IntT i = 0; i < param_vector_size(); ++i) {
      TensorT& tsr          = tensor(param_group_type(i));
      const Idxs& arr = offsets(i);
      for (IntT j = 0; j < (IntT)arr.size(); ++j) { *(tsr.begin() + arr[j]) = pv[i];}
    }
    set_aux_from_base_param();
  }
  void set_param(const string& name, ScoreT score) {
    IntT i = param_index(name); Check(i >= 0, "no param: ", name);
    TensorT& tsr = tensor(param_group_type(i));
    const Idxs& arr = offsets(i);
    for (IntT j = 0; j < (IntT)arr.size(); ++j) { *(tsr.begin() + arr[j]) = score;}
    set_aux_from_base_param();
  }
  vector<ScoreT> count_vector() {
    vector<ScoreT> cv(param_vector_size(), 0);
    set_base_from_aux_count();
    for (IntT i = 0; i < param_vector_size(); ++i) {
      const TensorT& tsr = tensor(param_group_type(i));
      const Idxs& arr = offsets(i);
      ScoreT z = 0.0;
      for (IntT j = 0; j < (IntT)arr.size(); ++j) { z += *(tsr.begin() + arr[j]);}
      cv[i] = z;
    }
    return cv;
  }
  void set_count_vector(const vector<ScoreT>& cv) {
    Check((IntT)cv.size() == param_vector_size());
    init_tensor_values();
    for (IntT i = 0; i < param_vector_size(); ++i) {
      TensorT& tsr = tensor(param_group_type(i));
      const Idxs& arr = offsets(i);
      ScoreT factor = 1.0 / (ScoreT)arr.size();
      for (IntT j = 0; j < (IntT)arr.size(); ++j) { *(tsr.begin() + arr[j]) = factor * cv[i];}
    }
    set_aux_from_base_count();
  }
  void read_from_file(istream& fi, bool supress_warning = false) {
    map<string, ScoreT> h;
    string buf;
    while (getline(fi, buf)) {
      //  const Vector<string>& tokens = tokenize<string>(buf);
      const vector<string>& tokens = splitv(buf);
      if (tokens.empty()) continue;

      Check(tokens.size() == 2);
      h[tokens[0]] = atof(tokens[1].c_str());
    }
    const string& name = "temperature_k";
    if (h.find(name) != h.end()) {
      // parameter file is in energy format [kcal/mol]
      _params.temperature_k() = h[name];
    } else {
      _params.temperature_k() = TEMPERATURE_K();
      if (!supress_warning) { Warn("no entry for name:", name);}
    }
    vector<ScoreT> pv(param_vector_size(), 0.0);
    for (IntT i = 0; i < (IntT)pv.size(); i++) {
      const string& name = param_name(i);
      if (h.find(name) != h.end()) {
	// parameter file is in energy format [kcal/mol]
	pv[i] = energy_to_score(h[name]); 
      } else {
	if (!supress_warning) { Warn("no entry for idx:", i, ",name:", name);}
      }
    }
    set_param_vector(pv);
    _params0 = _params;
  }
  ScoreT energy_to_score(ScoreT energy) { return (-energy / RT_KCAL_MOL(_params.temperature_k()));}
  ScoreT score_to_energy(ScoreT score) { return (-score * RT_KCAL_MOL(_params.temperature_k()));}
  void change_temperature(ScoreT temperature_c) { // Be careful for parameter changing method
    Die("not implemented yet");
  }
  void scale_enthalpy_param_group(const string& name, const string& name_h, ScoreT tfac, bool non_pos) {
    // assuming enthalpy and entropy is temperature independent.
    TensorT& t    = tensor(_params, param_group(name).type());
    TensorT& t_h  = tensor(_params, param_group(name_h).type());
    TensorT& t0   = tensor(_params0, param_group(name).type());
    TensorT& t0_h = tensor(_params0, param_group(name_h).type());
    Check(t.size() == t_h.size() && t0.size() == t0_h.size() && t.size() == t0.size());
    TensorT::iterator       ti    = t.begin();
    TensorT::iterator       t_hi  = t_h.begin();
    TensorT::const_iterator t0i   = t0.begin();
    TensorT::const_iterator t0_hi = t0_h.begin();
    for (; ti != t.end(); ++ti, ++t_hi, ++t0i, ++t0_hi) {
      (*ti)   = (tfac * (*t0_hi)) + ((*t0i) - (*t0_hi));
      (*t_hi) = (tfac * (*t0_hi));
      if (non_pos && 0 < (*ti)) { (*ti) = 0.0;}
    }
  }
  string param_file_str() {
    const vector<ScoreT>& pv = param_vector();
    ostringstream oss;
    for (IntT i = 0; i < (IntT)pv.size(); i++) {
      // parameter file is in energy format [kcal/mol]
      oss << param_name(i) << " " << score_to_energy(pv[i]) << "\n";
    }
    return oss.str();
  }
  void set_aux_from_base_param() {
    for (IntT i = nparam_group(); i < nparam_group1(); ++i) {
      const ParamGroup& pg = param_group(i);
      TensorT& tsr = tensor(pg.type());
      const TensorT& tsr0 = tensor(pg.pg_pair());
      for (IntT j = 0; j < (IntT)tsr0.size(); ++j) { 
	tsr.ref(j) = (tsr0.ref(j) + (j > 0 ? tsr.ref(j-1) : 0));
      }
    }
    TensorT& tsr = tensor(cache_single_length);
    vfill(tsr, 0);
    for (IntT l1 = 0; l1 <= MAXLOOP; ++l1) {
      for (IntT l2 = 0; l2 <= MAXLOOP; ++l2) {
	if (l1 == 0 && l2 == 0) continue;
	if (MAXLOOP < l1 + l2) continue;
	if (l1 == 0 || l2 == 0) {
	  tsr.ref(l1, l2) += tensor(cache_bulge_length).ref(l1 + l2);
	} else {
	  tsr.ref(l1, l2) += tensor(cache_internal_length).ref(l1 + l2);
	  if (l1 == l2) { tsr.ref(l1, l2) += tensor(cache_internal_symmetric_length).ref(l1);}
	  tsr.ref(l1, l2) += tensor(cache_internal_asymmetry).ref(abs(l1 - l2));
	  tsr.ref(l1, l2) += tensor(internal_explicit).ref(l1, l2);
	}
      }
    }
  }
  void set_base_from_aux_param() {
    TensorT& tsr = tensor(cache_single_length);
    for (IntT l1 = 0; l1 <= MAXLOOP; ++l1) {
      for (IntT l2 = 0; l2 <= MAXLOOP; ++l2) {
	if (l1 == 0 && l2 == 0) continue;
	if (MAXLOOP < l1 + l2) continue;
	if (l1 == 0 || l2 == 0) {
	} else {
	  ScoreT z = tsr.ref(l1, l2); // local copy
	  z -= tensor(cache_internal_length).ref(l1 + l2);
	  if (l1 == l2) { z -= tensor(cache_internal_symmetric_length).ref(l1);}
	  z -= tensor(cache_internal_asymmetry).ref(abs(l1 - l2));
	  tensor(internal_explicit).ref(l1, l2) = z;
	}
      }
    }
    for (IntT i = nparam_group(); i < nparam_group1(); ++i) {
      const ParamGroup& pg = param_group(i);
      const TensorT& tsr = tensor(pg.type());
      TensorT& tsr0 = tensor(pg.pg_pair());
      for (IntT j = 0; j < (IntT)tsr0.size(); ++j) {
	tsr0.ref(j) = (tsr.ref(j) - (j > 0 ? tsr.ref(j-1) : 0));
      }
    }
  }
  void set_aux_from_base_count() {
    for (IntT i = nparam_group(); i < nparam_group1(); i++) {
      const ParamGroup& pg = param_group(i);
      TensorT& tsr = tensor(pg.type());
      const TensorT& tsr0 = tensor(pg.pg_pair());
      IntT n = tsr0.size();
      for (IntT j = (n-1); j >= 0; j--) {
	tsr.ref(j) = (tsr0.ref(j) - (j < (n-1) ? tsr0.ref(j+1) : 0));
      }
    }
    TensorT& tsr = tensor(cache_single_length);
    vfill(tsr, 0);
    for (IntT l1 = 0; l1 <= MAXLOOP; ++l1) {
      for (IntT l2 = 0; l2 <= MAXLOOP; ++l2) {
	if (l1 == 0 && l2 == 0) continue;
	if (MAXLOOP < l1 + l2) continue;
	if (l1 == 0 || l2 == 0) {
	  tsr.ref(l1, l2) += 0.5 * tensor(cache_bulge_length).ref(l1 + l2); // left right symmetry
	} else {
	  ScoreT cnt = 0;
	  // symmetry is more complicated
	  // tsr.ref(l1, l2) += 0.5 * tensor(cache_internal_length).ref(l1 + l2); ++cnt;
	  // if (l1 == l2) { 
	  //   tsr.ref(l1, l2) += 0.5 * tensor(cache_internal_symmetric_length).ref(l1); ++cnt;
	  // }
	  // tsr.ref(l1, l2) += tensor(cache_internal_asymmetry).ref(0); ++cnt;
	  tsr.ref(l1, l2) += tensor(internal_explicit).ref(l1, l2); ++cnt;
	  tsr.ref(l1, l2) /= cnt;
	}
      }
    }
  }
  void set_base_from_aux_count() {
    TensorT& tsr = tensor(cache_single_length);
    vfill(tensor(cache_bulge_length), 0);
    vfill(tensor(internal_explicit), 0);
    vfill(tensor(cache_internal_length), 0);
    vfill(tensor(cache_internal_symmetric_length), 0);
    vfill(tensor(cache_internal_asymmetry), 0);
    for (IntT l1 = 0; l1 <= MAXLOOP; ++l1) {
      for (IntT l2 = 0; l2 <= MAXLOOP; ++l2) {
	if (l1 == 0 && l2 == 0) continue;
	if (MAXLOOP < l1 + l2) continue;
	if (l1 == 0 || l2 == 0) {
	  tensor(cache_bulge_length).ref(l1 + l2) += tsr.ref(l1, l2);
	} else {
	  tensor(cache_internal_length).ref(l1 + l2) += tsr.ref(l1, l2);
	  if (l1 == l2) { tensor(cache_internal_symmetric_length).ref(l1) += tsr.ref(l1, l2);}
	  tensor(cache_internal_asymmetry).ref(abs(l1 - l2)) += tsr.ref(l1, l2);
	  tensor(internal_explicit).ref(l1, l2) += tsr.ref(l1, l2);
	}
      }
    }
    for (IntT i = nparam_group(); i < nparam_group1(); i++) {
      const ParamGroup& pg = param_group(i);
      const TensorT& tsr = tensor(pg.type());
      TensorT& tsr0 = tensor(pg.pg_pair());
      IntT n = tsr0.size();
      for (IntT j = (n-1); j >= 0; j--) {
	tsr0.ref(j) = (tsr.ref(j) + (j < (n-1) ? tsr0.ref(j+1) : 0));
      }
    }
  }
  ScoreT score_hybridize_rna(IntT b, IntT e) { // 0 based half-open dp cell coordinate
    ScoreT sc = score_rna_rna_init();
    for (IntT i = (b + 2); i <= e; ++i) {
      IntT cx5 = seq(i - 1);
      IntT cx3 = seq(i);
      if (!(Alpha::is_base_ncode(cx5) && Alpha::is_base_ncode(cx3))) continue;
      IntT cy5 = Alpha::compl_ncode(cx3);
      IntT cy3 = Alpha::compl_ncode(cx5);
      sc += score_stack_rna_rna(cx5, cx3, cy5, cy3);
    }
    return sc;
  }
  ScoreT score_hybridize_dna(IntT b, IntT e) { // 0 based half-open dp cell coordinate
    ScoreT sc = score_rna_dna_init();
    for (IntT i = (b + 2); i <= e; ++i) {
      IntT cx5 = seq(i - 1);
      IntT cx3 = seq(i);
      if (!(Alpha::is_base_ncode(cx5) && Alpha::is_base_ncode(cx3))) continue;
      IntT cy5 = Alpha::compl_ncode(cx3);
      IntT cy3 = Alpha::compl_ncode(cx5);
      sc += score_stack_rna_dna(cx5, cx3, cy5, cy3);
    }
    return sc;
  }
#define TENSOR(name) _params.tsr_##name()
  ScoreT score_stack_rna_rna(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_rna_rna).ref(c, c1);
  }
  ScoreT score_stack_rna_dna(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_rna_dna).ref(c, c1);
  }
  ScoreT score_rna_rna_init() { return TENSOR(rna_rna_init).ref();}
  ScoreT score_rna_dna_init() { return TENSOR(rna_dna_init).ref();}
  ScoreT score_junction_a(IntT i, IntT j) {
    ScoreT sc = 0.0;
    sc += TENSOR(helix_closing).ref(seq(i), seq(j+1));
    if (i < _seqlen) { sc += TENSOR(dangle_left).ref(seq(i), seq(j+1), seq(i+1));} // 3'->left
    if (0 < j) { sc += TENSOR(dangle_right).ref(seq(i), seq(j+1), seq(j));} // 5'-> right
    return sc;
  }
  void count_junction_a(IntT i, IntT j, ScoreT w) {
    TENSOR(helix_closing).ref(seq(i), seq(j+1)) += w;
    if (i < _seqlen) { TENSOR(dangle_left).ref(seq(i), seq(j+1), seq(i+1)) += w;}
    if (0 < j) { TENSOR(dangle_right).ref(seq(i), seq(j+1), seq(j)) += w;}
  }
  ScoreT score_junction_b(IntT i, IntT j) {
    ScoreT sc = 0.0;
    sc += TENSOR(helix_closing).ref(seq(i), seq(j+1));
    sc += TENSOR(terminal_mismatch).ref(seq(i), seq(j+1), seq(i+1), seq(j));
    return sc;
  }
  void count_junction_b(IntT i, IntT j, ScoreT w) {
    TENSOR(helix_closing).ref(seq(i), seq(j+1)) += w;
    TENSOR(terminal_mismatch).ref(seq(i), seq(j+1), seq(i+1), seq(j)) += w;
  }
  ScoreT score_stem_close(IntT i, IntT j) { 
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return NEG_INF();
#endif
    return TENSOR(base_pair).ref(seq(i+1), seq(j));
  }
  void count_stem_close(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return;
#endif
    TENSOR(base_pair).ref(seq(i+1), seq(j)) += w;
  }
  ScoreT score_hairpin(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1))) return NEG_INF();
#endif
    ScoreT sc = 0;
    sc += score_junction_b(i, j);
    sc += TENSOR(cache_hairpin_length).ref(min(j - i, (IntT)MAXLOOP));
    return sc;
  }
  void count_hairpin(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1))) return;
#endif
    count_junction_b(i, j, w);
    TENSOR(cache_hairpin_length).ref(min(j - i, (IntT)MAXLOOP)) += w;
  }
  ScoreT score_stack(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j)) || 
	!Alpha::is_canonical(seq(j-1), seq(i+2))) return NEG_INF();
#endif
    ScoreT sc = 0;
    sc += TENSOR(base_pair).ref(seq(i+1), seq(j));
    sc += TENSOR(helix_stacking).ref(seq(i+1), seq(j), seq(i+2), seq(j-1));
    return sc;
  }
  void count_stack(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j)) || 
	!Alpha::is_canonical(seq(j-1), seq(i+2))) return;
#endif
    TENSOR(base_pair).ref(seq(i+1), seq(j)) += w;
    TENSOR(helix_stacking).ref(seq(i+1), seq(j), seq(i+2), seq(j-1)) += w;
  }
  ScoreT score_single_nucleotides(IntT i, IntT j, IntT li, IntT lj) {
    ScoreT sc = 0;
    if (li + lj <= 2) {
      if (li + lj == 1) { sc += TENSOR(bulge_0x1_nucleotides).ref(li == 1 ? seq(i + 1) : seq(j));}
      else if (li == 1) { sc += TENSOR(internal_1x1_nucleotides).ref(seq(i+1), seq(j));}
    }
    return sc;
  }
  void count_single_nucleotides(IntT i, IntT j, IntT li, IntT lj, ScoreT w) {
    if (li + lj <= 2) {
      if (li + lj == 1) { TENSOR(bulge_0x1_nucleotides).ref(li == 1 ? seq(i+1) : seq(j)) += w;}
      else if (li == 1) { TENSOR(internal_1x1_nucleotides).ref(seq(i+1), seq(j)) += w;}
    }
  }
  ScoreT score_interior(IntT i, IntT j, IntT ip, IntT jp) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1)) || 
	!Alpha::is_canonical(seq(jp), seq(ip+1))) return NEG_INF();
#endif
    IntT li = (ip - i);
    IntT lj = (j - jp);
    ScoreT sc = 0;
    sc += TENSOR(cache_single_length).ref(li, lj);
    sc += score_junction_b(i, j);
    sc += score_junction_b(jp, ip);
    sc += score_single_nucleotides(i, j, li, lj);
    return sc;
  }
  void count_interior(IntT i, IntT j, IntT ip, IntT jp, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1)) || 
	!Alpha::is_canonical(seq(jp), seq(ip+1))) return;
#endif
    IntT li = (ip - i);
    IntT lj = (j - jp);
    TENSOR(cache_single_length).ref(li, lj) += w;
    count_junction_b(i, j, w);
    count_junction_b(jp, ip, w);
    count_single_nucleotides(i, j, li, lj, w);
  }
  ScoreT score_multi_close(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1))) return NEG_INF();
#endif
    ScoreT sc = 0;
    sc += TENSOR(multi_base).ref();
    sc += score_junction_a(i, j);
    return sc;
  }
  void count_multi_close(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i), seq(j+1))) return;
#endif
    TENSOR(multi_base).ref() += w;
    count_junction_a(i, j, w);
  }
  ScoreT score_multi_open(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return NEG_INF();
#endif
    ScoreT sc = 0;
    sc += score_junction_a(j, i);
    sc += TENSOR(multi_paired).ref();
    return sc;
  }
  void count_multi_open(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return;
#endif
    count_junction_a(j, i, w);
    TENSOR(multi_paired).ref() += w;
  }
  ScoreT score_multi_extend(IntT i, IntT j) { return (j - i) * TENSOR(multi_unpaired).ref();}
  void count_multi_extend(IntT i, IntT j, ScoreT w) { TENSOR(multi_unpaired).ref() += (j - i) * w;}
  ScoreT score_outer_extend(IntT i, IntT j) { return (j - i) * TENSOR(external_unpaired).ref();}
  void count_outer_extend(IntT i, IntT j, ScoreT w) 
  { TENSOR(external_unpaired).ref() += (j - i) * w;}
  ScoreT score_outer_branch(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return NEG_INF();
#endif
    ScoreT sc = 0;
    sc += score_junction_a(j, i);
    sc += TENSOR(external_paired).ref();
    sc += TENSOR(external_paired_length_factor).ref() * (j-i);
    return sc;
  }
  void count_outer_branch(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    if (!Alpha::is_canonical(seq(i+1), seq(j))) return;
#endif
    count_junction_a(j, i, w);
    TENSOR(external_paired).ref() += w;
    TENSOR(external_paired_length_factor).ref() += (j-i) * w;
  }
#undef TENSOR
};
#define ST_(type, layer_in, layer_out, sc_dep_outer, sc_dep_inner) \
{type, layer_in, layer_out, sc_dep_outer, sc_dep_inner, #type},
const Raccess::ScoreModelContrafoldV202::State 
Raccess::ScoreModelContrafoldV202::STATES[] = {STATES_()};
#undef ST_
#define TR_(type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, emits_nuc) \
{ type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, emits_nuc, #type },	     
 const Raccess::ScoreModelContrafoldV202::Transition 
Raccess::ScoreModelContrafoldV202::TRANSITIONS[] = {TRANSITIONS_()};
#undef TR_	
#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) \
{type,idx_type,{{l0,l1,l2,l3}},{{a0,a1,a2,a3}},pg_pair,symmetric,base_idx,sum_bound,#type},
const Raccess::ScoreModelContrafoldV202::ParamGroup 
Raccess::ScoreModelContrafoldV202::PARAM_GROUPS[] = {PARAM_GROUPS_()};
#undef PG_
#undef STATES_
#undef TRANSITIONS_
#undef PARAM_GROUPS_
#endif
