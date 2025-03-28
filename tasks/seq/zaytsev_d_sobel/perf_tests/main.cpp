#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "core/task/include/task.hpp"
#include "seq/zaytsev_d_sobel/include/ops_seq.hpp"

namespace {
std::vector<int> GenerateRandomImage(size_t size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 255);

  std::vector<int> image(size);
  for (size_t i = 0; i < size; i++) {
    image[i] = dis(gen);
  }
  return image;
}
}  // namespace

TEST(zaytsev_d_sobel_seq, test_pipeline_run) {
  constexpr int kSize = 4500;

  std::vector<int> in = GenerateRandomImage(kSize * kSize);
  std::vector<int> out(kSize * kSize, 0);
  std::vector<int> size_ = {kSize, kSize};

  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_.data()));
  task_data_seq->inputs_count.emplace_back(in.size());
  task_data_seq->inputs_count.push_back(size_.size());
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  auto test_task_sequential = std::make_shared<zaytsev_d_sobel_seq::TestTaskSequential>(task_data_seq);

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_sequential);
  perf_analyzer->PipelineRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);

  ASSERT_NE(in, out);
}

TEST(zaytsev_d_sobel_seq, test_task_run) {
  constexpr int kSize = 4500;

  std::vector<int> in = GenerateRandomImage(kSize * kSize);
  std::vector<int> out(kSize * kSize, 0);
  std::vector<int> size_ = {kSize, kSize};

  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(size_.data()));
  task_data_seq->inputs_count.emplace_back(in.size());
  task_data_seq->inputs_count.emplace_back(size_.size());
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  auto test_task_sequential = std::make_shared<zaytsev_d_sobel_seq::TestTaskSequential>(task_data_seq);

  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  auto perf_analyzer = std::make_shared<ppc::core::Perf>(test_task_sequential);
  perf_analyzer->TaskRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);

  ASSERT_NE(in, out);
}
