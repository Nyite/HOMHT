#pragma once
#include "HOMHT/HOMHT/cluster.h"
#include <unordered_set>
#include <vector>

namespace HOMHT {
class HOMHT_Tracker final
{
  public:
    std::vector<Cluster> clusters;

    void process(const MeasurementVec &measurements);
    void clusterise(const MeasurementVec &measurements);

  private:
    void kalman_predict();
    Eigen::MatrixXi assosiation_matrix_gen(const MeasurementVec &measurements);

    void find_assosiations(const Eigen::MatrixXi &omega,
      Eigen::Index start_row,
      std::unordered_set<Eigen::Index> &merge_list,
      std::unordered_set<Eigen::Index> &meas_list);
    void assosiation_matrix_process(Eigen::MatrixXi &omega);

    void delete_cluster(Eigen::Index c_idx, Eigen::MatrixXi &omega);

    MeasurementVec free_measurements;
};

} // namespace HOMHT
