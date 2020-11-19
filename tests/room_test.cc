//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room.h>
#include <core/room_factory.h>

#include <string>
#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("Room Test Sanity Check") {
  std::cout << " Room Test Ready" << std::endl;
}

// Factory Generation assumed to be tested in Factor Test


RoomFactory factory = R"aa(
{
  "room_dimension" : {
    "width" : 500,
    "height" : 200
  },
  "__comment" : "rooms are assisgned as map of string with value of room template",
  "rooms" : {
    "default" : {
      "walls" : [
        {
          "head_x" : 10,
          "head_y" : 10,
          "tail_x" : 100,
          "tail_y" : 100
        },
        {
          "head_x" : 11,
          "head_y" : 11,
          "tail_x" : 101,
          "tail_y" : 101
        }
      ]
    },
    "second" : {
      "walls" : [
        {
          "head_x" : 1,
          "head_y" : 1,
          "tail_x" : 10,
          "tail_y" : 10
        },
        {
          "head_x" : 11,
          "head_y" : 11,
          "tail_x" : 11,
          "tail_y" : 101
        }
      ]
    },
    "third" : {
      "walls" : [
        {
          "head_x" : 1,
          "head_y" : 1,
          "tail_x" : 10,
          "tail_y" : 10
        },
        {
          "head_x" : 11,
          "head_y" : 11,
          "tail_x" : 11,
          "tail_y" : 101
        },
        {
          "head_x" : 11,
          "head_y" : 11,
          "tail_x" : 11,
          "tail_y" : 101
        }
      ]
    }
  }
})aa"_json;

TEST_CASE("Correct Members generated") {


  Room& room = *factory.GenerateRoom("default");

  SECTION("dimension") {
    REQUIRE(room.GetWidth() == 500);
    REQUIRE(room.GetHeight() == 200);
  }

  SECTION("Walls") {
    REQUIRE(room.GetWallCount() == 2);

    SECTION("Correct Walls") {
      auto walls_iterator = room.GetWalls().begin();
//      {
//          "head_x" : 10,
//          "head_y" : 10,
//          "tail_x" : 100,
//          "tail_y" : 100
//        },
//        {
//          "head_x" : 11,
//          "head_y" : 11,
//          "tail_x" : 101,
//          "tail_y" : 101
//        }
      bool flag_wall1 = *walls_iterator == Wall(glm::vec2(10, 10), glm::vec2(100, 100));
      bool flag_wall2 = *walls_iterator == Wall(glm::vec2(11, 11), glm::vec2(101, 101));
      REQUIRE( (flag_wall1 || flag_wall2) );

      ++walls_iterator;

      // Require wall is different but of either kind of walls
      bool flag_wall3 = *walls_iterator == Wall(glm::vec2(10, 10), glm::vec2(100, 100));
      bool flag_wall4 = *walls_iterator == Wall(glm::vec2(11, 11), glm::vec2(101, 101));
      REQUIRE( (flag_wall3 != flag_wall1) );
      REQUIRE( (flag_wall4 != flag_wall2) );
      REQUIRE( (flag_wall3 || flag_wall4) );
    }
  }
}


TEST_CASE("Get Room") {
  //Expeect room to generate new room automatically
  Room* room = factory.GenerateRandomRoom();

  SECTION("North") {
    Room* next = room->GetConnectedRoom(kNorth);

    REQUIRE(next != nullptr);

    REQUIRE(room->ConnectedWith(next, kNorth));
    REQUIRE(!room->ConnectedWith(next, kSouth));
    REQUIRE(!room->ConnectedWith(next, kEast));
    REQUIRE(!room->ConnectedWith(next, kWest));
    REQUIRE(room->ConnectedWith(next));
  }

  SECTION("South") {
    Room* next = room->GetConnectedRoom(kSouth);

    REQUIRE(next != nullptr);

    REQUIRE(!room->ConnectedWith(next, kNorth));
    REQUIRE(room->ConnectedWith(next, kSouth));
    REQUIRE(!room->ConnectedWith(next, kEast));
    REQUIRE(!room->ConnectedWith(next, kWest));
    REQUIRE(room->ConnectedWith(next));
  }

  SECTION("East") {
    Room* next = room->GetConnectedRoom(kEast);

    REQUIRE(next != nullptr);

    REQUIRE(!room->ConnectedWith(next, kNorth));
    REQUIRE(!room->ConnectedWith(next, kSouth));
    REQUIRE(room->ConnectedWith(next, kEast));
    REQUIRE(!room->ConnectedWith(next, kWest));
    REQUIRE(room->ConnectedWith(next));
  }

  SECTION("West") {
    Room* next = room->GetConnectedRoom(kWest);

    REQUIRE(next != nullptr);

    REQUIRE(!room->ConnectedWith(next, kNorth));
    REQUIRE(!room->ConnectedWith(next, kSouth));
    REQUIRE(!room->ConnectedWith(next, kEast));
    REQUIRE(room->ConnectedWith(next, kWest));
    REQUIRE(room->ConnectedWith(next));

  }

  SECTION("Check Get room consitency") {
    Room* room1 = room->GetConnectedRoom(kNorth);
    Room* room2 = room->GetConnectedRoom(kNorth);

    REQUIRE(room1 == room2);
  }
}