/*
 * author: hisanori kiryu
 */
#ifndef UTIL__ALPHA_HPP
#define UTIL__ALPHA_HPP
class Alpha {
public:
  typedef Alpha         Self;
  typedef unsigned char CodeT;
  typedef unsigned long WordT;
  typedef vector<CodeT> Codes;
  enum NCode {
    N_A   = 0,
    N_C   = 1,
    N_G   = 2,
    N_T   = 3,
    N_N   = 4,
    N_GAP = 5,
    N_UNA = 6,
    N_MSK = 7,
    N_BAD = 8
  };
  enum {
    NNCODE     = (N_T - N_A + 1),
    NNCODEN    = (N_N - N_A + 1),
    NNCODENG   = (N_GAP - N_A + 1),
    NNCODENGUM = (N_MSK - N_A + 1),
    NNCODE0    = (N_BAD - N_A + 1)
  };
  enum {
    BASE_BITS = 2UL,
    BASE_MASK = ~(~0UL << BASE_BITS)
  };
  enum {
    NCODE_BITS = 3UL, // this does not include N_BAD
    NCODE_MASK = ~(~0UL << NCODE_BITS)
  };
  typedef CArray<CodeT, 256>      CharToNCode;
  typedef CArray<char, NNCODE0>   NCodeToChar;
  typedef CArray<string, NNCODE0> NCodeToStr;
  // typedef CMatrix<NCodeMutType, NNCODE0, NNCODE0> Ncode2ToMutType;
  static const CharToNCode        CHAR_TO_NCODE;
  static const CharToNCode        CHAR_TO_NCODE_MASK_LOWERCASE;
  static const NCodeToChar        NCODE_TO_CHAR;
  static const NCodeToStr         NCODE_TO_STR;
  static const NCodeToChar        NCODE_TO_RNA_CHAR;
  static const NCodeToStr         NCODE_TO_RNA_STR;
  static const string             BASE_CHARS;
  static const string             GAP_CHARS;
  static const string             AMB_CHARS;
  static const string             MSK_CHARS;
  static const string             UNA_CHARS;
  static const string             RNA_CHARS;
  static const string             IUPAC_DNA_CHARS;
  // static const Ncode2ToMutType    NCODE2_TO_MUT_TYPE;
  static CodeT ncode(char c) { return CHAR_TO_NCODE[(unsigned char)c];}
  static CodeT ncode_mask_lowercase(char c) 
  { return CHAR_TO_NCODE_MASK_LOWERCASE[(unsigned char)c];}
  static string ncode_to_str(CodeT c) { return NCODE_TO_STR[c];}
  static char ncode_to_char(CodeT c) { return NCODE_TO_CHAR[c];}
  static bool is_base_char(char c) { return is_base_ncode(ncode(c));}
  static bool is_amb_char(char c) { return is_amb_ncode(ncode(c));}
  static bool is_gap_char(char c) { return is_gap_ncode(ncode(c));}
  static bool is_msk_char(char c) { return is_msk_ncode(ncode(c));}
  static bool is_una_char(char c) { return is_una_ncode(ncode(c));}
  static bool is_bad_char(char c) { return is_bad_ncode(ncode(c));}
  static bool is_base_amb_msk_char(char c) { return is_base_amb_msk_ncode(ncode(c));}
  static bool is_gap_una_char(char c) { return is_gap_una_ncode(ncode(c));}
  static bool is_base_ncode(CodeT c) { return (c < N_N);}
  static bool is_base_amb_ncode(CodeT c) { return (c <= N_N);}
  static bool is_base_amb_msk_ncode(CodeT c) { return (c <= N_N || c == N_MSK);}
  static bool is_gap_una_ncode(CodeT c) { return (c == N_GAP || c == N_UNA);}
  static bool is_amb_ncode(CodeT c) { return (c == N_N);}
  static bool is_gap_ncode(CodeT c) { return (c == N_GAP);}
  static bool is_msk_ncode(CodeT c) { return (c == N_MSK);}
  static bool is_una_ncode(CodeT c) { return (c == N_UNA);}
  static bool is_bad_ncode(CodeT c) { return (c == N_BAD);}
  static bool is_purine_ncode(CodeT c) { return (c == N_A || c == N_G);}
  static bool is_pyrimidine_ncode(CodeT c) { return (c == N_C || c == N_T);}
  template <typename Iter> 
  static bool contains_code(Iter b, Iter e, CodeT c) {return (find(b, e, c) != e);}
  template <typename Iter> static bool contains_amb_ncode(Iter b, Iter e) 
  { return contains_code(b, e, Alpha::N_N);}
  template <typename Iter> static bool contains_bad_ncode(Iter b, Iter e) 
  { return (find_if(b, e, ptr_fun(is_bad_ncode)) != e);}
  template <typename Iter> static bool is_base_ncodes(Iter b, Iter e) 
  { return (find_if(b, e, not1(ptr_fun(is_base_ncode))) == e);}
  template <typename Iter> static bool is_base_chars(Iter b, Iter e) 
  { return (find_if(b, e, not1(ptr_fun(is_base_char))) == e);}
  // template <typename Iter> static Codes str_to_ncodes(Iter b, Iter e) 
  // { Codes codes; str_to_ncodes(b, e, back_inserter(codes)); return codes;}
  template <typename Iter1, typename Iter2> 
  static void str_to_ncodes(Iter1 b, Iter1 e, Iter2 result) 
  { transform(b, e, result, ptr_fun(ncode));}
  template <typename V, typename S> static void ncodes_from_str(V& v, const S& s) 
  { v.resize(s.size()); str_to_ncodes(s.begin(), s.end(), v.begin());}
  template <typename Iter> static Codes str_to_ncodes_mask_lowercase(Iter b, Iter e) 
  { Codes codes; str_to_ncodes_mask_lowercase(b, e, back_inserter(codes)); return codes;}
  template <typename Iter1, typename Iter2> 
  static void str_to_ncodes_mask_lowercase(Iter1 b, Iter1 e, Iter2 result) 
  { transform(b, e, result, ptr_fun(ncode_mask_lowercase));}
  template <typename Iter1, typename Iter2> 
  static void ncodes_to_str(Iter1 b, Iter1 e, Iter2 result) 
  { transform(b, e, result, ptr_fun(ncode_to_char));}
  template <typename Iter> static string ncodes_to_str(Iter b, Iter e)
  { string str; for (; b != e; ++b) { str += ncode_to_char(*b);}; return str;}
  static bool match_amb_char_to_base_char(char c, char c1) 
  { return match_amb_char_to_base_ncode(c, ncode(c1));}
  static bool match_amb_char_to_base_ncode(char c, CodeT i) {
    static const bool table[][4] = {
      {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1},                     // ACGT
      {1,1,0,0},{1,0,1,0},{1,0,0,1},{0,1,1,0},{0,1,0,1},{0,0,1,1}, // AC,AG,AT,CG,CT,GT
      {1,1,1,0},{1,1,0,1},{1,0,1,1},{0,1,1,1},                     // ACG,ACT,AGT,CGT
      {1,1,1,1}                                                    // ACGT
    };
    if (!is_base_ncode(i)) return false;
    switch (std::toupper((int)c)) {
    case 'A': return table[ 0][i];
    case 'C': return table[ 1][i];
    case 'G': return table[ 2][i];
    case 'T': case 'U': return table[3][i];
    case 'M': return table[ 4][i]; // M Amino      AC
    case 'R': return table[ 5][i]; // R Purine     AG
    case 'W': return table[ 6][i]; // W Weak       AT
    case 'S': return table[ 7][i]; // S Strong     CG
    case 'Y': return table[ 8][i]; // Y Pyrimidine CT
    case 'K': return table[ 9][i]; // K Keto       GT
    case 'V': return table[10][i]; // V NotT       ACG
    case 'H': return table[11][i]; // H NotG       ACT
    case 'D': return table[12][i]; // D NotC       AGT
    case 'B': return table[13][i]; // B NotA       CGT
    case 'N': case 'X': return table[14][i]; // N Any nucleotide
    default: Die("bad amb char %c", c); return false;
    }
  }
  static const string& amb_char_to_base_chars(char c) {
    static const string table[] = {
      "A","C","G","T",               // A,C,G,T
      "AC","AG","AT","CG","CT","GT", // AC,AG,AT,CG,CT,GT
      "ACG","ACT","AGT","CGT",       // ACG,ACT,AGT,CGT
      "ACGT"                         // ACGT
    };
    switch (std::toupper((int)c)) {
    case 'A': return table[ 0];
    case 'C': return table[ 1];
    case 'G': return table[ 2];
    case 'T': case 'U': return table[3];
    case 'M': return table[ 4]; // M Amino      AC
    case 'R': return table[ 5]; // R Purine     AG
    case 'W': return table[ 6]; // W Weak       AT
    case 'S': return table[ 7]; // S Strong     CG
    case 'Y': return table[ 8]; // Y Pyrimidine CT
    case 'K': return table[ 9]; // K Keto       GT
    case 'V': return table[10]; // V NotT       ACG
    case 'H': return table[11]; // H NotG       ACT
    case 'D': return table[12]; // D NotC       AGT
    case 'B': return table[13]; // B NotA       CGT
    case 'N': case 'X': return table[14]; // N Any nucleotide
    default: Die("bad amb char %c", c); return table[14];
    }
  }
  enum NCodeMutType {
    NOCHANGE     = 0, // A <=> A, C <=> C, G <=> G, T <=> T
    TRANSITION   = 1, // A <=> G, C <=> T
    TRANSVERSION = 2, // A <=> C, A <=> T, G <=> C, G <=> T
    OTHER        = 3,
  };
  static NCodeMutType mut_type(CodeT c1, CodeT c2) {
    NCodeMutType o = NOCHANGE;
    NCodeMutType s = TRANSITION;
    NCodeMutType v = TRANSVERSION;
    NCodeMutType x = OTHER;
    static const NCodeMutType table[NNCODE0][NNCODE0] = { 
      //A  C  G  T  N GAPUNAMSKBAD
      { o, v, s, v, x, x, x, x, x}, // A
      { v, o, v, s, x, x, x, x, x}, // C
      { s, v, o, v, x, x, x, x, x}, // G
      { v, s, v, o, x, x, x, x, x}, // T
      { x, x, x, x, x, x, x, x, x}, // N
      { x, x, x, x, x, x, x, x, x}, // GAP
      { x, x, x, x, x, x, x, x, x}, // UNA
      { x, x, x, x, x, x, x, x, x}, // MSK
      { x, x, x, x, x, x, x, x, x}  // BAD
    };
    return table[c1][c2];
  }
  static bool is_transition(CodeT c1, CodeT c2) { return (mut_type(c1, c2) == TRANSITION);}
  static bool is_transversion(CodeT c1, CodeT c2) { return (mut_type(c1, c2) == TRANSVERSION);}
  template <typename Iter, typename Iter1> 
  static double jc69_distance_ncodes(Iter b, Iter e, Iter1 b1) {
    size_t s = 0;
    size_t n = 0;
    for (; b != e; ++b, ++b1) {
      const CodeT& c  = (*b);
      const CodeT& c1 = (*b1);
      NCodeMutType t = mut_type(c, c1);
      switch (t) {
      case NOCHANGE    : ++n; break;
      case TRANSITION  : ++s; break;
      case TRANSVERSION: ++s; break;
      default: break;
      };
    }
    n += s;
    const double& f = (n > 0 ? (1.0 / (double)n) : 0.0);
    const double& p = (s * f);
    return (-(3.0 / 4.0) * log(1.0 - (4.0 / 3.0) * p));
  }
  template <typename Iter, typename Iter1> 
  static double k80_distance_ncodes(Iter b, Iter e, Iter1 b1) 
  { size_t n; size_t s; size_t v; return k80_distance_ncodes<Iter, Iter1>(b, e, b1, n, s, v);}
  template <typename Iter, typename Iter1> 
  static double k80_distance_ncodes(Iter b, Iter e, Iter1 b1, size_t& n, size_t& s, size_t& v) {
    s = 0;
    v = 0;
    n = 0;
    for (; b != e; ++b, ++b1) {
      const CodeT& c  = (*b);
      const CodeT& c1 = (*b1);
      NCodeMutType t = mut_type(c, c1);
      switch (t) {
      case NOCHANGE    : ++n; break;
      case TRANSITION  : ++s; break;
      case TRANSVERSION: ++v; break;
      default: break;
      };
    }
    n += s;
    n += v;
    const double& f = (n > 0 ? (1.0 / (double)n) : 0.0);
    const double& p = (s * f);
    const double& q = (v * f);
    const double& d = (- (log(1 - 2 * p - q) / 2.0) - (log(1 - 2 * q) / 4.0));
    // cout << n << ',' << s << ',' << v << ',' <<  p << ',' << q << ',' << d << '\n';
    return d;
  }
  static unsigned long word_nbits(unsigned word_size) { return (word_size * BASE_BITS);}
  static unsigned long word_mask(unsigned word_size) { return ~(~0 << word_nbits(word_size));}
  static unsigned long num_words(unsigned word_size) { return (1 << word_nbits(word_size));}
  template <typename T> static WordT str_to_word(const T& t) 
  { return str_to_word(t.begin(), t.end());}
  template <typename Iter> static WordT str_to_word(Iter b, Iter e) {
    WordT w = 0;
    for (; b != e; ++b) {
      const CodeT& c = ncode(*b); Check(is_base_ncode(c));
      w <<= BASE_BITS;
      w |= (WordT)c;
    }
    return w;
  }
  static void word_push_back(WordT& w, CodeT c) { w <<= BASE_BITS; w |= c;}
  static void word_pop_back(WordT& w) { w >>= BASE_BITS;}
  static CodeT word_back(WordT w) { return (w & BASE_MASK);}
  static string word_to_str(WordT w, unsigned n) 
  { string s(n, '?'); word_to_str(w, s.begin(), s.end()); return s;}
  template <typename Iter> static void word_to_str(WordT w, Iter b, Iter e) 
  { while (e != b) { (*--e) = ncode_to_char(w & BASE_MASK); w >>= BASE_BITS;};}
  template <typename Iter> static WordT ncodes_to_word(Iter b, Iter e) {
    WordT w = 0;
    for (; b != e; ++b) {
      const CodeT& c = (*b); Check(is_base_ncode(c));
      w <<= BASE_BITS;
      w |= (WordT)c;
    }
    return w;
  }
  template <typename Iter> static WordT ncodes_to_word_no_check(Iter b, Iter e) 
  { WordT w = 0; for (; b != e; ++b) { w <<= BASE_BITS; w |= (WordT)(*b);}; return w;}
  template <typename Iter> static void word_to_ncodes(WordT w, Iter b, Iter e) 
  { while (e != b) { (*--e) = (w & BASE_MASK); w >>= BASE_BITS;};}
  static string word_to_ncodes(WordT w, unsigned n) 
  { string s(n, '?'); word_to_ncodes(w, s.begin(), s.end()); return s;}
  static CodeT word_at(WordT w, unsigned n, unsigned i) 
  { return (w >> (BASE_BITS * (n - 1 - i))) & BASE_MASK;}
  static WordT word_slice(WordT w, unsigned n, unsigned b, unsigned e) 
  { return ((w >> word_nbits(n - e)) & word_mask(e - b));}
  static string ncode_to_rna_str(CodeT c) { return NCODE_TO_RNA_STR[c];}
  static char ncode_to_rna_char(CodeT c) { return NCODE_TO_RNA_CHAR[c];}
  template <typename Iter> static string ncodes_to_rna_str(Iter b, Iter e) 
  { string str; for (; b != e; ++b) { str += NCODE_TO_RNA_CHAR[*b];}; return str;}
  static CodeT compl_ncode(const CodeT& c) { return (is_base_ncode(c) ? ((~c) & BASE_MASK) : c);}
  static bool is_compl_ncode(const CodeT& c, const CodeT& c1) 
  { return (is_base_ncode(c) && (((~c) & BASE_MASK) == c1));}
  template <typename Iter> static void rev_compl_ncodes(Iter b, Iter e) 
  { std::reverse(b, e); compl_ncodes(b, e);}
  template <typename Iter> static void compl_ncodes(Iter b, Iter e) 
  { std::transform(b, e, b, ptr_fun(&compl_ncode));}
  template <typename Iter> static void rev_compl_str_dna(Iter b, Iter e) 
  { std::reverse(b, e); compl_str_dna(b, e);}
  template <typename Iter> static void compl_str_dna(Iter b, Iter e) 
  { std::transform(b, e, b, ptr_fun(&compl_char_dna));}
  template <typename Iter> static void rev_compl_str_rna(Iter b, Iter e) 
  { std::reverse(b, e); compl_str_rna(b, e);}
  template <typename Iter> static void compl_str_rna(Iter b, Iter e) 
  { std::transform(b, e, b, ptr_fun(&compl_char_rna));}
  static WordT rev_compl_word(WordT w, unsigned n) {
    WordT w1 = 0; while (n--) { 
      w1 <<= (WordT)BASE_BITS; 
      w1 |= (~w & (WordT)BASE_MASK);
      w >>= (WordT)BASE_BITS;
    }
    return w1;
  }
  template <typename Iter> static unsigned long count_nucs_str(Iter b, Iter e) {
    unsigned long n = 0;
    for (; b != e; ++b) { if (is_base_amb_msk_ncode(ncode(*b))) { ++n;}}
    return n;
  }
  template <typename Iter> static unsigned long count_nucs_ncodes(Iter b, Iter e) {
    unsigned long n = 0;
    for (; b != e; ++b) { if (is_base_amb_msk_ncode(*b)) { ++n;}}
    return n;
  }
  template <typename Iter> static unsigned long count_gc_nucs_str(Iter b, Iter e) {
    unsigned long n = 0;
    for (; b != e; ++b) { const CodeT& c = ncode(*b); if (c == N_C || c == N_G) { ++n;}}
    return n;
  }
  template <typename Iter> static unsigned long count_at_nucs_str(Iter b, Iter e) {
    unsigned long n = 0;
    for (; b != e; ++b) { const CodeT& c = ncode(*b); if (c == N_A || c == N_T) { ++n;}}
    return n;
  }
  static char compl_char_dna(char c) {
    switch (c) {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    case 'U': return 'A';
    case 'R': return 'Y';
    case 'Y': return 'R';
      // case 'S': return 'S';
      // case 'W': return 'W';
    case 'M': return 'K';
    case 'K': return 'M';
    case 'B': return 'V';
    case 'V': return 'B';
    case 'D': return 'H';
    case 'H': return 'D';
      // case 'N': return 'N';
      // case 'X': return 'X';
    case 'a': return 't';
    case 'c': return 'g';
    case 'g': return 'c';
    case 't': return 'a';
    case 'u': return 'a';
    case 'r': return 'y';
    case 'y': return 'r';
      // case 's': return 's';
      // case 'w': return 'w';
    case 'm': return 'k';
    case 'k': return 'm';
    case 'b': return 'v';
    case 'v': return 'b';
    case 'd': return 'h';
    case 'h': return 'd';
      // case 'n': return 'n';
      // case 'x': return 'x';
    default: return c;
    }
  }
  static char compl_char_rna(char c) {
    switch (c) {
    case 'A': return 'U';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    case 'U': return 'A';
    case 'R': return 'Y';
    case 'Y': return 'R';
      // case 'S': return 'S';
      // case 'W': return 'W';
    case 'M': return 'K';
    case 'K': return 'M';
    case 'B': return 'V';
    case 'V': return 'B';
    case 'D': return 'H';
    case 'H': return 'D';
      // case 'N': return 'N';
      // case 'X': return 'X';
    case 'a': return 'u';
    case 'c': return 'g';
    case 'g': return 'c';
    case 't': return 'a';
    case 'u': return 'a';
    case 'r': return 'y';
    case 'y': return 'r';
      // case 's': return 's';
      // case 'w': return 'w';
    case 'm': return 'k';
    case 'k': return 'm';
    case 'b': return 'v';
    case 'v': return 'b';
    case 'd': return 'h';
    case 'h': return 'd';
      // case 'n': return 'n';
      // case 'x': return 'x';
    default: return c;
    }
  }

  // TODO:
  // PCode Scode do not account for una, mask codes.
  enum PCode {
    P_AA,
    P_AC,
    P_AG,
    P_AT,
    P_CA,
    P_CC,
    P_CG,
    P_CT,
    P_GA,
    P_GC,
    P_GG,
    P_GT,
    P_TA,
    P_TC,
    P_TG,
    P_TT,
    P_NN,
    P_GAP,
    P_BAD
  };
  enum {
    NPCODE   = ((unsigned long)P_TT - P_AA + 1),
    NPCODEN  = ((unsigned long)P_NN - P_AA + 1),
    NPCODENG = ((unsigned long)P_GAP - P_AA + 1),
    NPCODE0  = ((unsigned long)P_BAD - P_AA + 1)
  };
  typedef CArray<CodeT, 2>                        NCode2;
  typedef CArray<string, NPCODE0>                 PCodeToStr;
  typedef CArray<CArray<CodeT, NNCODE0>, NNCODE0> NCode2ToPCode;
  typedef CArray<NCode2, NPCODE0>                 PCodeToNCode2;
  typedef CArray<CodeT, NPCODE0>                  FlipPCode;
  static const PCodeToStr                         PCODE_TO_STR;
  static const PCodeToStr                         PCODE_TO_RNA_STR;
  static const NCode2ToPCode                      NCODE2_TO_PCODE;
  static const PCodeToNCode2                      PCODE_TO_NCODE2;
  static const FlipPCode                          FLIP_PCODE;
  enum SCode {
    S_AT,
    S_CG,
    S_GC,
    S_GT,
    S_TA,
    S_TG,
    S_MM,
    S_NN,
    S_GAP,
    S_BAD
  };
  enum {
    NSCODE   = ((unsigned long)S_MM - S_AT + 1),
    NSCODEN  = ((unsigned long)S_NN - S_AT + 1),
    NSCODENG = ((unsigned long)S_GAP - S_AT + 1),
    NSCODE0  = ((unsigned long)S_BAD - S_AT + 1),
    N_CANONICAL_SCODE = ((unsigned long)S_TG - S_AT + 1)
  };
  typedef CArray<string, NSCODE0>                 SCodeToStr;
  typedef CArray<CArray<CodeT, NNCODE0>, NNCODE0> NCode2ToSCode;
  typedef CArray<NCode2, NSCODE0>                 SCodeToNCode2;
  typedef CArray<CodeT, NSCODE0>                  FlipSCode; 
  typedef CArray<CodeT, NPCODE0>                  PCodeToSCode;
  static const SCodeToStr                         SCODE_TO_STR;
  static const SCodeToStr                         SCODE_TO_RNA_STR;
  static const NCode2ToSCode                      NCODE2_TO_SCODE;
  static const SCodeToNCode2                      SCODE_TO_NCODE2;
  static const FlipSCode                          FLIP_SCODE;
  static const PCodeToSCode                       PCODE_TO_SCODE;
  static CodeT pcode(CodeT c, CodeT c1) { return NCODE2_TO_PCODE[c][c1];}
  static string pcode_to_str(CodeT c) { return PCODE_TO_STR[c];}
  static string pcode_to_rna_str(CodeT c) { return PCODE_TO_RNA_STR[c];}
  static std::pair<CodeT, CodeT> split_pcode(CodeT c) 
  { const NCode2& a = PCODE_TO_NCODE2[c]; return std::make_pair(a[0], a[1]);}
  static CodeT flip_pcode(CodeT c) { return FLIP_PCODE[c];}
  static CodeT scode(CodeT c, CodeT c1) { return NCODE2_TO_SCODE[c][c1];}
  static string scode_to_str(CodeT c) { return SCODE_TO_STR[c];}
  static string scode_to_rna_str(CodeT c) { return SCODE_TO_RNA_STR[c];}
  static std::pair<CodeT, CodeT> split_scode(CodeT c) 
  { const NCode2& a = SCODE_TO_NCODE2[c]; return std::make_pair(a[0], a[1]);}
  static CodeT flip_scode(CodeT c) { return FLIP_SCODE[c];}
  static CodeT pcode_to_scode(CodeT c) { return PCODE_TO_SCODE[c];}
  static bool is_canonical_scode(CodeT c) { return (c < S_MM);}
  static bool is_canonical_pcode(CodeT c) { return is_canonical_scode(pcode_to_scode(c));}
  static bool is_canonical(CodeT c, CodeT c1) { return is_canonical_scode(scode(c, c1));}
  static bool is_canonical_chars(char c, char c1) { return is_canonical(ncode(c), ncode(c1));}

  template <typename Iter> static void compl_pcodes(Iter b, Iter e) {

  }

  enum ACode {
    A_A   =  0,
    A_C   =  1,
    A_D   =  2,
    A_E   =  3,
    A_F   =  4,
    A_G   =  5,
    A_H   =  6,
    A_I   =  7,
    A_K   =  8,
    A_L   =  9,
    A_M   = 10,
    A_N   = 11,
    A_P   = 12,
    A_Q   = 13,
    A_R   = 14,
    A_S   = 15,
    A_T   = 16,
    A_V   = 17,
    A_W   = 18,
    A_Y   = 19,
    A_X   = 20,
    A_GAP = 21,
    A_STO = 22,
    A_UNA = 23,
    A_MSK = 24,
    A_BAD = 25
  };
  enum {
    NACODE  = (A_Y - A_A + 1),
    NACODEX = (A_X - A_A + 1),
    NACODE0 = (A_BAD - A_A + 1)
  };

  enum CCode {
    C_AAA =  0,
    C_AAC =  1,
    C_AAG =  2,
    C_AAT =  3,

    C_ACA =  4,
    C_ACC =  5,
    C_ACG =  6,
    C_ACT =  7,

    C_AGA =  8,
    C_AGC =  9,
    C_AGG = 10,
    C_AGT = 11,

    C_ATA = 12,
    C_ATC = 13,
    C_ATG = 14,
    C_ATT = 15,

    C_CAA = 16,
    C_CAC = 17,
    C_CAG = 18,
    C_CAT = 19,

    C_CCA = 20,
    C_CCC = 21,
    C_CCG = 22,
    C_CCT = 23,

    C_CGA = 24,
    C_CGC = 25,
    C_CGG = 26,
    C_CGT = 27,

    C_CTA = 28,
    C_CTC = 29,
    C_CTG = 30,
    C_CTT = 31,

    C_GAA = 32,
    C_GAC = 33,
    C_GAG = 34,
    C_GAT = 35,

    C_GCA = 36,
    C_GCC = 37,
    C_GCG = 38,
    C_GCT = 39,

    C_GGA = 40,
    C_GGC = 41,
    C_GGG = 42,
    C_GGT = 43,

    C_GTA = 44,
    C_GTC = 45,
    C_GTG = 46,
    C_GTT = 47,

    C_TAA = 48,
    C_TAC = 49,
    C_TAG = 50,
    C_TAT = 51,

    C_TCA = 52,
    C_TCC = 53,
    C_TCG = 54,
    C_TCT = 55,

    C_TGA = 56,
    C_TGC = 57,
    C_TGG = 58,
    C_TGT = 59,

    C_TTA = 60,
    C_TTC = 61,
    C_TTG = 62,
    C_TTT = 63,
  
    C_NNN = 64,
    C_GAP = 65,
    C_UNA = 66,
    C_MSK = 67,
    C_BAD = 68
  };
  enum {
    NCCODE     = (C_TTT - C_AAA + 1),
    NCCODEN    = (C_NNN - C_AAA + 1),
    NCCODENG   = (C_GAP - C_AAA + 1),
    NCCODENGUM = (C_MSK - C_AAA + 1),
    NCCODE0    = (C_BAD - C_AAA + 1)
  };
  enum {
    CODON_BITS = 6UL,
    CODON_MASK = ~(~0UL << CODON_BITS)
  };
  typedef CArray<CodeT, 256>      CharToACode;
  typedef CArray<CodeT, NCCODE0>  CCodeToACode;
  typedef string                  ACodeToChar;
  typedef CArray<string, NCCODE0> CCodeToStr;

  static CharToACode  CHAR_TO_ACODE;
  static CharToACode  CHAR_TO_ACODE_MSK_LOWERCASE;
  static CCodeToACode CCODE_TO_ACODE;
  static ACodeToChar  ACODE_TO_CHAR;
  static CCodeToStr   CCODE_TO_STR;
  static CodeT ncode3_to_ccode(CodeT c0, CodeT c1, CodeT c2) {
    if (is_base_ncode(c0) && is_base_ncode(c1) && is_base_ncode(c2)) {
      return ((c0 << 2 * BASE_BITS) | (c1 << BASE_BITS) | c2);
    } else if (is_base_amb_ncode(c0) && is_base_amb_ncode(c1) && is_base_amb_ncode(c2)) {
      return C_NNN;
    } else if (is_gap_ncode(c0) && is_gap_ncode(c1) && is_gap_ncode(c2)) {
      return C_GAP;
    } else if (is_una_ncode(c0) && is_una_ncode(c1) && is_una_ncode(c2)) {
      return C_UNA;
    } else if (is_msk_ncode(c0) && is_msk_ncode(c1) && is_msk_ncode(c2)) {
      return C_MSK;
    } else {
      return C_BAD;
    }
  }
  static CodeT ccode_to_acode(CodeT c) { return CCODE_TO_ACODE[c];}
  static Codes acode_to_ccodes(CodeT c) {
    Codes codes;
    for (CodeT c1 = 0; c1 < NCCODE0; ++c1) { if (c == ccode_to_acode(c1)) { codes.push_back(c1);}}
    return codes; // should create table
  }
  static CodeT char_to_acode(char c) { return CHAR_TO_ACODE[(unsigned char)c];}
  static char acode_to_char(CodeT c) { return ACODE_TO_CHAR[c];}
  static bool is_amino_acode(CodeT c) { return (c <= A_Y);}
  static bool is_stop_acode(CodeT c) { return (c == A_STO);}
  static bool is_gap_acode(CodeT c) { return (c == A_GAP);}
  static bool is_bad_acode(CodeT c) { return (c == A_BAD);}
  static bool is_amb_acode(CodeT c) { return (c == A_X);}
  static bool is_stop_char(char c) { return is_stop_acode(char_to_acode(c));}
  template <typename Iter1, typename Iter2> 
  static void str_to_acodes(Iter1 b, Iter1 e, Iter2 result) 
  { transform(b, e, result, ptr_fun(char_to_acode));}
  template <typename Iter1, typename Iter2> 
  static void acodes_to_str(Iter1 b, Iter1 e, Iter2 result) 
  { transform(b, e, result, ptr_fun(acode_to_char));}
  template <typename Iter1, typename Iter2> 
  static void ncodes_to_acodes(Iter1 b, Iter1 e, Iter2 result)  {
    for (; b != e;) {
      CodeT c0 = (*b++);
      if (b == e) break;
      CodeT c1 = (*b++);
      if (b == e) break;
      CodeT c2 = (*b++);
      (*result++) = ccode_to_acode(ncode3_to_ccode(c0, c1, c2));
    }
  }
  template <typename Iter1, typename Iter2> 
  static void translate_str(Iter1 b, Iter1 e, Iter2 result)  {
    for (; b != e;) {
      CodeT c0 = ncode(*b); ++b;
      if (b == e) break;
      CodeT c1 = ncode(*b); ++b;
      if (b == e) break;
      CodeT c2 = ncode(*b); ++b;
      CodeT c  = ncode3_to_ccode(c0, c1, c2);
      CodeT a  = ccode_to_acode(c);
      (*result) = acode_to_char(a); ++result;
    }
  }
};

/* base codes
 * a = A = N_A = 0, c = C = N_C = 1, g = G = N_G = 2, t = T = u = U = N_T = 3
 * N R Y M K S W H B V D and X (and n r y m k s w h b v d and x) as ambiguous chars (= N_N = 4)
 * '-' is regarded as gap chars (= N_GAP = 5)
 * '.' is unaligned char (= N_UNK = 6)
 *  no  mask chars (= N_MSK = 7)
 * other characters are invalid (= N_BAD = 8)
 */
const Alpha::CharToNCode Alpha::CHAR_TO_NCODE = {{
#define A N_A
#define C N_C
#define G N_G
#define T N_T
#define N N_N
#define g N_GAP
#define u N_UNA
#define m N_MSK
#define _ N_BAD
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, g, u, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, A, N, C, N, _, _, G, N, _, _, N, _, N, N, _,
 _, _, N, N, T, T, N, N, N, N, _, _, _, _, _, _,
 _, A, N, C, N, _, _, G, N, _, _, N, _, N, N, _,
 _, _, N, N, T, T, N, N, N, N, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
#undef A
#undef C
#undef G
#undef T
#undef N
#undef g
#undef u
#undef m
#undef _
}};

const Alpha::CharToNCode Alpha::CHAR_TO_NCODE_MASK_LOWERCASE = {{
#define A N_A
#define C N_C
#define G N_G
#define T N_T
#define N N_N
#define g N_GAP
#define u N_UNA
#define m N_MSK
#define _ N_BAD
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, g, u, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, A, N, C, N, _, _, G, N, _, _, N, _, N, N, _,
 _, _, N, N, T, T, N, N, N, N, _, _, _, _, _, _,
 _, m, m, m, m, _, _, m, m, _, _, m, _, m, m, _,
 _, _, m, m, m, m, m, m, m, m, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
 _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
#undef A
#undef C
#undef G
#undef T
#undef N
#undef g
#undef u
#undef m
#undef _
}};
const Alpha::NCodeToChar Alpha::NCODE_TO_CHAR = {{
  'A', 'C', 'G', 'T', 'N', '-', '.', 'n', '?'
}};
const Alpha::NCodeToStr Alpha::NCODE_TO_STR = {{
  "A", "C", "G", "T", "N", "-", ".", "n", "?"
}};
const string Alpha::BASE_CHARS = "ACGTacgt";
const string Alpha::AMB_CHARS = "NRYMKSWHBVDX";
const string Alpha::GAP_CHARS = "-";
const string Alpha::UNA_CHARS = ".";
const string Alpha::MSK_CHARS = "nrymkswhbvdx";
const string Alpha::IUPAC_DNA_CHARS = "ACGTMRWSYKVHDBN";



const string Alpha::RNA_CHARS = "ACGU";
const Alpha::NCodeToChar Alpha::NCODE_TO_RNA_CHAR = {{
  'A', 'C', 'G', 'U', 'N', '-', '.', 'n', '?'
}};
const Alpha::NCodeToStr Alpha::NCODE_TO_RNA_STR = {{
  "A", "C", "G", "U", "N", "-", ".", "n", "?"
}};

const Alpha::PCodeToStr Alpha::PCODE_TO_STR = {{
  "AA", "AC", "AG", "AT",
  "CA", "CC", "CG", "CT",
  "GA", "GC", "GG", "GT",
  "TA", "TC", "TG", "TT",
  "NN", "..", "??"
}};
const Alpha::PCodeToStr Alpha::PCODE_TO_RNA_STR = {{
  "AA", "AC", "AG", "AU",
  "CA", "CC", "CG", "CU",
  "GA", "GC", "GG", "GU",
  "UA", "UC", "UG", "UU",
  "NN", "..", "??"
}};

// should account for una, msk codes
const Alpha::NCode2ToPCode Alpha::NCODE2_TO_PCODE = {{
{{P_AA, P_AC, P_AG, P_AT, P_NN, P_GAP, P_BAD}},
{{P_CA, P_CC, P_CG, P_CT, P_NN, P_GAP, P_BAD}},
{{P_GA, P_GC, P_GG, P_GT, P_NN, P_GAP, P_BAD}},
{{P_TA, P_TC, P_TG, P_TT, P_NN, P_GAP, P_BAD}},
{{P_NN, P_NN, P_NN, P_NN, P_NN, P_GAP, P_BAD}},
{{P_GAP,P_GAP,P_GAP,P_GAP,P_GAP,P_GAP, P_BAD}},
{{P_BAD,P_BAD,P_BAD,P_BAD,P_BAD,P_BAD, P_BAD}}
}};
const Alpha::PCodeToNCode2 Alpha::PCODE_TO_NCODE2 = {{
  {{N_A, N_A}}, {{N_A, N_C}}, {{N_A, N_G}}, {{N_A, N_T}},
  {{N_C, N_A}}, {{N_C, N_C}}, {{N_C, N_G}}, {{N_C, N_T}},
  {{N_G, N_A}}, {{N_G, N_C}}, {{N_G, N_G}}, {{N_G, N_T}},
  {{N_T, N_A}}, {{N_T, N_C}}, {{N_T, N_G}}, {{N_T, N_T}},
  {{N_N, N_N}}, {{N_GAP, N_GAP}}, {{N_BAD, N_BAD}}
}};
const Alpha::FlipPCode  Alpha::FLIP_PCODE = {{
  P_AA, P_CA, P_GA, P_TA,
  P_AC, P_CC, P_GC, P_TC,
  P_AG, P_CG, P_GG, P_TG,
  P_AT, P_CT, P_GT, P_TT,
  P_NN, P_GAP,P_BAD
}};

const Alpha::SCodeToStr  Alpha::SCODE_TO_STR = {{
  "AT", "CG", "GC", "GT", "TA", "TG", "MM", "NN", "..", "??"
}};
const Alpha::SCodeToStr  Alpha::SCODE_TO_RNA_STR = {{
  "AU", "CG", "GC", "GU", "UA", "UG", "MM", "NN", "..", "??"
}};
const Alpha::NCode2ToSCode Alpha::NCODE2_TO_SCODE = {{
  {{S_MM, S_MM, S_MM, S_AT, S_NN, S_GAP, S_BAD}},
  {{S_MM, S_MM, S_CG, S_MM, S_NN, S_GAP, S_BAD}},
  {{S_MM, S_GC, S_MM, S_GT, S_NN, S_GAP, S_BAD}},
  {{S_TA, S_MM, S_TG, S_MM, S_NN, S_GAP, S_BAD}},
  {{S_NN, S_NN, S_NN, S_NN, S_NN, S_GAP, S_BAD}},
  {{S_GAP,S_GAP,S_GAP,S_GAP,S_GAP,S_GAP, S_BAD}},
  {{S_BAD,S_BAD,S_BAD,S_BAD,S_BAD,S_BAD, S_BAD}}
}};
const Alpha::SCodeToNCode2 Alpha::SCODE_TO_NCODE2 = {{
  {{N_A, N_T}}, {{N_C, N_G}}, {{N_G, N_C}}, {{N_G, N_T}},
  {{N_T, N_A}}, {{N_T, N_G}}, {{N_N, N_N}}, {{N_N, N_N}},
  {{N_GAP, N_GAP}}, {{N_BAD, N_BAD}}
}};
const Alpha::FlipSCode Alpha::FLIP_SCODE = {{
  S_TA, S_GC, S_CG, S_TG, S_AT, S_GT,
  S_MM, S_NN, S_GAP,S_BAD
}};
const Alpha::PCodeToSCode Alpha::PCODE_TO_SCODE = {{
  S_MM, S_MM, S_MM, S_AT,
  S_MM, S_MM, S_CG, S_MM,
  S_MM, S_GC, S_MM, S_GT,
  S_TA, S_MM, S_TG, S_MM,
  S_NN, S_GAP,S_BAD
}};

Alpha::CharToACode Alpha::CHAR_TO_ACODE = {{ // stop=$,* // gap - // una .
#define g A_GAP
#define s A_STO
#define u A_UNA
#define m A_MSK
#define _ A_BAD
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //   0 -  15
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //  16 -  31
   _,   _,   _,   _,   s,   _,   _,   _,   _,   _,   _,   s,   _,   g,   u,   _, //  32 -  47
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //  48 -  63
   _, A_A,   _, A_C, A_D, A_E, A_F, A_G, A_H, A_I,   _, A_K, A_L, A_M, A_N,   _, //  64 -  79
 A_P, A_Q, A_R, A_S, A_T,   _, A_V, A_W, A_X, A_Y,   _,   _,   _,   _,   _,   _, //  80 -  95
   _, A_A,   _, A_C, A_D, A_E, A_F, A_G, A_H, A_I,   _, A_K, A_L, A_M, A_N,   _, //  96 - 111
 A_P, A_Q, A_R, A_S, A_T,   _, A_V, A_W, A_X, A_Y,   _,   _,   _,   _,   _,   _, // 112 - 127
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, // 128 - 143
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
#undef g
#undef s
#undef u
#undef m
#undef _
}};

Alpha::CharToACode Alpha::CHAR_TO_ACODE_MSK_LOWERCASE = {{ // stop=$,*, gap=- una=.
#define g A_GAP
#define s A_STO
#define u A_UNA
#define m A_MSK
#define _ A_BAD
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //   0 -  15
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //  16 -  31
   _,   _,   _,   _,   s,   _,   _,   _,   _,   _,   _,   s,   _,   g,   u,   _, //  32 -  47
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, //  48 -  63
   _, A_A,   _, A_C, A_D, A_E, A_F, A_G, A_H, A_I,   _, A_K, A_L, A_M, A_N,   _, //  64 -  79
 A_P, A_Q, A_R, A_S, A_T,   _, A_V, A_W, A_X, A_Y,   _,   _,   _,   _,   _,   _, //  80 -  95
   _,   m,   _,   m,   m,   m,   m,   m,   m,   m,   _,   m,   m,   m,   m,   _, //  96 - 111
   m,   m,   m,   m,   m,   _,   m,   m,   m,   m,   _,   _,   _,   _,   _,   _, // 112 - 127
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, // 128 - 143
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
#undef g
#undef s
#undef u
#undef m
#undef _
}};

Alpha::CCodeToACode Alpha::CCODE_TO_ACODE = {{
    A_K, // C_AAA,
    A_N, // C_AAC,
    A_K, // C_AAG,
    A_N, // C_AAT,
 
    A_T, // C_ACA,
    A_T, // C_ACC,
    A_T, // C_ACG,
    A_T, // C_ACT,
 
    A_R, // C_AGA,
    A_S, // C_AGC,
    A_R, // C_AGG,
    A_S, // C_AGT,
 
    A_I, // C_ATA,
    A_I, // C_ATC,
    A_M, // C_ATG,
    A_I, // C_ATT,
 
    A_Q, // C_CAA,
    A_H, // C_CAC,
    A_Q, // C_CAG,
    A_H, // C_CAT,
 
    A_P, // C_CCA,
    A_P, // C_CCC,
    A_P, // C_CCG,
    A_P, // C_CCT,
 
    A_R, // C_CGA,
    A_R, // C_CGC,
    A_R, // C_CGG,
    A_R, // C_CGT,
 
    A_L, // C_CTA,
    A_L, // C_CTC,
    A_L, // C_CTG,
    A_L, // C_CTT,
 
    A_E, // C_GAA,
    A_D, // C_GAC,
    A_E, // C_GAG,
    A_D, // C_GAT,

    A_A, // C_GCA,
    A_A, // C_GCC,
    A_A, // C_GCG,
    A_A, // C_GCT,

    A_G, // C_GGA,
    A_G, // C_GGC,
    A_G, // C_GGG,
    A_G, // C_GGT,
 
    A_V, // C_GTA,
    A_V, // C_GTC,
    A_V, // C_GTG,
    A_V, // C_GTT,
 
    A_STO, // C_TAA,  ochre
    A_Y,   // C_TAC,
    A_STO, // C_TAG,  amber
    A_Y,   // C_TAT,
 
    A_S, // C_TCA,
    A_S, // C_TCC,
    A_S, // C_TCG,
    A_S, // C_TCT,
 
    A_STO, // C_TGA,  opal,umber
    A_C,   // C_TGC,
    A_W,   // C_TGG,
    A_C,   // C_TGT,

    A_L, // C_TTA,
    A_F, // C_TTC,
    A_L, // C_TTG,
    A_F, // C_TTT,
 
    A_X,   // C_NNN,
    A_GAP, // C_GAP,
    A_UNA, // C_UNA,
    A_MSK, // C_MSK,
    A_BAD, // C_BAD
}};

Alpha::ACodeToChar Alpha::ACODE_TO_CHAR = "ACDEFGHIKLMNPQRSTVWYX-$.x?";
Alpha::CCodeToStr Alpha::CCODE_TO_STR = {{
    "AAA",
    "AAC",
    "AAG",
    "AAT",

    "ACA",
    "ACC",
    "ACG",
    "ACT",

    "AGA",
    "AGC",
    "AGG",
    "AGT",

    "ATA",
    "ATC",
    "ATG",
    "ATT",

    "CAA",
    "CAC",
    "CAG",
    "CAT",

    "CCA",
    "CCC",
    "CCG",
    "CCT",

    "CGA",
    "CGC",
    "CGG",
    "CGT",

    "CTA",
    "CTC",
    "CTG",
    "CTT",

    "GAA",
    "GAC",
    "GAG",
    "GAT",

    "GCA",
    "GCC",
    "GCG",
    "GCT",
 
    "GGA",
    "GGC",
    "GGG",
    "GGT",

    "GTA",
    "GTC",
    "GTG",
    "GTT",

    "TAA",
    "TAC",
    "TAG",
    "TAT",

    "TCA",
    "TCC",
    "TCG",
    "TCT",
 
    "TGA",
    "TGC",
    "TGG",
    "TGT",

    "TTA",
    "TTC",
    "TTG",
    "TTT",

    "NNN",
    "---",
    "...",
    "nnn",
    "???"
}};
#endif
