//
// Created by Jack Lee on 2020/11/18.
//

#include <core/wall.h>

#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("Sanity Check Wall") {
  std::cout << "Wall Tests Ready" << std::endl;
}

TEST_CASE("Load From JSON") {
  json json = R"aa(
    {
      "head_x" : 100.5,
      "head_y" : 55.555,
      "tail_x" : -1002.34,
      "tail_y" : 0
    }
  )aa"_json;

  Wall wall = json;

  REQUIRE(wall.getHead() == glm::vec2(100.5, 55.555));
  REQUIRE(wall.getTail() == glm::vec2(-1002.34, 0));
}