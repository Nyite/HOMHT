#include <Eigen/Eigen>
#include <HOMHT/HOMHT/tracker.h>
#include <iterator>

namespace HOMHT {
[[nodiscard]] Eigen::MatrixXi HOMHT_Tracker::assosiation_matrix_gen(
  const MeasurementVec &measurements)
{
    Eigen::MatrixXi omega = Eigen::MatrixXi::Zero(static_cast<Eigen::Index>(clusters.size() + 1),
      static_cast<Eigen::Index>(measurements.size() + 1));
    for (Eigen::Index i = 1; i < omega.cols(); ++i) omega(0, i) = static_cast<int>(i - 1);
    for (Eigen::Index i = 1; i < omega.rows(); ++i) omega(i, 0) = static_cast<int>(i - 1);

    for (size_t m_i = 0; m_i < measurements.size(); ++m_i) {
        const auto &z = measurements[m_i];
        bool assosiated = false;

        for (size_t c_i = 0; c_i < clusters.size(); ++c_i) {
            if (clusters[c_i].strobe(z)) {
                assosiated = true;
                omega(static_cast<Eigen::Index>(c_i + 1), static_cast<Eigen::Index>(m_i + 1)) = 1;
            }
        }

        if (!assosiated) {
            free_measurements.push_back(z);
        }
    }

    return omega;
}

void HOMHT_Tracker::kalman_predict()
{
    for (auto &cluster : clusters) {
        cluster.predict_hyps();
    }
}

void HOMHT_Tracker::find_assosiations(const Eigen::MatrixXi &omega,
  Eigen::Index start_row,
  std::unordered_set<Eigen::Index> &merge_list,
  std::unordered_set<Eigen::Index> &meas_list)
{
    for (Eigen::Index col_i = 1; col_i < omega.cols(); ++col_i) {
        if (omega(start_row, col_i) == 0) continue;
        meas_list.insert(omega(0, col_i));
        for (Eigen::Index row_i = 1; row_i < omega.rows(); ++row_i) {
            const auto c_indx = omega(row_i, 0);
            if (omega(row_i, col_i) == 0 || merge_list.contains(c_indx)) continue;
            merge_list.insert(c_indx);

            if (row_i == start_row) continue;
            find_assosiations(omega, row_i, merge_list, meas_list);
        }
    }
}

void HOMHT_Tracker::delete_cluster(Eigen::Index c_idx, Eigen::MatrixXi &omega)
{
    homht_ftrace(fg(fmt::color::red), "Deleted {}", clusters[static_cast<size_t>(c_idx)]);
    clusters.erase(clusters.begin() + c_idx);
    for (Eigen::Index row_i = c_idx + 2; row_i < omega.rows(); ++row_i) --omega(row_i, 0);
}

void HOMHT_Tracker::assosiation_matrix_process(Eigen::MatrixXi &omega)
{
    while (omega.rows() > 1) {
        // std::cout << "IN omega\n" << omega << std::endl;

        std::unordered_set<Eigen::Index> clusters_to_merge;
        std::unordered_set<Eigen::Index> common_measurements;

        find_assosiations(omega, 1, clusters_to_merge, common_measurements);
        const auto &c_idx = omega(1, 0);
        auto &origin_cluster = clusters[static_cast<size_t>(c_idx)];

        if (common_measurements.empty()) {
            // cluster did not receive any measurements
            origin_cluster.measurements.miss();
            if (origin_cluster.delete_pending()) {
                delete_cluster(c_idx, omega);
            }
        } else if (clusters_to_merge.size() > 1) {
            // clusters pending to merge
            homht_ftrace(fg(fmt::color::golden_rod), "Merged {}", clusters_to_merge);
            clusters_to_merge.erase(c_idx); // left every other index

            Eigen::Index offset = 0;
            for (const auto merge_idx : clusters_to_merge) {
                /* MERGE OP */
                auto true_index = merge_idx - offset++;
                delete_cluster(true_index, omega);
                removeRow(omega, true_index);
            }

            origin_cluster.measurements = to_vec(common_measurements);
        } else {
            // cluster update
            origin_cluster.measurements = to_vec(common_measurements);
            /* UPDATE OP */
        }

        // std::cout << "OUT omega\n" << omega << std::endl;
        removeRow(omega, 1);
    }
}

void HOMHT_Tracker::clusterise(const MeasurementVec &measurements)
{
    kalman_predict();
    if (measurements.empty()) {
        for (auto &cluster : clusters) cluster.measurements.miss();
    } else {
        auto omega = assosiation_matrix_gen(measurements);
        assosiation_matrix_process(omega);
    }
}
} // namespace HOMHT
