// Copyright 2017 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cmath>
#include <cstring>
#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include <glog/logging.h>
#include "public/myanmartools.h"

using namespace google_myanmar_tools;

class ZawgyiDetectorTest : public testing::Test {
 protected:
  const ZawgyiDetector detector_;
};

TEST_F (ZawgyiDetectorTest, strongTest) {
  const char* strong_unicode = u8"အပြည်ပြည်ဆိုင်ရာ လူ့အခွင့်အရေး ကြေညာစာတမ်း";
  const char* strong_zawgyi = u8"အျပည္ျပည္ဆိုင္ရာ လူ႔အခြင့္အေရး ေၾကညာစာတမ္း";
  EXPECT_LT(
      detector_.GetZawgyiProbability(strong_unicode),
      0.001);
  EXPECT_GT(
      detector_.GetZawgyiProbability(strong_zawgyi),
      0.999);
}

TEST_F (ZawgyiDetectorTest, compatibilityTest) {
  // Compare results with those obtained from Java version of the detector.
  // They should be the same, within float tolerances.
  std::ifstream infile("resources/compatibility.tsv");

  // Reads test file lines with numeric probability from Java for the string.
  // Compares each for identical values.
  std::string input_line;
  while (std::getline(infile, input_line)) {
    size_t tab_index;
    double expected_probability = std::stod(input_line, &tab_index);
    const char* test_text = input_line.data() + tab_index + 1;

    double computed_probability = detector_.GetZawgyiProbability(test_text);
    VLOG(2) << "exact? " << expected_probability <<
            " vs " << computed_probability <<
            " diff = " << fabs(expected_probability - computed_probability);
    EXPECT_DOUBLE_EQ(expected_probability, computed_probability)
              << "Test string: " << test_text;
  }
}
