//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room_factory.h>

#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("Room Factory Sanity Check") {
  std::cout << "Room Factory Test Ready" << std::endl;
}

TEST_CASE("Load RoomTemplate from JSON ") {
//Will expect Wall's Load tester to handle correctness of wall load
  SECTION("No wall") {
    json json = R"aa({
      "walls" : []
    })aa"_json;

    RoomFactory::RoomTemplate room_template = json;

    REQUIRE(room_template.GetWallCount() == 0);
  }

  SECTION("Multiple walls") {
    SECTION("One Wall") {
      json json = R"aa({
        "walls" : [{
          "head_x" : 0,
          "head_y" : 0,
          "tail_x" : 10,
          "tail_y" : 10
        }]
      })aa"_json;

      RoomFactory::RoomTemplate room_template = json;

      REQUIRE(room_template.GetWallCount() == 1);
    }

    SECTION("Many wall") {
      json json = R"aa({
        "walls" : [{
          "head_x" : 0,
          "head_y" : 0,
          "tail_x" : 10,
          "tail_y" : 10
        },
        {
          "head_x" : 10,
          "head_y" : 10,
          "tail_x" : 110,
          "tail_y" : 110
        }]
      })aa"_json;

      RoomFactory::RoomTemplate room_template = json;

      REQUIRE(room_template.GetWallCount() == 2);
    }
  }
}

TEST_CASE("Load Factory from JSON") {
  SECTION("No Rooms") {

//    json json = R"aa(
//    {
//      "room_dimension" : {
//        "width" : 500,
//        "height" : 500
//      }
//      "rooms" : []
//    })aa"_json;

//    RoomFactory factory = json;x

  }

  SECTION("Many Rooms") {

  }
}