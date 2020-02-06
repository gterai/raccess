/*
 * score.hpp
 */
#ifndef SCORE_HPP
#define SCORE_HPP
#define LOG1P
typedef double ScoreT;
inline ScoreT NEG_INF() { return -1.0e100;}
// inline ScoreT NEG_INF() { return -HUGE_VAL;}
inline ScoreT LOG_PROB_THRESHOLD() { return 0.1 * NEG_INF();}
inline bool possible(ScoreT val) { return (val > LOG_PROB_THRESHOLD());}
inline bool impossible(ScoreT val) { return (val <= LOG_PROB_THRESHOLD());}
// inline bool impossible(ScoreT val) { return (isinf(z1) && z1 < 0);}
inline double score_scale() { return 1.0;}
#if defined(LOG1P)
inline void logadd(ScoreT& z, const ScoreT& z1) {
  if (impossible(z1)) return;
  const ScoreT& d = (z - z1);
  if (0 <= d) { z += log1p(std::exp(-d));}
  else { z = z1 + log1p(std::exp(d));}
}
#else
inline void logadd(ScoreT& z, const ScoreT& z1) {
  enum {CUTOFF = 50};
  if (impossible(z1)) return;
  const ScoreT& d = (z - z1);
  if (0 < d) { if (d < CUTOFF) { z += std::log(1 + std::exp(-d));}} 
  else { z = z1; if (-CUTOFF < d) { z += std::log(1 + std::exp(d));}}
}
#endif
#define LOGADD(x, y) ::logadd((x), (y))
#define EXP(x) std::exp(x)
#endif
