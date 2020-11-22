//
// Created by Jack Lee on 2020/11/18.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <core/current_room.h>

bool KnuthApprox(float actual, float expected, float epsilon) {
  float diff = actual - expected;
  if (diff < epsilon || diff > epsilon) {
    return false;
  }
  return true;
}

TEST_CASE("Main Test Sanity Check") {
  std::cout << "Main Test Ready" << std::endl;

}