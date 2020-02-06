/*
 * author: hisanori kiryu
 */
#ifndef RACCESS__SCORE_MODEL_ENERGY_HPP
#define RACCESS__SCORE_MODEL_ENERGY_HPP
#include "util/util.hpp"
#include "util/tensor.hpp"
#include "raccess/score.hpp"
#include "raccess/score_model_energy_default_params.hpp"
namespace Raccess { class ScoreModelEnergy;}
// #define CHECK_CANONICAL
class Raccess::ScoreModelEnergy {
public:
  typedef ScoreModelEnergy Self;
  typedef long             IntT;
  static ScoreT GAS_CONST_KCAL_MOL_K() { return (1.9872 * 0.001);}
  static ScoreT TEMPERATURE_K0() { return 273.15;}
  static ScoreT TEMPERATURE_K() { return (TEMPERATURE_K0() + 37.0);}
  static ScoreT RT_KCAL_MOL(ScoreT temperature_k) { return GAS_CONST_KCAL_MOL_K() * temperature_k;}
  static ScoreT RT_KCAL_MOL() { return GAS_CONST_KCAL_MOL_K() * TEMPERATURE_K();} // =0.61633008
  typedef Alpha::CodeT CodeT;
  typedef Alpha::Codes Seq;
  typedef vector<IntT> VI;
  enum {
    NNCODE   = Alpha::NNCODE,
    NNCODEN  = Alpha::NNCODEN,
    NSCODEN  = Alpha::NSCODEN,
    NPCODEN  = Alpha::NPCODEN,
    MAXLOOP  = 30, // maximal size of interior loop
    MINHPIN  = 3
    // MINHPIN  = 1 // for test
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
    attr(char  , type        );
    attr(char  , layer_in    );
    attr(char  , layer_out   );
    attr(bool  , sc_dep_outer);
    attr(bool  , sc_dep_inner);
    attr(string, name        );
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
    NOP_TYPE = 1
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
    attr(Lengths, lengths  ); // fill (-1) for rank <= i	     
    attr(AlTypes, al_types );					     
    attr(char   , pg_pair  );					     
    attr(bool   , symmetric);
    attr(IntT   , base_idx );
    attr(IntT   , sum_bound);
    attr(string , name     );
    char al_types(IntT k) const { return _al_types[k];}
    IntT rank() const {
      for (IntT k = 0; k < (IntT)_lengths.size(); ++k) { if (_lengths[k] < 0) return k;}
      return _lengths.size();
    }
  };
#define PARAM_GROUPS_()     \
  PG_(large_loop_factor   , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(multiloop_offset    , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(multiloop_nuc       , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(multiloop_helix     , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(max_ninio           , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(rna_rna_init        , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(hairpinloop_change  , \
      IT_LENGTH           , \
      (MAXLOOP+1),(-1),(-1),(-1), \
      (-1),(-1),(-1),(-1) ,       \
      hairpinloop_cumu_change,    \
      false,(-1),(-1)     ) \
  PG_(bulgeloop_change    , \
      IT_LENGTH           , \
      (MAXLOOP+1),(-1),(-1),(-1), \
      (-1),(-1),(-1),(-1) ,       \
      bulgeloop_cumu_change,      \
      false,(-1),(-1)     ) \
  PG_(internalloop_change , \
      IT_LENGTH           , \
      (MAXLOOP+1),(-1),(-1),(-1),  \
      (-1),(-1),(-1),(-1) ,        \
      internalloop_cumu_change,    \
      false,(-1),(-1)     )        \
  PG_(internalloop_asymmetry,      \
      IT_LENGTH           ,        \
      (MAXLOOP+1),(-1),(-1),(-1),  \
      (-1),(-1),(-1),(-1) ,        \
      internalloop_cumu_asymmetry, \
      false,(-1),(-1)     ) \
  PG_(internalloop_asymmetry_h,    \
      IT_LENGTH           ,        \
      (MAXLOOP+1),(-1),(-1),(-1),  \
      (-1),(-1),(-1),(-1) ,        \
      param_group_null,            \
      false,(-1),(-1)     ) \
  PG_(terminal_base_pair  , \
      IT_ALPHA            , \
      NSCODEN,(-1),(-1),(-1),  \
      AT_SCODE,(-1),(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(terminal_base_pair_h , \
      IT_ALPHA            , \
      NSCODEN,(-1),(-1),(-1),  \
      AT_SCODE,(-1),(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(dangle3             , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,(-1),(-1),   \
      AT_SCODE,AT_NCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(dangle3_h           , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,(-1),(-1),   \
      AT_SCODE,AT_NCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(dangle5             , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,(-1),(-1),   \
      AT_SCODE,AT_NCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(dangle5_h           , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,(-1),(-1),   \
      AT_SCODE,AT_NCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(stack               , \
      IT_ALPHA            , \
      NSCODEN,NSCODEN,(-1),(-1),   \
      AT_SCODE,AT_SCODE,(-1),(-1), \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(stack_h             , \
      IT_ALPHA            , \
      NSCODEN,NSCODEN,(-1),(-1),   \
      AT_SCODE,AT_SCODE,(-1),(-1), \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(stack_rna_dna       , \
      IT_ALPHA            , \
      NSCODEN,NSCODEN,(-1),(-1),   \
      AT_SCODE,AT_SCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(stack_rna_dna_h     , \
      IT_ALPHA            , \
      NSCODEN,NSCODEN,(-1),(-1),   \
      AT_SCODE,AT_SCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(rna_dna_init        , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(tstackh             , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,(-1),(-1),   \
      AT_SCODE,AT_PCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(tstacki             , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,(-1),(-1),   \
      AT_SCODE,AT_PCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(tstack_h            , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,(-1),(-1),   \
      AT_SCODE,AT_PCODE,(-1),(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(int2                , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,NNCODEN,NSCODEN,     \
      AT_SCODE,AT_NCODE,AT_NCODE,AT_SCODE, \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(int2_h              , \
      IT_ALPHA            , \
      NSCODEN,NNCODEN,NNCODEN,NSCODEN,     \
      AT_SCODE,AT_NCODE,AT_NCODE,AT_SCODE, \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(int21               , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NNCODEN,NSCODEN,     \
      AT_SCODE,AT_PCODE,AT_NCODE,AT_SCODE, \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(int21_h             , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NNCODEN,NSCODEN,     \
      AT_SCODE,AT_PCODE,AT_NCODE,AT_SCODE, \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(int22               , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NPCODEN,NSCODEN,     \
      AT_SCODE,AT_PCODE,AT_PCODE,AT_SCODE, \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(int22_h             , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NPCODEN,NSCODEN,     \
      AT_SCODE,AT_PCODE,AT_PCODE,AT_SCODE, \
      param_group_null    , \
      true,(-1),(-1)      ) \
  PG_(triloop             , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NNCODEN,(-1),    \
      AT_SCODE,AT_PCODE,AT_NCODE,(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(triloop_h           , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NNCODEN,(-1),    \
      AT_SCODE,AT_PCODE,AT_NCODE,(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(tetraloop           , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NPCODEN,(-1),    \
      AT_SCODE,AT_PCODE,AT_PCODE,(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(tetraloop_h         , \
      IT_ALPHA            , \
      NSCODEN,NPCODEN,NPCODEN,(-1),    \
      AT_SCODE,AT_PCODE,AT_PCODE,(-1), \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(external_unpaired   , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(external_paired     , \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(external_paired_length_factor, \
      IT_NONE             , \
      (-1),(-1),(-1),(-1) , \
      (-1),(-1),(-1),(-1) , \
      param_group_null    , \
      false,(-1),(-1)     ) \
  PG_(hairpinloop_cumu_change,    \
      IT_LENGTH           ,       \
      (MAXLOOP+1),(-1),(-1),(-1), \
      (-1),(-1),(-1),(-1) , \
      hairpinloop_change  , \
      false,(-1),(-1)     ) \
  PG_(bulgeloop_cumu_change,      \
      IT_LENGTH           ,       \
      (MAXLOOP+1),(-1),(-1),(-1), \
      (-1),(-1),(-1),(-1) , \
      bulgeloop_change    , \
      false,(-1),(-1)     ) \
  PG_(internalloop_cumu_change,   \
      IT_LENGTH           ,       \
      (MAXLOOP+1),(-1),(-1),(-1), \
      (-1),(-1),(-1),(-1) , \
      internalloop_change , \
      false,(-1),(-1)     ) \
  PG_(internalloop_cumu_asymmetry, \
      IT_LENGTH           ,        \
      (MAXLOOP+1),(-1),(-1),(-1),  \
      (-1),(-1),(-1),(-1) ,        \
      internalloop_asymmetry,      \
      false,(-1),(-1)     )   

#define PG_(type,idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) type,
  enum PgType { PARAM_GROUPS_() param_group_null = (-1)};
#undef PG_
  enum {
    NPARAMGROUP  = (external_paired_length_factor  - large_loop_factor + 1),
    NPARAMGROUP1 = (internalloop_cumu_asymmetry - large_loop_factor + 1),
    NPARAMGROUP0 = (internalloop_cumu_asymmetry - large_loop_factor + 1)
  };
  // #define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound)
  //    {type,idx_type,{{l0,l1,l2,l3}},{{a0,a1,a2,a3}},pg_pair,symmetric,base_idx,sum_bound,#type},
  //  static const ParamGroup& param_groups(int t) 
  // { static const ParamGroup a[] = {PARAM_GROUPS_()}; return a[t];}
  // #undef PG_
  static const ParamGroup PARAM_GROUPS[NPARAMGROUP1];
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
  CTensor<ScoreT, l0,l1,l2,l3> _##type;                                    \
  CTensor<ScoreT, l0,l1,l2,l3> const& tsr_##type() const {return _##type;} \
  CTensor<ScoreT, l0,l1,l2,l3>& tsr_##type() {return _##type;};
  class Params { public: PARAM_GROUPS_(); attr(ScoreT, temperature_k);};
#undef PG_
#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) \
  case type: return params.tsr_##type();
  static TensorT& tensor(Params& params, IntT t) {
    static CTensor<ScoreT> tsr_null;
    switch (t) { PARAM_GROUPS_() default: Die("bad tensor %d", t);} 
    return tsr_null;
  }
#undef PG_
  typedef CTensor<ScoreT>::Idxs Idxs;
  class ParamId {
  public:
    typedef ScoreModelEnergy SM;
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
    void run() { // cannot put this in constructor since this depends on other static values
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
	      case AT_NONE : Die("bad alpha type %d", pg.al_types(k));  break;
	      case AT_NCODE: if (idx >= Alpha::N_N ) { new_param = false;} break;
	      case AT_PCODE: if (idx >= Alpha::P_NN) { new_param = false;} break;
	      case AT_SCODE: if (idx >= Alpha::S_MM) { new_param = false;} break;
	      default: Die("bad alpha type %d", pg.al_types(k)); new_param = false; break;
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
	    Die("bad subscript type %d", pg.idx_type());
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
      const Idxs& arr  = _data[idx].second;
      const TensorT& tsr     = _sm->tensor(pg.type());
      const Idxs& idxs = tsr.indexes(arr[0]);
      ostringstream oss;
      oss << pg.name();
      switch (pg.idx_type()) {
      case IT_NONE: break;
      case IT_ALPHA:
	oss << "_";
	for (IntT k = 0; k < (IntT)idxs.size(); ++k) {
	  switch (pg.al_types(k)) {
	  case AT_NONE: Die("bad alpha type %d", pg.al_types(k)); break;
	  case AT_NCODE: oss << Alpha::ncode_to_rna_str(idxs[k]); break;
	  case AT_PCODE: oss << Alpha::pcode_to_rna_str(idxs[k]); break;
	  case AT_SCODE: oss << Alpha::scode_to_rna_str(idxs[k]); break;
	  default: Die("bad alpha type %d", pg.al_types(k)); break;
	  }
	}
	break;
      case IT_LENGTH: for (IntT k = 0; k < (IntT)idxs.size(); ++k) { oss << "_" << idxs[k];} break;
      default: Die("bad subscript type %d", pg.idx_type()); break;
      }
      return oss.str();
    }
  };
  Seq     _seq;
  IntT    _seqlen;
  Params  _params;
  Params  _params0;
  ParamId _param_id;
  ScoreModelEnergy() : _seq(0), _seqlen(0), _param_id(*this) {}
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
  CodeT& seq(IntT i) { Assert(0 <= i && i < (IntT)_seq.size()); return _seq[i];}
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
  ScoreT tsc_enthalpy(TrType t, IntT i, IntT j, IntT k, IntT l) {
    switch (t) {
    case TR_S_S:    return score_stack_enthalpy(i, j);
    case TR_S_E:    return score_stem_close_enthalpy(i, j);
      // case TR_M_BF: case TR_M_BFL: case TR_M_BFR: return 0.0;
    case TR_M2_S:   return score_multi_open_enthalpy(i, j);
    case TR_M2_M2:  return score_multi_extend_enthalpy(l, j);
      // case TR_M1_M2:  return 0.0;
      // case TR_M1_MBF: return 0.0;
    case TR_M_M:    return score_multi_extend_enthalpy(i, k);
      // case TR_M_MBF:  return 0.0;
    case TR_E_H:    return score_hairpin_enthalpy(i, j);
    case TR_E_I:    return score_interior_enthalpy(i, j, k, l);
    case TR_E_M:    return score_multi_close_enthalpy(i, j);
    case TR_IB_S:   return score_outer_branch_enthalpy(k, l);
      // case TR_O_X:    return 0.0;
      // case TR_X_O:    return 0.0;
    case TR_O_O:    return score_outer_extend_enthalpy(l, j);
      // case TR_O_IB:   return 0.0;
      // case TR_O_BF: case TR_O_BFL: case TR_O_BFR: return 0.0;
    default: return 0.0;
    }
  }
  ScoreT tsc_dep_pos(TrType t, IntT i, IntT j, IntT k, IntT l, IntT pos) {
    switch (t) {
    case TR_E_H: return ! (4 < (j - i) && (i + 1) < pos && pos < j);
    case TR_E_I: return ! (((i + 1) < pos && pos < k) || ((l + 1) < pos && pos < l));
    default: return true;
    }
  }
  void tsc_nuc_pos(VI& v, TrType t, IntT i, IntT j, IntT k, IntT l) {
    switch (t) {
    case TR_S_S:    return tsc_nuc_pos_stack(v, i, j);
    case TR_S_E:    return tsc_nuc_pos_stem_close(v, i, j);
      // case TR_M_BF: case TR_M_BFL: case TR_M_BFR: return 0.0;
    case TR_M2_S:   return tsc_nuc_pos_multi_open(v, i, j);
      // case TR_M2_M2 : return tsc_nuc_pos_multi_extend(v, l, j);
      // case TR_M1_M2 : return 0.0;
      // case TR_M1_MBF: return 0.0;
      // case TR_M_M   : return tsc_nuc_pos_multi_extend(v, i, k);
      // case TR_M_MBF : return 0.0;
    case TR_E_H:    return tsc_nuc_pos_hairpin(v, i, j);
    case TR_E_I:    return tsc_nuc_pos_interior(v, i, j, k, l);
    case TR_E_M:    return tsc_nuc_pos_multi_close(v, i, j);
    case TR_IB_S:   return tsc_nuc_pos_outer_branch(v, k, l);
      // case TR_O_X:    return 0.0;
      // case TR_X_O:    return 0.0;
      // case TR_O_O:    return tsc_nuc_pos_outer_extend(v, l, j);
      // case TR_O_IB:   return 0.0;
      // case TR_O_BF: case TR_O_BFL: case TR_O_BFR: return 0.0;
    default: v.clear(); return;
    }
  }
  void set_default_params() 
  { istringstream oss(SCORE_MODEL_ENERGY_DEFAULT_PARAMS()); read_from_file(oss, true);}
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
    IntT i = param_index(name); Check(i >= 0, "no param %s", name.c_str());
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
      if (!supress_warning) { Warn("no entry for name: %s", name.c_str());}
    }
    vector<ScoreT> pv(param_vector_size(), 0);
    for (IntT i = 0; i < (IntT)pv.size(); i++) {
      const string& name = param_name(i);
      if (h.find(name) != h.end()) {
	// parameter file is in energy format [kcal/mol]
	pv[i] = energy_to_score(h[name]); 
      } else {

	if (!supress_warning) { Warn("no entry for idx: %d, name: %s", i, name.c_str());}
      }
    }
    set_param_vector(pv);
    _params0 = _params; // keep original params; Be careful for parameter changing method
  }
  ScoreT energy_to_score(ScoreT energy) { return (-energy / RT_KCAL_MOL(_params.temperature_k()));}
  ScoreT score_to_energy(ScoreT score) { return (-score * RT_KCAL_MOL(_params.temperature_k()));}
  void change_temperature(ScoreT temperature_c) { // Be careful for parameter changing method
    // assuming enthalpy and entropy is temperature independent.
    _params = _params0; // purely entropy terms need not to be altered.
    _params.temperature_k() = (TEMPERATURE_K0() + temperature_c);
    ScoreT tfac = (_params0.temperature_k() / _params.temperature_k());
    // internal loop asymmetry is strange
    scale_enthalpy_param_group("internal_loop_asymmetry", "internal_loop_asymmetry_h", tfac, false);
    scale_enthalpy_param_group("terminal_base_pair", "terminal_base_pair_h", tfac, false);
    scale_enthalpy_param_group("dangle3", "dangle3_h", tfac, true);
    scale_enthalpy_param_group("dangle5", "dangle5_h", tfac, true);
    scale_enthalpy_param_group("stack", "stack_h", tfac, false);
    scale_enthalpy_param_group("stack_rna_dna", "stack_rna_dna_h", tfac, false);
    scale_enthalpy_param_group("tstackh", "tstack_h", tfac, false);
    scale_enthalpy_param_group("tstacki", "tstack_h", tfac, false);
    scale_enthalpy_param_group("int2", "int2_h", tfac, false);
    scale_enthalpy_param_group("int21", "int21_h", tfac, false);
    scale_enthalpy_param_group("int22", "int22_h", tfac, false);
    scale_enthalpy_param_group("triloop", "triloop_h", tfac, false);
    scale_enthalpy_param_group("tetraloop", "tetraloop_h", tfac, false);
    set_aux_from_base_param();
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
  }
  void set_base_from_aux_param() {
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
  }
  void set_base_from_aux_count() {
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
    if (!Alpha::is_base_ncodes(_seq.begin() + (b + 1), _seq.begin() + (e + 1))) return 0;
    ScoreT sc = score_rna_rna_init();
    for (IntT i = (b + 2); i <= e; ++i) {
      IntT cx5 = seq(i - 1);
      IntT cx3 = seq(i);
      IntT cy5 = Alpha::compl_ncode(cx3);
      IntT cy3 = Alpha::compl_ncode(cx5);
      sc += score_stack(cx5, cx3, cy5, cy3);
    }
    return sc;
  }
  ScoreT score_hybridize_dna(IntT b, IntT e) { // 0 based half-open dp cell coordinate
    if (!Alpha::is_base_ncodes(_seq.begin() + (b + 1), _seq.begin() + (e + 1))) return 0;
    ScoreT sc = score_rna_dna_init();
    for (IntT i = (b + 2); i <= e; ++i) {
      IntT cx5 = seq(i - 1);
      IntT cx3 = seq(i);
      IntT cy5 = Alpha::compl_ncode(cx3);
      IntT cy3 = Alpha::compl_ncode(cx5);
      sc += score_stack_rna_dna(cx5, cx3, cy5, cy3);
    }
    return sc;
  }
#define TENSOR(name) _params.tsr_##name()
  ScoreT score_stack(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack).ref(c, c1);
  }
  ScoreT score_stack_enthalpy(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_h).ref(c, c1); // stack -> stack_h
  }
  ScoreT score_stack_rna_dna(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_rna_dna).ref(c, c1);
  }
  ScoreT score_stack_rna_dna_enthalpy(IntT cx5, IntT cx3, IntT cy5, IntT cy3) {
    IntT c  = Alpha::scode(cx5, cy3);
    IntT c1 = Alpha::scode(cy5, cx3); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_rna_dna_h).ref(c, c1); // stack_rna_dna -> stack_rna_dna_h
  }
  ScoreT score_rna_rna_init() { return TENSOR(rna_rna_init).ref();} // entropy ?
  ScoreT score_rna_dna_init() { return TENSOR(rna_dna_init).ref();} // entropy ?
  ScoreT score_rna_rna_init_enthalpy() { return 0.0;} // correct ?
  ScoreT score_rna_dna_init_enthalpy() { return 0.0;} // correct ?
  ScoreT score_stack(IntT i, IntT j) {
    IntT c  = Alpha::scode(seq(i+1), seq(j));
    IntT c1 = Alpha::scode(seq(j-1), seq(i+2)); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack).ref(c, c1);
  }
  ScoreT score_stack_enthalpy(IntT i, IntT j) {
    IntT c  = Alpha::scode(seq(i+1), seq(j));
    IntT c1 = Alpha::scode(seq(j-1), seq(i+2)); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return NEG_INF();
#endif
    return TENSOR(stack_h).ref(c, c1); // stack -> stack_h
  }
  void count_stack(IntT i, IntT j, ScoreT w) {
    IntT c = Alpha::scode(seq(i+1), seq(j));
    IntT c1 = Alpha::scode(seq(j-1), seq(i+2)); // reverse order
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c) && Alpha::is_canonical_scode(c1))) return;
#endif
    TENSOR(stack).ref(c, c1) += w;
  }
  void tsc_nuc_pos_stack(VI& v, IntT i, IntT j) 
  { IntT a[] = {(i+1),(i+2),(j-1),j}; v.assign((IntT*)a, (IntT*)a + 4);}
  ScoreT score_stem_close(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    IntT c  = Alpha::scode(seq(i+1), seq(j));
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    return 0.0;
  }
  ScoreT score_stem_close_enthalpy(IntT i, IntT j) {
#if defined(CHECK_CANONICAL)
    IntT c  = Alpha::scode(seq(i+1), seq(j));
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    return 0.0;
  }
  void count_stem_close(IntT i, IntT j, ScoreT w) {
#if defined(CHECK_CANONICAL)
    IntT c  = Alpha::scode(seq(i+1), seq(j));
    if (!(Alpha::is_canonical_scode(c))) return;
#endif
  } // no-op
  void tsc_nuc_pos_stem_close(VI& v, IntT i, IntT j) { v.clear();}
  ScoreT score_hairpin_nuc(IntT i, IntT j) {
    IntT c  = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    IntT l  = (j-i);
    ScoreT sc = 0;
    if (l <= 4) {
      if (l == 3) {
	sc += TENSOR(triloop).ref(c, c1, seq(i+2));
	sc += TENSOR(terminal_base_pair).ref(c);
      } else {//l == 4
	IntT c2 = Alpha::pcode(seq(i+2), seq(j-1));
	sc += TENSOR(tetraloop).ref(c, c1, c2);
	sc += TENSOR(tstackh).ref(c, c1);
      }
      return sc;
    }
    sc += TENSOR(tstackh).ref(c, c1);
    return sc;
  }
  ScoreT score_hairpin_nuc_enthalpy(IntT i, IntT j) {
    IntT c  = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    IntT l  = (j-i);
    ScoreT sc = 0;
    if (l <= 4) {
      if (l == 3) {
	sc += TENSOR(triloop).ref(c, c1, seq(i+2)); // enthalpy
	sc += TENSOR(terminal_base_pair).ref(c);    // enthalpy
      } else {//l == 4
	IntT c2 = Alpha::pcode(seq(i+2), seq(j-1));
	sc += TENSOR(tetraloop).ref(c, c1, c2);     // enthalpy
	sc += TENSOR(tstack_h).ref(c, c1);          // tstackh -> tstack_h
      }
      return sc;
    }
    sc += TENSOR(tstack_h).ref(c, c1); // tstackh => tstack_h
    return sc;
  }
  bool count_hairpin_nuc(IntT i, IntT j, ScoreT w) {
    IntT c  = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return false;
#endif
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    IntT l  = (j-i);
    if (l <= 4) {
      if (l == 3) {
	TENSOR(triloop).ref(c, c1, seq(i+2)) += w;
	TENSOR(terminal_base_pair).ref(c) += w;
      } else {//l == 4
	IntT c2 = Alpha::pcode(seq(i+2), seq(j-1));
	TENSOR(tetraloop).ref(c, c1, c2) += w;
	TENSOR(tstackh).ref(c, c1) += w;
      }
    }
    TENSOR(tstackh).ref(c, c1) += w;
    return true;
  }
  ScoreT score_hairpin(IntT i, IntT j) {
    IntT l = (j-i);
    ScoreT sc = score_hairpin_nuc(i, j);
    if (impossible(sc)) return NEG_INF();
    if (l <= MAXLOOP) {
      sc += TENSOR(hairpinloop_cumu_change).ref(l);
    } else {
      sc += TENSOR(hairpinloop_cumu_change).ref(MAXLOOP) ;
      sc += log(l / (ScoreT)MAXLOOP) * TENSOR(large_loop_factor).ref();
    }
    return sc;
  }
  ScoreT score_hairpin_enthalpy(IntT i, IntT j) {
    IntT l = (j-i);
    ScoreT sc = score_hairpin_nuc_enthalpy(i, j); // score_hairpin_nuc -> score_hairpin_nuc_enthalpy
    if (impossible(sc)) return NEG_INF();
    if (l <= MAXLOOP) {
      // sc += TENSOR(hairpinloop_cumu_change).ref(l); // entropy
    } else {
      // sc += TENSOR(hairpinloop_cumu_change).ref(MAXLOOP) ; // entropy
      // sc += log(l / (ScoreT)MAXLOOP) * TENSOR(large_loop_factor).ref(); // entropy
    }
    return sc;
  }
  void count_hairpin(IntT i, IntT j, ScoreT w) {
    if (!count_hairpin_nuc(i, j, w)) return;
    IntT l = (j-i);
    if (l <= MAXLOOP) {
      TENSOR(hairpinloop_cumu_change).ref(l) += w;
    } else {
      TENSOR(hairpinloop_cumu_change).ref(MAXLOOP) += w;
      TENSOR(large_loop_factor).ref() += log(l / (ScoreT)MAXLOOP) * w;
    }
  }
  void tsc_nuc_pos_hairpin(VI& v, IntT i, IntT j) {
    IntT l  = (j-i);
    if (l <= 4) {
      if (l == 3) { IntT a[] = {i,(i+1),(i+2),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 5);}
      else { IntT a[] = {i,(i+1),(i+2),(j-1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 5);}
    } else {
      IntT a[] = {i,(i+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 4);
    }
  }
  ScoreT score_interior_nuc(IntT i, IntT j, IntT ip, IntT jp) {
    IntT si  = seq(i);
    IntT si1 = seq(i+1);
    IntT sj  = seq(j);
    IntT sj1 = seq(j+1);
    IntT sip = seq(ip);
    IntT sip1= seq(ip+1);
    IntT sjp = seq(jp);
    IntT sjp1= seq(jp+1);
    IntT c1 = Alpha::scode(si, sj1);
    IntT c3 = Alpha::scode(sjp, sip1);
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c1) && Alpha::is_canonical_scode(c3))) return NEG_INF();
#endif
    IntT li = (ip - i);
    IntT lj = (j - jp);
    ScoreT sc = 0.0;
    if (li == 0 || lj == 0) {// bulge
      if ((li + lj) == 1) {
	// no terminal base pair in alifold
	sc += TENSOR(stack).ref(c1, c3);
      } else {
	sc += TENSOR(terminal_base_pair).ref(c1);
	sc += TENSOR(terminal_base_pair).ref(c3);
	// no dangle in alifold
	// if (lj == 0) {
	//   sc += TENSOR(dangle3).ref(c1, si1);
	//   sc += TENSOR(dangle5).ref(c3, sip);
	// } else {
	//   sc += TENSOR(dangle5).ref(c1, sj);
	//   sc += TENSOR(dangle3).ref(c3, sjp1);
	// }
      }
    } else {// internal
      IntT c2 = Alpha::pcode(si1, sj);
      IntT c4 = Alpha::pcode(sjp1, sip);
      if (li <= 2 && lj <= 2) {
	if ((li + lj) == 2) {
	  sc += TENSOR(int2).ref(c1, si1, sj, c3);
	} else {
	  if (lj == 1) { sc += TENSOR(int21).ref(c1, c2, sip, c3);}
	  else if (li == 1) { sc += TENSOR(int21).ref(c3, c4, sj, c1);}
	  else { sc += TENSOR(int22).ref(c1, c2, c4, c3);}
	}
      } else {
	sc += TENSOR(tstacki).ref(c1, c2);
	sc += TENSOR(tstacki).ref(c3, c4);
      }
    }
    return sc;
  }
  ScoreT score_interior_nuc_enthalpy(IntT i, IntT j, IntT ip, IntT jp) {
    IntT si  = seq(i);
    IntT si1 = seq(i+1);
    IntT sj  = seq(j);
    IntT sj1 = seq(j+1);
    IntT sip = seq(ip);
    IntT sip1= seq(ip+1);
    IntT sjp = seq(jp);
    IntT sjp1= seq(jp+1);
    IntT c1 = Alpha::scode(si, sj1);
    IntT c3 = Alpha::scode(sjp, sip1);
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c1) && Alpha::is_canonical_scode(c3))) return NEG_INF();
#endif
    IntT li = (ip - i);
    IntT lj = (j - jp);
    ScoreT sc = 0.0;
    if (li == 0 || lj == 0) {// bulge
      if ((li + lj) == 1) {
	// no terminal base pair in alifold
	sc += TENSOR(stack_h).ref(c1, c3); // stack -> stack_h
      } else {
	sc += TENSOR(terminal_base_pair).ref(c1); // enthalpy
	sc += TENSOR(terminal_base_pair).ref(c3); // enthalpy
	// no dangle in alifold
	// if (lj == 0) {
	//   sc += TENSOR(dangle3_h).ref(c1, si1);   // dangle3 -> dangle3_h
	//   sc += TENSOR(dangle5_h).ref(c3, sip);   // dangle5 -> dangle5_h
	// } else {
	//   sc += TENSOR(dangle5_h).ref(c1, sj);    // dangle5 -> dangle5_h
	//   sc += TENSOR(dangle3_h).ref(c3, sjp1);  // dangle3 -> dangle3_h
	// }
      }
    } else {// internal
      IntT c2 = Alpha::pcode(si1, sj);
      IntT c4 = Alpha::pcode(sjp1, sip);
      if (li <= 2 && lj <= 2) {
	if ((li + lj) == 2) {
	  sc += TENSOR(int2_h).ref(c1, si1, sj, c3); // int2 -> int2_h
	} else {
	  if (lj == 1) { sc += TENSOR(int21_h).ref(c1, c2, sip, c3);} // int21 -> int21_h
	  else if (li == 1) { sc += TENSOR(int21_h).ref(c3, c4, sj, c1);} // int21 -> int21_h
	  else { sc += TENSOR(int22).ref(c1, c2, c4, c3);} // int22 -> int22_h
	}
      } else {
	sc += TENSOR(tstack_h).ref(c1, c2); // tstacki -> tstack_h
	sc += TENSOR(tstack_h).ref(c3, c4); // tstacki -> tstack_h
      }
    }
    return sc;
  }
  ScoreT score_interior(IntT i, IntT j, IntT ip, IntT jp) {
    ScoreT sc = score_interior_nuc(i, j, ip, jp);
    if (impossible(sc)) return NEG_INF();
    IntT li = (ip - i);
    IntT lj = (j - jp);
    if (li == 0 || lj == 0) {// bulge
      sc += TENSOR(bulgeloop_cumu_change).ref(li + lj);
    } else {// internal
      if (!(li <= 2 && lj <= 2)) {
	sc += TENSOR(internalloop_cumu_change).ref(li + lj);
	sc += TENSOR(internalloop_cumu_asymmetry).ref(abs(li - lj));
      }
    }
    return sc;
  }
  ScoreT score_interior_enthalpy(IntT i, IntT j, IntT ip, IntT jp) {
    ScoreT sc = score_interior_nuc_enthalpy(i, j, ip, jp); // score_interior_nuc -> score_interior_nuc_enthalpy
    if (impossible(sc)) return NEG_INF();
    IntT li = (ip - i);
    IntT lj = (j - jp);
    if (li == 0 || lj == 0) {// bulge
      // sc += TENSOR(bulgeloop_cumu_change).ref(li + lj); // entropy
    } else {// internal
      if (!(li <= 2 && lj <= 2)) {
	// sc += TENSOR(internalloop_cumu_change).ref(li + lj); // entropy
	// sc += TENSOR(internalloop_cumu_asymmetry).ref(abs(li - lj)); // all entropy ? how to treat this ? when max_ninio is reached ?
      }
    }
    return sc;
  }
  bool count_interior_nuc(IntT i, IntT j, IntT ip, IntT jp, ScoreT w) {
    IntT si  = seq(i);
    IntT si1 = seq(i+1);
    IntT sj  = seq(j);
    IntT sj1 = seq(j+1);
    IntT sip = seq(ip);
    IntT sip1= seq(ip+1);
    IntT sjp = seq(jp);
    IntT sjp1= seq(jp+1);
    IntT c1 = Alpha::scode(si, sj1);
    IntT c3 = Alpha::scode(sjp, sip1);
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c1) && Alpha::is_canonical_scode(c3))) return false;
#endif
    IntT li = (ip - i);
    IntT lj = (j - jp);
    if (li == 0 || lj == 0) {// bulge
      if ((li + lj) == 1) {// no terminal base pair in alifold
	TENSOR(stack).ref(c1, c3) += w;
      } else {
	TENSOR(terminal_base_pair).ref(c1) += w;
	TENSOR(terminal_base_pair).ref(c3) += w;
	// no dangle in alifold
	// if (lj == 0) {
	//   TENSOR(dangle3).ref(c1, si1) += w;
	//   TENSOR(dangle5).ref(c3, sip) += w;
	// } else {
	//   TENSOR(dangle5).ref(c1, sj) += w;
	//   TENSOR(dangle3).ref(c3, sjp1) += w;
	// }
      }
    } else {// internal
      IntT c2 = Alpha::pcode(si1, sj);
      IntT c4 = Alpha::pcode(sjp1, sip);
      if (li <= 2 && lj <= 2) {
	if ((li + lj) == 2) {
	  TENSOR(int2).ref(c1, si1, sj, c3) += w;
	} else {
	  if (lj == 1) { TENSOR(int21).ref(c1, c2, sip, c3) += w;}
          else if (li == 1) { TENSOR(int21).ref(c3, c4, sj, c1) += w;}
	  else { TENSOR(int22).ref(c1, c2, c4, c3) += w;}
	}
      } else {
	TENSOR(tstacki).ref(c1, c2) += w;
	TENSOR(tstacki).ref(c3, c4) += w;
      }
    }
    return true;
  }
  void count_interior(IntT i, IntT j, IntT ip, IntT jp, ScoreT w) {
    if (!count_interior_nuc(i, j, ip, jp, w)) return;
    IntT li = (ip - i);
    IntT lj = (j - jp);
    if (li == 0 || lj == 0) {// bulge
      TENSOR(bulgeloop_cumu_change).ref(li + lj) += w;
    } else {// internal
      if (!(li <= 2 && lj <= 2)) {
	TENSOR(internalloop_cumu_change).ref(li + lj) += w;
	TENSOR(internalloop_cumu_asymmetry).ref(abs(li - lj)) += w;
      }
    }
  }
  void tsc_nuc_pos_interior(VI& v, IntT i, IntT j, IntT ip, IntT jp) {
    IntT li = (ip - i);
    IntT lj = (j - jp);
    if (li == 0 || lj == 0) {// bulge
      if ((li + lj) == 1) {// no terminal base pair in alifold
	IntT a[] = {i,(ip+1),jp,(j+1)}; v.assign((IntT*)a, (IntT*)a + 4);
      } else { 	// no dangle in alifold
	IntT a[] = {i,(ip+1),jp,(j+1)}; v.assign((IntT*)a, (IntT*)a + 4);
      }
    } else {// IntTernal
      if (li <= 2 && lj <= 2) {
	if ((li + lj) == 2) {
	  IntT a[] = {i,(i+1),(ip+1),jp,j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 6);
	} else {
	  if (lj == 1) {
	    IntT a[] = {i,(i+1),ip,(ip+1),jp,j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 7);
	  } else if (li == 1) {
	    IntT a[] = {i,(i+1),(ip+1),jp,(jp+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 7);
	  } else {
	    IntT a[] = {i,(i+1),ip,(ip+1),jp,(jp+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 8);
	  }
	}
      } else {
	IntT a[] = {i,(i+1),ip,(ip+1),jp,(jp+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 8);
      }
    }
  }
  ScoreT score_multi_close(IntT i, IntT j) {
    ScoreT sc = score_multi_mismatch(i, j);
    if (impossible(sc)) return NEG_INF();
    sc += TENSOR(multiloop_offset).ref();
    return sc;
  }
  ScoreT score_multi_close_enthalpy(IntT i, IntT j) {
    ScoreT sc = score_multi_mismatch_enthalpy(i, j); // score_multi_mismatch => score_multi_mismatch_enthalpy
    if (impossible(sc)) return NEG_INF();
    // sc += TENSOR(multiloop_offset).ref(); // entropy
    return sc;
  }
  void count_multi_close(IntT i, IntT j, ScoreT w) {
    if (!count_multi_mismatch(i, j, w)) return;
    TENSOR(multiloop_offset).ref() += w;
  }
  void tsc_nuc_pos_multi_close(VI& v, IntT i, IntT j) 
  { IntT a[] = {i,(i+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 4);}
  ScoreT score_multi_open(IntT i, IntT j) {
    ScoreT sc = score_multi_mismatch(j, i);
    if (impossible(sc)) return NEG_INF();
    sc += TENSOR(multiloop_helix).ref();
    return sc;
  }
  ScoreT score_multi_open_enthalpy(IntT i, IntT j) {
    ScoreT sc = score_multi_mismatch_enthalpy(j, i); // score_multi_mismatch => score_multi_mismatch_enthalpy
    if (impossible(sc)) return NEG_INF();
    // sc += TENSOR(multiloop_helix).ref(); // entropy
    return sc;
  }
  void count_multi_open(IntT i, IntT j, ScoreT w) {
    if (!count_multi_mismatch(j, i, w)) return;
    TENSOR(multiloop_helix).ref() += w;
  }
  void tsc_nuc_pos_multi_open(VI& v, IntT i, IntT j)
  { IntT a[] = {i,(i+1),j,(j+1)}; v.assign((IntT*)a, (IntT*)a + 4);}
  ScoreT score_multi_extend(IntT i, IntT j) { return (j - i) * TENSOR(multiloop_nuc).ref();}
  ScoreT score_multi_extend_enthalpy(IntT i, IntT j) { return 0.0;} // entropy
  void count_multi_extend(IntT i, IntT j, ScoreT w)  { TENSOR(multiloop_nuc).ref() += (j - i) * w;}
  ScoreT score_outer_extend(IntT i, IntT j) 
  { return (j - i) * TENSOR(external_unpaired).ref();}
  ScoreT score_outer_extend_enthalpy(IntT i, IntT j) { 
    // return (j - i) * TENSOR(external_unpaired).ref(); // entropy
    return 0.0;
  }
  void count_outer_extend(IntT i, IntT j, ScoreT w) 
  { TENSOR(external_unpaired).ref() += (j - i) * w;}
  ScoreT score_outer_branch_nuc(IntT i, IntT j) {
    IntT c = Alpha::scode(seq(j), seq(i+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif 
    ScoreT sc = 0.0;
    if (0 < i) { sc += TENSOR(dangle5).ref(c, seq(i));}
    if (j < _seqlen) { sc += TENSOR(dangle3).ref(c, seq(j+1));}
    sc += TENSOR(terminal_base_pair).ref(c);
    return sc;
  }
  ScoreT score_outer_branch_nuc_enthalpy(IntT i, IntT j) {
    IntT c = Alpha::scode(seq(j), seq(i+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif 
    ScoreT sc = 0.0;
    if (0 < i) { sc += TENSOR(dangle5_h).ref(c, seq(i));} // dangle5 -> dangle5_h
    if (j < _seqlen) { sc += TENSOR(dangle3_h).ref(c, seq(j+1));} // dangle3 -> dangle3_h
    sc += TENSOR(terminal_base_pair).ref(c); // enthalpy
    return sc;
  } 
  ScoreT score_outer_branch(IntT i, IntT j) {
    ScoreT sc = score_outer_branch_nuc(i, j);
    if (impossible(sc)) return NEG_INF();
    sc += TENSOR(external_paired).ref();
    sc += TENSOR(external_paired_length_factor).ref() * (j-i);
    return sc;
  } 
  ScoreT score_outer_branch_enthalpy(IntT i, IntT j) {
    ScoreT sc = score_outer_branch_nuc_enthalpy(i, j); // score_outer_branch_nuc->score_outer_branch_nuc_enthalpy
    if (impossible(sc)) return NEG_INF();
    sc += TENSOR(external_paired).ref(); // enthalpy
    // sc += TENSOR(external_paired_length_factor).ref() * (j-i); entropy
    return sc;
  } 
  bool count_outer_branch_nuc(IntT i, IntT j, ScoreT w) {
    IntT c = Alpha::scode(seq(j), seq(i+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return false;
#endif
    if (0 < i) { TENSOR(dangle5).ref(c, seq(i)) += w;}
    if (j < _seqlen) { TENSOR(dangle3).ref(c, seq(j+1)) += w;}
    TENSOR(terminal_base_pair).ref(c) += w;
    return true;
  }
  void count_outer_branch(IntT i, IntT j, ScoreT w) {
    if (!count_outer_branch_nuc(i, j, w)) return;
    TENSOR(external_paired).ref() += w;
    TENSOR(external_paired_length_factor).ref() += (j-i) * w;
  }
  void tsc_nuc_pos_outer_branch(VI& v, IntT i, IntT j) {
    v.clear();
    if (0 < i) { v.push_back(i);}
    v.push_back(i + 1);
    v.push_back(j);
    if (j < _seqlen) { v.push_back(j+1);}
  }
  ScoreT score_multi_mismatch(IntT i, IntT j) {
    IntT c = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    ScoreT sc = 0.0;
#if false
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    sc += TENSOR(tstackm).ref(c, c1);
#else
    // alifold case
    sc += TENSOR(dangle3).ref(c, seq(i+1));
    sc += TENSOR(dangle5).ref(c, seq(j));
    sc += TENSOR(terminal_base_pair).ref(c);
#endif
    return sc;
  }
  ScoreT score_multi_mismatch_enthalpy(IntT i, IntT j) {
    IntT c = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return NEG_INF();
#endif
    ScoreT sc = 0.0;
#if false
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    sc += TENSOR(tstack_h).ref(c, c1); // tstackm -> tstack_h
#else
    // alifold case
    sc += TENSOR(dangle3_h).ref(c, seq(i+1)); // dangle3 -> dangle3_h
    sc += TENSOR(dangle5_h).ref(c, seq(j));   // dangle5 -> dangle5_h
    sc += TENSOR(terminal_base_pair).ref(c);  // enthalpy
#endif
    return sc;
  }
  bool count_multi_mismatch(IntT i, IntT j, ScoreT w) {
    IntT c = Alpha::scode(seq(i), seq(j+1));
#if defined(CHECK_CANONICAL)
    if (!(Alpha::is_canonical_scode(c))) return false;
#endif
#if false
    IntT c1 = Alpha::pcode(seq(i+1), seq(j));
    TENSOR(tstackm).ref(c, c1)        += w;
#else
    // alifold case
    TENSOR(dangle3).ref(c, seq(i+1))  += w;
    TENSOR(dangle5).ref(c, seq(j))    += w;
    TENSOR(terminal_base_pair).ref(c) += w;
    return true;
  }
#endif
#undef TENSOR
};
#define ST_(type, layer_in, layer_out, sc_dep_outer, sc_dep_inner) \
{type, layer_in, layer_out, sc_dep_outer, sc_dep_inner, #type},
const Raccess::ScoreModelEnergy::State Raccess::ScoreModelEnergy::STATES[] = {STATES_()};
#undef ST_
#define TR_(type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, emits_nuc) \
{ type, bf, from, to, to1, refer_cur_sc, tsc_dep_nuc, emits_nuc, #type },	     
 const Raccess::ScoreModelEnergy::Transition 
Raccess::ScoreModelEnergy::TRANSITIONS[] = {TRANSITIONS_()};
#undef TR_	
#define PG_(type, idx_type, l0,l1,l2,l3, a0,a1,a2,a3, pg_pair, symmetric, base_idx, sum_bound) \
{type,idx_type,{{l0,l1,l2,l3}},{{a0,a1,a2,a3}},pg_pair,symmetric,base_idx,sum_bound,#type},
const Raccess::ScoreModelEnergy::ParamGroup 
Raccess::ScoreModelEnergy::PARAM_GROUPS[] = {PARAM_GROUPS_()};
#undef PG_
#undef STATES_
#undef TRANSITIONS_
#undef PARAM_GROUPS_
#endif
