template <typename Iter1, typename Iter2> double cor(Iter1 b1, Iter1 e1, Iter2 b2) {
  if (b1 == e1) return 0;
  double sum_sq_x      = 0;
  double sum_sq_y      = 0;
  double sum_coproduct = 0;
  double mean_x        = (*b1++);
  double mean_y        = (*b2++);
  double n             = 1;
  while (b1 != e1) {
    ++n;
    const double& sweep   = (n - 1) / n;
    const double& x       = (*b1++);
    const double& y       = (*b2++);
    const double& delta_x = (x - mean_x);
    const double& delta_y = (y - mean_y);
    sum_sq_x += delta_x * delta_x * sweep;
    sum_sq_y += delta_y * delta_y * sweep;
    sum_coproduct += delta_x * delta_y * sweep;
    mean_x += delta_x / n;
    mean_y += delta_y / n;
  }
  // const double& pop_sd_x = std::sqrt(sum_sq_x / n);
  // const double& pop_sd_y = std::sqrt(sum_sq_y / n);
  // const double& cov_x_y = sum_coproduct / n;
  // return cov_x_y / (pop_sd_x * pop_sd_y);
  const double& sum_sq_x_y = (sum_sq_x * sum_sq_y);
  return ((sum_sq_x_y > 0) ? (sum_coproduct / sqrt(sum_sq_x_y)) : 0);
}
