/*
 * author: hisanori kiryu
 */
#ifndef UTIL__STICK_BREAK_HPP
#define UTIL__STICK_BREAK_HPP
#include "util/util.hpp"
class StickBreak { // to do hyper param information
public:
  typedef vector<double> V;
  static int pv_size(int prob_size) { return (prob_size - 1);}
  static int beta_size(int pv_size) { return (pv_size + 1);}
  static int prob_size(int pv_size) { return (pv_size + 1);}
  static double beta_from_param(double s) { return (1.0 / (1.0 + exp(-s)));}
  static double beta1_from_param(double s) { return (1.0 / (1.0 + exp(+s)));} // beta1 = (1 - beta)
  static double beta_to_param(double beta) { return log((1 - beta) / beta);}
  static double dbeta_dparam(double beta) { return (beta * (1 - beta));}
  static double dbeta1_dparam(double beta) { return (- (beta * (1 - beta)));}
  static void beta_from_param(V& beta, const V& pv) {
    beta.resize(pv.size() + 1);
    for (int i = 0; i < (int)pv.size(); ++i) { beta[i] = beta_from_param(pv[i]);}
    beta[pv.size()] = 1;
  }
  static void prob_from_beta(V& prob, const V& beta) {
    prob.resize(beta.size());
    double p = 1;
    for (int i = 0; i < (int)beta.size(); ++i) { 
      const double& b = beta[i];
      prob[i] = (p * b);
      p *= (1 - b);
    }
  }
  static void prob_to_beta(V& beta, const V& prob) {
    beta.resize(prob.size());
    double p = 1;
    for (int i = 0; i < (int)beta.size(); ++i) { 
      beta[i] = (prob[i] / p)
      p *= (1 - beta[i]);
    }
  }
  static void prob_from_param(V& prob, const V& pv) {
    prob.resize(pv.size() + 1);
    double p = 1;
    for (int i = 0; i < (int)pv.size(); ++i) { 
      const double& b = beta_from_param(pv[i]);
      prob[i] = (p * b);
      p *= (1 - b);
    }
    prob[pv.size()] = (p * 1);
  }
  static void prob_to_param(V& pv, const V& prob) { // prob should be sorted in descending order.
    Check(prob.size() > 0);
    pv.resize(prob.size() - 1);
    double p = 1;
    for (int i = 0; i < (int)pv.size(); ++i) { 
      const double& b = (prob[i] / p); Check(0 <= b && b <= 1);
      pv[i] = beta_to_param(b);
      p *= (1 - b);
    }
  }
  static double dprob_dparam(const V& prob, const V& beta, int i, int j) {
    Check(prob.size() == beta.size());
    int n = prob.size();
    Check(0 <= i && i < n && 0 <= j && j < (n - 1));
    if (i < j) { return 0;}
    else if (i == j) { return (1 - beta[j]) * prob[j];}
    else { return (beta[j] * prob[j]);}
  }
  static void dprob_to_dparam(V& dpv, const V& dprob, const V& prob, const V& pv) {
    int n = prob.size();
    Check(n > 0 && ((int)pv.size() == (n - 1)) && ((int)dprob.size() == n));
    dpv.resize(n - 1);
    double z = (prob[n - 1] * dprob[n - 1]);
    for (int i = (n - 2); i >= 0; --i) {


      const double& b = beta_from_param(pv[i]);
      const double& d = (prob[i] * dprob[i]);
      dpv[i] = (((1 - b) * d) + (b * z));
      z += d;
    }
  }
};
#endif
