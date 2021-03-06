/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include "paddle/fluid/framework/executor.h"
#include "paddle/fluid/inference/analysis/data_flow_graph.h"
#include "paddle/fluid/inference/analysis/fluid_to_data_flow_graph_pass.h"
#include "paddle/fluid/inference/analysis/ut_helper.h"
#include "paddle/fluid/inference/io.h"

namespace paddle {
namespace inference {
namespace analysis {

DEFINE_string(inference_model_dir, "", "inference test model dir");

static framework::proto::ProgramDesc LoadProgramDesc(
    const std::string& model_dir = FLAGS_inference_model_dir) {
  paddle::platform::CPUPlace place;
  paddle::framework::Executor executor(place);
  paddle::framework::Scope scope;
  auto program = Load(&executor, &scope, model_dir);
  return *program->Proto();
}

static DataFlowGraph ProgramDescToDFG(
    const framework::proto::ProgramDesc& desc) {
  DataFlowGraph graph;
  FluidToDataFlowGraphPass pass;
  pass.Initialize(desc);
  pass.Run(&graph);
  pass.Finalize();
  return graph;
}

class DFG_Tester : public ::testing::Test {
 protected:
  void SetUp() override { desc = LoadProgramDesc(FLAGS_inference_model_dir); }

  framework::proto::ProgramDesc desc;
};

}  // namespace analysis
}  // namespace inference
}  // namespace paddle
