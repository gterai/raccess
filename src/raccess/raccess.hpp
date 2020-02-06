/*
 * author: hisanori kiryu
 */
#ifndef RACCESS__RACCESS_HPP
#define RACCESS__RACCESS_HPP
#include "util/util.hpp"
#include "raccess/score_model_energy.hpp"
#include "raccess/prob_model.hpp"
namespace Raccess { class Raccess;}
class Raccess::Raccess {
public:
  typedef Raccess          Self;
  typedef ScoreModelEnergy SM;
  typedef ProbModel<SM>    PM;
  typedef SM::IntT         IntT;
  typedef vector<IntT>     VI;
  typedef vector<ScoreT>   V;
  typedef Alpha::CodeT     CodeT;
  typedef Alpha::Codes     Seq;
  class EnergyElem {
  public:
    attr(IntT,    row);
    attr(IntT,    width);
    attr(double, energy);
    EnergyElem(IntT r, IntT w, double e) : _row(r), _width(w), _energy(e) {}
  };
  typedef vector<EnergyElem> ER;
  class PrintEnergyRow {
  public:
    Raccess*       _r;
    ostream*       _fo;
    IntT*          _i0;
    PrintEnergyRow(Raccess& r, ostream& fo, IntT& i0) : _r(&r), _fo(&fo), _i0(&i0) {}
    void init() { (*_i0) = (-1);}
    void finish() { 
      if (_r->table_format()) { 
	// no op
      } else { 
	if (0 <= (*_i0)) { (*_fo) << '\n';}
      }
    }
    void operator()(IntT i, IntT w, double ene) { // zero based sequence coord
      if (_r->table_format()) {
	if ((*_i0) != i) { (*_i0) = i;}
	(*_fo) << i << '\t' << (i + w - 1) << '\t' << ene; // zero based sequence coord
	if (_r->bind_na()) {
	  IntT b = (i + _r->bind_b());
	  IntT e = (i + _r->bind_e());
	  double ene1 = _r->compute_bind_energy(b, e);
	  (*_fo) << '\t' << ene1 << '\t' << (ene + ene1);
	}
	(*_fo) << '\n';
      } else {
	if ((*_i0) != i) {
	  if (0 <= (*_i0)) { (*_fo) << '\n';}
	  (*_i0) = i;
	  // (*_fo) << ((*_i0) + (w / 2)) << '\t'; 
	  (*_fo) << (*_i0) << '\t'; // WARNING: print zero based format
	}
	(*_fo) << w << ',' << ene << ';';
      }
    }
  };
  bool _bind_na;
  IntT _bind_b;
  IntT _bind_e;
  ER   _er;
  Seq  _seq;
  PM   _pm;
  SM   _sm;
  ER& er() { return _er;}
  bool bind_na() const { return _bind_na;}
  IntT bind_b() const { return _bind_b;}
  IntT bind_e() const { return _bind_e;}
  void run() {
    init_model();
    if      (_command == "compute_prob"            ) { compute_prob();            }
    else { Die("bad command %s", _command.c_str());}
  }
  void init_model() { // should call for each setting
    _sm.initialize();
    _sm.set_param("external_unpaired"            , _length_factor);
    _sm.set_param("external_paired_length_factor", _length_factor);
    _pm.set_score_model(_sm);
    _pm.set_max_span(_max_span);
    _pm.set_prob_thr(exp(_sm.energy_to_score(_energy_thr)));
    _pm.set_length_factor(_length_factor);
    _pm.set_acc_lens(_access_len);
    set_bind_be();
  }
  void set_bind_be() {
    if (_bind_range == "none") {
      _bind_na = false;
      _bind_b = 0;
      _bind_e = _access_len[0];
    } else {
      _bind_na = true;
      const vector<IntT>& v = splitvt<IntT>(_bind_range, ",;:"); Check(v.size() == 2);
      _bind_b = v[0]; // 0 based
      _bind_e = v[1];
    }
  }
  template <typename Block> void process_fasta(void (Self::*task)(const Seq&, Block)) {
    ofstream_(fo, _outfile);
    ifstream_(fi, _seqfile);
    if (_table_format && _print_header) { print_table_header(fo);}
    IntT i0;
    PrintEnergyRow out(*this, fo, i0);
    for (FastaIter i = FastaIter(fi); i != FastaIter(); ++i) {
      _seq.resize(i.seq().size());
      Alpha::str_to_ncodes(i.seq().begin(), i.seq().end(), _seq.begin());
      fo << '>';
      if (_bed_header) { print_bed_header(fo, i.name(), i.seq().size());}
      else { fo << i.name() << '\n';}
      out.init();
      (this->*task)(_seq, out);
      out.finish();
      fo << '\n';
    }
  }
  void print_bed_header(ostream& fo, const string& name, const IntT& len) 
  { fo << name << '\t' << 0 << '\t' << len << '\t' << name << '\t' << 0 << '\t' << '+' << '\n';}
  void print_table_header(ostream& fo) {
    fo << "#acc_from\tacc_to\tacc_energy";
    // fo << "#pos\tacc_energy";
    if (_bind_na) { fo << "\tbind_energy\ttot_energy";}
    fo << "\n";
  }
  double compute_bind_energy(IntT b, IntT e) { 
    return _sm.score_to_energy(_bind_dna ?
			       _sm.score_hybridize_dna(b, e) : 
			       _sm.score_hybridize_rna(b, e));
  }
  double compute_bind_energy_rna(IntT b, IntT e)
  { return _sm.score_to_energy(_sm.score_hybridize_rna(b, e));}
  void compute_prob() { process_fasta(&Self::compute_prob<PrintEnergyRow>);}
  template <typename Block> void compute_prob(const Seq& seq, Block block) 
  { _sm.set_seq(seq); _pm.compute_prob(block);}
#define opt_items()  \
  opt_item(help            , bool  , "false", "if true, print this help message and exit.") \
  opt_item(outfile         , string, "raccess_out.txt", "output file") \
  opt_item(command         , string, "compute_prob"   , "only 'compute_prob' available") \
  opt_item(seqfile         , string, "NO FILE"        , "input sequences in fasta format.") \
  opt_item(max_span        , IntT  , "100"            , "maximal span of base pairs considered") \
  opt_item(access_len      , VI    , "10,20,"             , "contiguous length over which the transcript is accesible") \
  opt_item(bind_range      , string, "none"           , "If set in format 'first:last', then for each segment of accessibility computation, the binding energy between the region [first, last] (in 1-based, inclusive-end, coordinate relative to the segment) with a complementary DNA/RNA fragment is calculated.") \
  opt_item(energy_thr      , double, "100"            , "only output the results below the specified energy threshold (unit: kcal/mol)") \
  opt_item_(length_factor   , double, "-0.541728723"   , "") \
  opt_item_(print_header    , bool  , "false"          , "") \
  opt_item_(bed_header      , bool  , "false"          , "") \
  opt_item_(table_format    , bool  , "false"          , "") \
  opt_item_(bind_dna        , bool  , "false"          , "")

#include "util/def_opt_main.hpp"
#undef opt_items 
};
#endif
