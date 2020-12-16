/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */


#include <gtest/gtest.h>
#include <tvm/relay/dataflow_pattern.h>
#include <tvm/tir/analysis.h>

TEST(DFPattern, IsVar) {
  using namespace tvm;
  using namespace tvm::relay;
  auto pattern = IsVar("add");
  auto* node = pattern.as<VarPatternNode>();
  ICHECK(node);
  ICHECK(node->name == String("add"));
}

TEST(DFPattern, IsConstant) {
  using namespace tvm;
  using namespace tvm::relay;
  auto pattern = IsConstant();
  auto* node = pattern.as<ConstantPatternNode>();
  ICHECK(node);
}

TEST(DFPattern, IsOp) {
  using namespace tvm;
  using namespace tvm::relay;
  auto pattern = IsOp("add");
  auto* node = pattern.as<ExprPatternNode>();
  ICHECK(node);
  ICHECK(node->expr == Op::Get("add"));
}

TEST(DFPattern, IsTuple) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = IsTuple({a, b});
  auto* node = pattern.as<TuplePatternNode>();
  ICHECK(node);
  ICHECK(node->fields[0] == a);
  ICHECK(node->fields[1] == b);
}

TEST(DFPattern, IsTupleGetItem) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto tuple = IsTuple({a, b});
  auto pattern = IsTupleGetItem(tuple, 1);
  auto* node = pattern.as<TupleGetItemPatternNode>();
  ICHECK(node);
  ICHECK(node->tuple == tuple);
  ICHECK(node->index == 1);
}

TEST(DFPattern, ADD) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = a + b;
  auto* node = pattern.as<CallPatternNode>();
  ICHECK(node);
  ICHECK(node->args[0] == a);
  ICHECK(node->args[1] == b);
  auto* expr_pattern = node->op.as<ExprPatternNode>();
  ICHECK(expr_pattern);
  ICHECK(expr_pattern->expr == Op::Get("add"));
}

TEST(DFPattern, SUB) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = a - b;
  auto* node = pattern.as<CallPatternNode>();
  ICHECK(node);
  ICHECK(node->args[0] == a);
  ICHECK(node->args[1] == b);
  auto* expr_pattern = node->op.as<ExprPatternNode>();
  ICHECK(expr_pattern);
  ICHECK(expr_pattern->expr == Op::Get("subtract"));
}

TEST(DFPattern, MUL) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = a * b;
  auto* node = pattern.as<CallPatternNode>();
  ICHECK(node);
  ICHECK(node->args[0] == a);
  ICHECK(node->args[1] == b);
  auto* expr_pattern = node->op.as<ExprPatternNode>();
  ICHECK(expr_pattern);
  ICHECK(expr_pattern->expr == Op::Get("multiply"));
}

TEST(DFPattern, DIV) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = a / b;
  auto* node = pattern.as<CallPatternNode>();
  ICHECK(node);
  ICHECK(node->args[0] == a);
  ICHECK(node->args[1] == b);
  auto* expr_pattern = node->op.as<ExprPatternNode>();
  ICHECK(expr_pattern);
  ICHECK(expr_pattern->expr == Op::Get("divide"));
}

TEST(DFPattern, OR) {
  using namespace tvm;
  using namespace tvm::relay;
  auto a = WildcardPattern();
  auto b = WildcardPattern();
  auto pattern = a || b;
  auto* node = pattern.as<AltPatternNode>();
  ICHECK(node);
  ICHECK(node->left == a);
  ICHECK(node->right == b);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::FLAGS_gtest_death_test_style = "threadsafe";
  return RUN_ALL_TESTS();
}
