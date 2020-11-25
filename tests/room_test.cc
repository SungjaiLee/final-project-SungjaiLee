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
    "height" : 200,
    "ns_door_width" : 300,
    "ew_door_width" : 100
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

    REQUIRE(room.GetNSDoorWidth() == 300);
    REQUIRE(room.GetEWDoorWidth() == 100);
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

  SECTION("Check Get room consistency") {
    Room* room1 = room->GetConnectedRoom(kNorth);
    Room* room2 = room->GetConnectedRoom(kNorth);

    REQUIRE(room1 == room2);
  }
}

TEST_CASE("In-Room Check") {
  Room room = *factory.GenerateRandomRoom();
  // dimension of 500, 200

  SECTION("Middle") {
    REQUIRE(room.WithinRoom(glm::vec2(10,10)));
    REQUIRE(room.WithinRoom(glm::vec2(10,10), false));
  }

  SECTION("Edge") {
    SECTION("North") {
      REQUIRE(room.WithinRoom(glm::vec2(10,200)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,200), false));
    }

    SECTION("South") {
      REQUIRE(room.WithinRoom(glm::vec2(10,0)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,0), false));
    }

    SECTION("East") {
      REQUIRE(room.WithinRoom(glm::vec2(500,10)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(500,10), false));
    }

    SECTION("West") {
      REQUIRE(room.WithinRoom(glm::vec2(0,10)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(0,10), false));
    }
  }

  SECTION("Corner") {
    SECTION("North East") {
      REQUIRE(room.WithinRoom(glm::vec2(500,200)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(500,200), false));
    }

    SECTION("South East") {
      REQUIRE(room.WithinRoom(glm::vec2(500,0)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(500,0), false));
    }

    SECTION("North West") {
      REQUIRE(room.WithinRoom(glm::vec2(0,200)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(0,200), false));
    }

    SECTION("South West") {
      REQUIRE(room.WithinRoom(glm::vec2(0,0)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(0,0), false));
    }
  }

  SECTION("Outsdie") {
    SECTION("Nofth") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,900)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,900), false));
    }
    SECTION("North East") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,900)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,900), false));
    }
    SECTION("East") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,90)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,90), false));
    }
    SECTION("South East") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,-90)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(1000,-90), false));
    }
    SECTION("South") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,-90)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(10,-90), false));
    }
    SECTION("South west") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,-90)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,-90), false));
    }
    SECTION("West") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,90)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,90), false));
    }
    SECTION("North West") {
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,900)));
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,900), false));
    }
  }

}