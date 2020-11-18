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

  SECTION("Single Wall") {
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

  SECTION("Load into Set of Walls") {

    json json = R"(
    {
      "walls" : [
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        },
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        }
      ]
    }
    )"_json;

    std::set<Wall> walls;

    std::copy(json.at("walls").begin(), json.at("walls").end(),
              std::inserter(walls, walls.begin()));

    REQUIRE(walls.size() == 2);

    auto itr = walls.begin();

    REQUIRE(itr->getHead() == glm::vec2(100.5, 55.555));
    REQUIRE(itr->getTail() == glm::vec2(-1002.34, 0));
    ++itr;
    REQUIRE(itr->getHead() == glm::vec2(100.5, 55.555));
    REQUIRE(itr->getTail() == glm::vec2(-1002.34, 0));
  }

  SECTION("Load into Vector of Walls") {

    json json = R"(
    {
      "walls" : [
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        },
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        }
      ]
    }
    )"_json;

    std::vector<Wall> walls;

    walls.resize(json.at("walls").size());
    std::copy(json.at("walls").begin(), json.at("walls").end(), walls.begin() );

    REQUIRE(walls.size() == 2);

    REQUIRE(walls[0].getHead() == glm::vec2(100.5, 55.555));
    REQUIRE(walls[0].getTail() == glm::vec2(-1002.34, 0));
    REQUIRE(walls[1].getHead() == glm::vec2(100.5, 55.555));
    REQUIRE(walls[1].getTail() == glm::vec2(-1002.34, 0));
  }
}