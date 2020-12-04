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
    },
    "empty" : {
      "walls" : []
    },
    "simple_room" : {
      "walls" : [
        {
          "head_x" : 0,
          "head_y" : 0,
          "tail_x" : 500,
          "tail_y" : 200
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

    REQUIRE(room->IsConnectedWith(next, kNorth));
    REQUIRE(!room->IsConnectedWith(next, kSouth));
    REQUIRE(!room->IsConnectedWith(next, kEast));
    REQUIRE(!room->IsConnectedWith(next, kWest));
    REQUIRE(room->IsConnectedWith(next));
  }

  SECTION("South") {
    Room* next = room->GetConnectedRoom(kSouth);

    REQUIRE(next != nullptr);

    REQUIRE(!room->IsConnectedWith(next, kNorth));
    REQUIRE(room->IsConnectedWith(next, kSouth));
    REQUIRE(!room->IsConnectedWith(next, kEast));
    REQUIRE(!room->IsConnectedWith(next, kWest));
    REQUIRE(room->IsConnectedWith(next));
  }

  SECTION("East") {
    Room* next = room->GetConnectedRoom(kEast);

    REQUIRE(next != nullptr);

    REQUIRE(!room->IsConnectedWith(next, kNorth));
    REQUIRE(!room->IsConnectedWith(next, kSouth));
    REQUIRE(room->IsConnectedWith(next, kEast));
    REQUIRE(!room->IsConnectedWith(next, kWest));
    REQUIRE(room->IsConnectedWith(next));
  }

  SECTION("West") {
    Room* next = room->GetConnectedRoom(kWest);

    REQUIRE(next != nullptr);

    REQUIRE(!room->IsConnectedWith(next, kNorth));
    REQUIRE(!room->IsConnectedWith(next, kSouth));
    REQUIRE(!room->IsConnectedWith(next, kEast));
    REQUIRE(room->IsConnectedWith(next, kWest));
    REQUIRE(room->IsConnectedWith(next));

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

    SECTION("invalid edges"){
      REQUIRE_FALSE(room.WithinRoom(glm::vec2(-10,0)));
    }
  }
}

TEST_CASE("Room hit Diretion") {
  Room room = *factory.GenerateRandomRoom();
  // (500, 200)

  SECTION("Middle of Room") {
    SECTION("North") {
      SECTION("Edge") {
        SECTION("sloping") {
          REQUIRE(room.GetSideHit(glm::vec2(20,50), glm::vec2(1,5)) == kNorth);
          REQUIRE(room.GetSideHit(glm::vec2(20,50), glm::vec2(1,5), false) == kNorth);
        }
        SECTION("Vertical") {
          REQUIRE(room.GetSideHit(glm::vec2(20,50), glm::vec2(0,1)) == kNorth);
          REQUIRE(room.GetSideHit(glm::vec2(20,50), glm::vec2(0,1), false) == kNorth);
        }
      }
      SECTION("Corner") {
        REQUIRE(room.GetSideHit(glm::vec2(450,150), glm::vec2(1,1)) == kNorth);
        REQUIRE(room.GetSideHit(glm::vec2(450,150), glm::vec2(1,1), false) == kNorth);
      }
    }

    SECTION("East") {
      SECTION("Edge") {
        SECTION("Sloping") {
          REQUIRE(room.GetSideHit(glm::vec2(400, 50), glm::vec2(1, .5f)) == kEast);
          REQUIRE(room.GetSideHit(glm::vec2(400, 50), glm::vec2(1, .5f), false) == kEast);
        }
        SECTION("Horizontal") {
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(1, 0)) == kEast);
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(1, 0), false) == kEast);
        }
      }
      SECTION("Corner") {
        REQUIRE(room.GetSideHit(glm::vec2(450,150), glm::vec2(1,-1)) == kEast);
        REQUIRE(room.GetSideHit(glm::vec2(450,150), glm::vec2(1,-1), false) == kEast);
      }
    }

    SECTION("South") {
      SECTION("Edge") {
        SECTION("Sloping") {
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(1, -10)) == kSouth);
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(1, -10), false) == kSouth);
        }
        SECTION("Vertical") {
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(0, -1)) == kSouth);
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(0, -1), false) == kSouth);
        }
      }
      SECTION("Corner") {
        REQUIRE(room.GetSideHit(glm::vec2(50,50), glm::vec2(-1,-1)) == kSouth);
        REQUIRE(room.GetSideHit(glm::vec2(50,50), glm::vec2(-1,-1), false) == kSouth);
      }
    }

    SECTION("West") {
      SECTION("Edgy") {
        SECTION("Sloping") {
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(-4, 1)) == kWest);
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(-4, 1), false) == kWest);
        }
        SECTION("Horizontal") {
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(-1, 0)) == kWest);
          REQUIRE(room.GetSideHit(glm::vec2(20, 50), glm::vec2(-1, 0), false) == kWest);
        }
      }
      SECTION("Corner") {
        REQUIRE(room.GetSideHit(glm::vec2(50,150), glm::vec2(-1,1)) == kWest);
        REQUIRE(room.GetSideHit(glm::vec2(50,150), glm::vec2(-1,1), false) == kWest);
      }
    }
  }

  // check only point-inclusive
  SECTION("On edge") {
    SECTION("North") {
      REQUIRE(room.GetSideHit(glm::vec2(20,room.GetHeight()), glm::vec2(-1,0), true) == kNorth);
    }
    SECTION("East") {
      REQUIRE(room.GetSideHit(glm::vec2(room.GetWidth(),20), glm::vec2(-1,0), true) == kEast);
    }
    SECTION("South") {
      REQUIRE(room.GetSideHit(glm::vec2(10,0), glm::vec2(-1,0), true) == kSouth);
    }
    SECTION("West") {
      REQUIRE(room.GetSideHit(glm::vec2(0,10), glm::vec2(-1,0), true) == kWest);
    }
  }

  // only check point inclusive
  SECTION("On corner") {
    SECTION("NE") {
      REQUIRE(room.GetSideHit(glm::vec2(500,200), glm::vec2(-1,0), true) == kNorth);
    }
    SECTION("SE") {
      REQUIRE(room.GetSideHit(glm::vec2(500,0), glm::vec2(-1,0), true) == kEast);
    }
    SECTION("SW") {
      REQUIRE(room.GetSideHit(glm::vec2(0,0), glm::vec2(-1,0), true) == kSouth);
    }
    SECTION("NW") {
      REQUIRE(room.GetSideHit(glm::vec2(0,200), glm::vec2(-1,0), true) == kWest);
    }
  }

  SECTION("On edge pointing Point exclusion") {
    SECTION("On North") {
      SECTION("Point South") {
        REQUIRE(room.GetSideHit(glm::vec2(20, 200), glm::vec2(0, -1), false) == kSouth);
      }
      SECTION("Point East") {
        REQUIRE(room.GetSideHit(glm::vec2(200, 200), glm::vec2(10, -1), false) == kEast);
      }

      SECTION("Point West") {
        REQUIRE(room.GetSideHit(glm::vec2(1, 200), glm::vec2(-1, -1), false) == kWest);
      }
      SECTION("Point north, should be unavailable") {
        REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(5, 200), glm::vec2(0, 1), false),
                          exceptions::InvalidDirectionException);
      }
      SECTION("Pointing sideways") {
        SECTION("Point diretly east") {
          REQUIRE(room.GetSideHit(glm::vec2(1, 200), glm::vec2(1, 0), false) == kNorth);
        }
        SECTION("Point directly west") {
          REQUIRE(room.GetSideHit(glm::vec2(1, 200), glm::vec2(-1, 0), false) == kNorth);
        }
      }
    }

    SECTION("On South") {
      SECTION("Point North") {
        REQUIRE(room.GetSideHit(glm::vec2(20, 0), glm::vec2(0, 1), false) == kNorth);
      }
      SECTION("Point East") {
        REQUIRE(room.GetSideHit(glm::vec2(20, 0), glm::vec2(10, 1), false) == kEast);
      }

      SECTION("Point West") {
        REQUIRE(room.GetSideHit(glm::vec2(1, 0), glm::vec2(-1, 1), false) == kWest);
      }
      SECTION("Point South, should be unavailable") {
        REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(5, 0), glm::vec2(0, -1), false),
                          exceptions::InvalidDirectionException);
      }
      SECTION("Pointing sideways") {
        SECTION("Point diretly east") {
          REQUIRE(room.GetSideHit(glm::vec2(1, 0), glm::vec2(1, 0), false) == kSouth);
        }
        SECTION("Point directly west") {
          REQUIRE(room.GetSideHit(glm::vec2(1, 0), glm::vec2(-1, 0), false) == kSouth);
        }
      }
    }

    SECTION("West") {
      SECTION("Point East") {
        REQUIRE(room.GetSideHit(glm::vec2(0, 10), glm::vec2(100, 1), false) == kEast);
      }
      SECTION("Point North") {
        REQUIRE(room.GetSideHit(glm::vec2(0, 10), glm::vec2(1, 1), false) == kNorth);
      }
      SECTION("Point South") {
        REQUIRE(room.GetSideHit(glm::vec2(0, 10), glm::vec2(1, -1), false) == kSouth);
      }
      SECTION("Point West, should be unavailable") {
        REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(0, 10), glm::vec2(-1, -1), false),
                       exceptions::InvalidDirectionException);
      }
      SECTION("Pointing Sideways") {
        SECTION("Point directly North") {
          REQUIRE(room.GetSideHit(glm::vec2(0, 10), glm::vec2(0, 1), false) == kWest);
        }
        SECTION("Point directly South") {
          REQUIRE(room.GetSideHit(glm::vec2(0, 10), glm::vec2(0, -1), false) == kWest);
        }
      }
    }

    SECTION("East") {
      SECTION("Point West") {
        REQUIRE(room.GetSideHit(glm::vec2(500, 10), glm::vec2(-100, 1), false) == kWest);
      }
      SECTION("Point North") {
        REQUIRE(room.GetSideHit(glm::vec2(500, 10), glm::vec2(-1, 1), false) == kNorth);
      }
      SECTION("Point South") {
        REQUIRE(room.GetSideHit(glm::vec2(500, 10), glm::vec2(-1, -1), false) == kSouth);
      }
      SECTION("Point East, should be unavailable") {
        REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(500, 10), glm::vec2(1, -1), false),
                       exceptions::InvalidDirectionException);
      }
      SECTION("Pointing Sideways") {
        SECTION("Point directly North") {
          REQUIRE(room.GetSideHit(glm::vec2(500, 10), glm::vec2(0, 1), false) == kEast);
        }
        SECTION("Point directly South") {
          REQUIRE(room.GetSideHit(glm::vec2(500, 10), glm::vec2(0, -1), false) == kEast);
        }
      }
    }
  }

  SECTION("Outside Room") {
    SECTION("N") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(0, 1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("NE") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(1000, 1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("E") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(1000, 0), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("SE") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(1000, -1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("S") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(0, -1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("SW") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(-1000, -1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("W") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(-1000, 0), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
    SECTION("NW") {
      REQUIRE_THROWS_AS(room.GetSideHit(glm::vec2(-1000, 1000), glm::vec2(-1, -1), false),
                        exceptions::InvalidDirectionException);
    }
  }
}

TEST_CASE("Room Wall Hit Distance") {
  Room room = *factory.GenerateRandomRoom();
  SECTION("North") {
    REQUIRE(room.GetRoomWallHitDistance(kNorth, glm::vec2(0, 0), glm::vec2(0, 1)) == 200);
  }
  SECTION("South") {
    REQUIRE(room.GetRoomWallHitDistance(kSouth, glm::vec2(0, 200), glm::vec2(0, -1)) == 200);
  }
  SECTION("East") {
    REQUIRE(room.GetRoomWallHitDistance(kEast, glm::vec2(0, 200), glm::vec2(1, 0)) == 500);
  }
  SECTION("West") {
    REQUIRE(FloatApproximation(room.GetRoomWallHitDistance(kWest, glm::vec2(-100, 200), glm::vec2(1, -1)),
                               141.4213562373f));
  }
}

TEST_CASE("Portal Hit") {
  Room room = *factory.GenerateRandomRoom();
  // 5,2,3,1
  // (500, 200), [100, 400], [50, 150]
  SECTION("North") {
    SECTION("Portal") {
      SECTION("On Portal") {
        SECTION("left_edge") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(100, 200), glm::vec2(1, 1)));
        }
        SECTION("right_edge") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(400, 200), glm::vec2(-1, 1)));
        }
        SECTION("Middle") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(200, 200), glm::vec2(1, 0)));
        }
      }
      SECTION("Hits Portal") {
        SECTION("left_edge") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(150, 150), glm::vec2(-1, 1)));
        }
        SECTION("right_edge") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(350, 150), glm::vec2(1, 1)));
        }
        SECTION("middle") {
          REQUIRE(room.RayHitsPortal(kNorth, glm::vec2(350, 150), glm::vec2(0, 1)));
        }
      }
    }
    SECTION("Misses Portal") {
      SECTION("Hits in negative direction") {
        REQUIRE_FALSE(room.RayHitsPortal(kNorth, glm::vec2(100, 100), glm::vec2(0, -1)));
      }
      SECTION("Misses completely") {
        REQUIRE_FALSE(room.RayHitsPortal(kNorth, glm::vec2(0, 0), glm::vec2(-1, 1)));
      }
    }
  }

  SECTION("South") {
    SECTION("Portal") {
      SECTION("On Portal") {
        SECTION("left_edge") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(100, 0), glm::vec2(1, 1)));
        }
        SECTION("right_edge") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(400, 0), glm::vec2(-1, 1)));
        }
        SECTION("middle") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(200, 0), glm::vec2(1, 0)));
        }
      }
      SECTION("Hits Portal") {
        SECTION("left_edge") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(150, 50), glm::vec2(-1, -1)));
        }
        SECTION("right_edge") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(350, 50), glm::vec2(1, -1)));
        }
        SECTION("middle") {
          REQUIRE(room.RayHitsPortal(kSouth, glm::vec2(350, 150), glm::vec2(0, -1)));
        }
      }
    }
    SECTION("Misses Portal") {
      SECTION("Hits in negative direction") {
        REQUIRE_FALSE(room.RayHitsPortal(kSouth, glm::vec2(0, 150), glm::vec2(0, -1)));
      }
      SECTION("Misses completely") {
        REQUIRE_FALSE(room.RayHitsPortal(kSouth, glm::vec2(350, 150), glm::vec2(0, 1)));
      }
    }
  }

  SECTION("East") {
    SECTION("Portal") {
      SECTION("Hits Portal") {
        SECTION("top_edge") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(450, 200), glm::vec2(1, -1)));
        }
        SECTION("bottom_edge") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(450, 0), glm::vec2(1, 1)));
        }
        SECTION("Middle") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(100, 100), glm::vec2(1, 0)));
        }
      }
      SECTION("On Portal") {
        SECTION("top_edge") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(500, 150), glm::vec2(1, -1)));
        }
        SECTION("bottom_edge") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(500, 50), glm::vec2(1, -1)));
        }
        SECTION("Middle") {
          REQUIRE(room.RayHitsPortal(kEast, glm::vec2(500, 100), glm::vec2(1, 0)));
        }
      }
    }
    SECTION("Misses Portal") {
      SECTION("Hits in negative direction") {
        REQUIRE_FALSE(room.RayHitsPortal(kEast, glm::vec2(100, 100), glm::vec2(-1, 0)));
      }
      SECTION("Misses completely") {
        REQUIRE_FALSE(room.RayHitsPortal(kEast, glm::vec2(100, 0), glm::vec2(1, 0)));
      }
    }
  }

  SECTION("West") {
    SECTION("Portal") {
      SECTION("Hits Portal") {
        SECTION("top_edge") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(50, 200), glm::vec2(-1, -1)));
        }
        SECTION("bottom_edge") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(50, 0), glm::vec2(-1, 1)));
        }
        SECTION("Middle") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(50, 100), glm::vec2(-1, 0)));
        }
      }
      SECTION("On Portal") {
        SECTION("top_edge") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(0, 150), glm::vec2(-1, -1)));
        }
        SECTION("bottom_edge") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(0, 50), glm::vec2(-1, 1)));
        }
        SECTION("Middle") {
          REQUIRE(room.RayHitsPortal(kWest, glm::vec2(0, 100), glm::vec2(-1, 0)));
        }
      }
    }
    SECTION("Misses Portal") {
      SECTION("Hits in negative direction") {
        REQUIRE_FALSE(room.RayHitsPortal(kWest, glm::vec2(60, 100), glm::vec2(1, 0)));
      }
      SECTION("Misses completely") {
        REQUIRE_FALSE(room.RayHitsPortal(kWest, glm::vec2(60, 100), glm::vec2(-1, 10)));
      }
    }
  }
}

TEST_CASE("Get Wall/ Portal Head") {
  Room room = *factory.GenerateRandomRoom();

  SECTION("Portal") {
    SECTION("North") {
      REQUIRE(room.GetHead(kNorth, true) == glm::vec2(400, 200));
    }
    SECTION("East") {
      REQUIRE(room.GetHead(kEast, true) == glm::vec2(500, 50));
    }
    SECTION("South") {
      REQUIRE(room.GetHead(kSouth, true) == glm::vec2(100, 0));
    }
    SECTION("West") {
      REQUIRE(room.GetHead(kWest, true) == glm::vec2(0, 150));
    }
  }
  SECTION("Wall") {
    SECTION("North") {
      REQUIRE(room.GetHead(kNorth, false) == glm::vec2(500, 200));
    }
    SECTION("East") {
      REQUIRE(room.GetHead(kEast, false) == glm::vec2(500, 0));
    }
    SECTION("South") {
      REQUIRE(room.GetHead(kSouth, false) == glm::vec2(0, 0));
    }
    SECTION("West") {
      REQUIRE(room.GetHead(kWest, false) == glm::vec2(0, 200));
    }
  }
}

TEST_CASE("Get Wall/Portal Tail") {
  Room room = *factory.GenerateRandomRoom();
  SECTION("Wall") {
    SECTION("North") {
      REQUIRE(room.GetTail(kNorth, false) == glm::vec2(0, 200));
    }
    SECTION("East") {
      REQUIRE(room.GetTail(kEast, false) == glm::vec2(500, 200));
    }
    SECTION("South") {
      REQUIRE(room.GetTail(kSouth, false) == glm::vec2(500, 0));
    }
    SECTION("West") {
      REQUIRE(room.GetTail(kWest, false) == glm::vec2(0, 0));
    }
  }
  SECTION("Portal") {
    SECTION("North") {
      REQUIRE(room.GetTail(kNorth, true) == glm::vec2(100, 200));
    }
    SECTION("East") {
      REQUIRE(room.GetTail(kEast, true) == glm::vec2(500, 150));
    }
    SECTION("South") {
      REQUIRE(room.GetTail(kSouth, true) == glm::vec2(400, 0));
    }
    SECTION("West") {
      REQUIRE(room.GetTail(kWest, true) == glm::vec2(0, 50));
    }
  }
}

TEST_CASE("Get Wall Texture Index") {
  Room room = *factory.GenerateRandomRoom();
  SECTION("North") {
    SECTION("Portal") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kNorth, true,
                                         glm::vec2(399, 0), glm::vec2(0, 1)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kNorth, true,
                                           glm::vec2(0, 200), glm::vec2(1, 0)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kNorth, true,
                                           glm::vec2(0, 200), glm::vec2(-1, 0)) == 400);
        }
      }
    }
    SECTION("Wall") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kNorth, false,
                                         glm::vec2(499, 0), glm::vec2(0, 1)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kNorth, false,
                                           glm::vec2(0, 200), glm::vec2(1, 0)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kNorth, false,
                                           glm::vec2(0, 200), glm::vec2(-1, 0)) == 500);
        }
      }
    }
  }
  SECTION("South") {
    SECTION("Portal") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kSouth, true,
                                         glm::vec2(101, 50), glm::vec2(0, -1)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kSouth, true,
                                           glm::vec2(0, 0), glm::vec2(1, 0)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kSouth, true,
                                           glm::vec2(0, 0), glm::vec2(-1, 0)) == 100);
        }
      }
    }
    SECTION("Wall") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kSouth, false,
                                         glm::vec2(1, 0), glm::vec2(0, -1)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kSouth, false,
                                           glm::vec2(0, 0), glm::vec2(-1, 0)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kSouth, false,
                                           glm::vec2(-10, 0), glm::vec2(-1, 0)) == 10);
        }
      }
    }
  }
  SECTION("East") {

    SECTION("Portal") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kEast, true,
                                         glm::vec2(51, 51), glm::vec2(1, 0)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kEast, true,
                                           glm::vec2(500, 51), glm::vec2(0, -1)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kEast, true,
                                           glm::vec2(500, 51), glm::vec2(0, 1)) == 1);
        }
      }
    }
    SECTION("Wall") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kEast, false,
                                         glm::vec2(1, 1), glm::vec2(1, 0)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kEast, false,
                                           glm::vec2(500, 1), glm::vec2(0, -1)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kEast, false,
                                           glm::vec2(500, 10), glm::vec2(0, 10)) == 10);
        }
      }
    }
  }
  SECTION("West") {

    SECTION("Portal") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kWest, true,
                                         glm::vec2(51, 149), glm::vec2(-1, 0)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kWest, true,
                                           glm::vec2(0, 149), glm::vec2(0, 1)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kWest, true,
                                           glm::vec2(0, 50), glm::vec2(0, -1)) == 100);
        }
      }
    }
    SECTION("Wall") {
      SECTION("Hit") {
        REQUIRE(room.GetWallTextureIndex(kWest, false,
                                         glm::vec2(1, 199), glm::vec2(-1, 0)) == 1);
      }
      SECTION("In Line") {
        SECTION("Ray hits head") {
          REQUIRE(room.GetWallTextureIndex(kWest, false,
                                           glm::vec2(0, 0), glm::vec2(0, 1)) == 0);
        }
        SECTION("Does not hit head") {
          REQUIRE(room.GetWallTextureIndex(kWest, false,
                                           glm::vec2(0, 0), glm::vec2(0, -1)) == 200);
        }
      }
    }
  }
}


TEST_CASE("Room Primary Hit") {
  Room room = *factory.GenerateRandomRoom();
  SECTION("Valid") {
    SECTION("North") {
      SECTION("On This Wall") {
        SECTION("Inclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(399, 200), glm::vec2(0, -1));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 200), glm::vec2(0, 1));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }
        }
        SECTION("Exclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(399, 200), glm::vec2(1, 0), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 200), glm::vec2(1, 0), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }
        }
      }
        // Needs to be exclusive
      SECTION("On Other Wall") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 0), glm::vec2(0, 1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 200);
          REQUIRE(hit.texture_index_ == 1);
        }

        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(399, 0), glm::vec2(0, 1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 200);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
      SECTION("From Room") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 199), glm::vec2(0, 1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(399, 199), glm::vec2(0, 1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
    }
    SECTION("South") {
      SECTION("On This Wall") {
        SECTION("Inclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(101, 0), glm::vec2(0, -1));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 0), glm::vec2(0, 1));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }
        }
        SECTION("Exclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(101, 0), glm::vec2(-1, 0), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 0), glm::vec2(-1, 0), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }
        }
      }
        // Needs to be exclusive
      SECTION("On Other Wall") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 200), glm::vec2(0, -1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 200);
          REQUIRE(hit.texture_index_ == 1);
        }

        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(101, 200), glm::vec2(0, -1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 200);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
      SECTION("From Room") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 1), glm::vec2(0, -1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(101, 1), glm::vec2(0, -1), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
    }
    SECTION("East") {
      SECTION("On This Wall") {
        SECTION("Inclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 51), glm::vec2(1, 0));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 1), glm::vec2(1, 0));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }
        }
        SECTION("Exclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 51), glm::vec2(0, -1), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 1), glm::vec2(0, -1), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }
        }
      }
        // Needs to be exclusive
      SECTION("On Other Wall") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 1), glm::vec2(1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 500);
          REQUIRE(hit.texture_index_ == 1);
        }

        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 51), glm::vec2(1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 500);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
      SECTION("From Room") {
        SECTION("PORTAL") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 51), glm::vec2(1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
        SECTION("WALL") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(499, 1), glm::vec2(1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
    }
    SECTION("West") {
      SECTION("On This Wall") {
        SECTION("Inclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 149), glm::vec2(1, 0));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 199), glm::vec2(1, 0));

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 1);
          }
        }
        SECTION("Exclusive") {
          SECTION("Portal") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 149), glm::vec2(0, 1), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kPortal);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }

          SECTION("Wall") {
            Hit hit = room.GetPrimaryWallHit(glm::vec2(0, 199), glm::vec2(0, 1), false);

            REQUIRE_FALSE(hit.IsNoHit());

            REQUIRE(hit.hit_type_ == kRoomWall);
            REQUIRE(hit.hit_distance_ == 0);
            REQUIRE(hit.texture_index_ == 0);
          }
        }
      }
        // Needs to be exclusive
      SECTION("On Other Wall") {
        SECTION("Wall") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 199), glm::vec2(-1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 500);
          REQUIRE(hit.texture_index_ == 1);
        }

        SECTION("Portal") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(500, 149), glm::vec2(-1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 500);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
      SECTION("From Room") {
        SECTION("PORTAL") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 149), glm::vec2(-1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kPortal);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
        SECTION("WALL") {
          Hit hit = room.GetPrimaryWallHit(glm::vec2(1, 199), glm::vec2(-1, 0), false);

          REQUIRE_FALSE(hit.IsNoHit());

          REQUIRE(hit.hit_type_ == kRoomWall);
          REQUIRE(hit.hit_distance_ == 1);
          REQUIRE(hit.texture_index_ == 1);
        }
      }
    }
  }
  SECTION("Invalid") {
    Hit hit = room.GetPrimaryWallHit(glm::vec2(-1, 0), glm::vec2(1, 0));
    REQUIRE(hit.IsNoHit());
  }
}

TEST_CASE("Single Room HitPackage") {

  SECTION("Empty room") {
    Room room  = *factory.GenerateRoom("empty");
    SECTION("All the rooms") {
      SECTION("point inclusive") {
        SECTION("from a wall") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(0, 100),
                                                       glm::vec2(1, 0),
                                                       500, true);

          REQUIRE(package.HitCount() == 2);

          auto hits = package.GetHits();

          REQUIRE(hits[0].hit_type_ == kPortal);
          REQUIRE(hits[500].hit_type_ == kPortal);
        }
        SECTION("within room") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 100),
                                                       glm::vec2(1, 0),
                                                       500, true);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[400].hit_type_ == kPortal);
        }
      }
      SECTION("point exclusive") {
        SECTION("from a wall") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(0, 100),
                                                       glm::vec2(1, 0),
                                                       500, false);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[0].IsNoHit());
          REQUIRE(hits[500].hit_type_ == kPortal);
        }
        SECTION("within room") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 100),
                                                       glm::vec2(1, 0),
                                                       500, false);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[400].hit_type_ == kPortal);
        }
      }
    }

    SECTION("Limited Range") {
      SECTION("point inclusive") {
        SECTION("from a wall") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(0, 100),
                                                       glm::vec2(1, 0),
                                                       200, true);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[0].hit_type_ == kPortal);
          REQUIRE(hits[500].IsNoHit());
        }
        SECTION("within room") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 100),
                                                       glm::vec2(1, 0),
                                                       200, true);

          REQUIRE(package.HitCount() == 0);

          auto hits = package.GetHits();

          REQUIRE(hits[400].IsNoHit());
        }
      }
      SECTION("point exclusive") {
        SECTION("from a wall") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(0, 100),
                                                       glm::vec2(1, 0),
                                                       200, false);

          REQUIRE(package.HitCount() == 0);

          auto hits = package.GetHits();

          REQUIRE(hits[0].IsNoHit());
          REQUIRE(hits[500].IsNoHit());
        }
        SECTION("within room") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 100),
                                                       glm::vec2(1, 0),
                                                       200, false);

          REQUIRE(package.HitCount() == 0);

          auto hits = package.GetHits();

          REQUIRE(hits[400].IsNoHit());
        }
      }
    }
  }

  SECTION("Room with walls") {
    Room room  = *factory.GenerateRoom("simple_room");
    // wall is from corner to corner, with slope if 200/500 = 2/5
    //  so at y of
    SECTION("All the rooms") {
      SECTION("point inclusive") {
        HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                     glm::vec2(0, 1),
                                                     500, true);

        REQUIRE(package.HitCount() == 3);

        auto hits = package.GetHits();

        REQUIRE(hits[0].hit_type_ == kPortal);
        REQUIRE(hits[40].hit_type_ == kWall);
        REQUIRE(hits[200].hit_type_ == kPortal);
      }
      SECTION("point exclusive") {
        HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                     glm::vec2(0, 1),
                                                     500, false);

        REQUIRE(package.HitCount() == 2);

        auto hits = package.GetHits();

        REQUIRE(hits[0].IsNoHit());
        REQUIRE(hits[40].hit_type_ == kWall);
        REQUIRE(hits[200].hit_type_ == kPortal);
      }
    }

    SECTION("Limited Range") {
      SECTION("wall included") {
        SECTION("point inclusive") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                       glm::vec2(0, 1),
                                                       50, true);

          REQUIRE(package.HitCount() == 2);

          auto hits = package.GetHits();

          REQUIRE(hits[0].hit_type_ == kPortal);
          REQUIRE(hits[40].hit_type_ == kWall);
          REQUIRE(hits[200].IsNoHit());
        }

        SECTION("point exclusive") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                       glm::vec2(0, 1),
                                                       50, false);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[0].IsNoHit());
          REQUIRE(hits[40].hit_type_ == kWall);
          REQUIRE(hits[200].IsNoHit());
        }
      }
      SECTION("Wall not included") {
        SECTION("point inclusive") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                       glm::vec2(0, 1),
                                                       10, true);

          REQUIRE(package.HitCount() == 1);

          auto hits = package.GetHits();

          REQUIRE(hits[0].hit_type_ == kPortal);
          REQUIRE(hits[40].IsNoHit());
          REQUIRE(hits[200].IsNoHit());
        }

        SECTION("point exclusive") {
          HitPackage package = room.CurrentRoomPackage(glm::vec2(100, 0),
                                                       glm::vec2(0, 1),
                                                       10, false);

          REQUIRE(package.HitCount() == 0);

          auto hits = package.GetHits();

          REQUIRE(hits[0].IsNoHit());
          REQUIRE(hits[40].IsNoHit());
          REQUIRE(hits[200].IsNoHit());
        }
      }
    }
  }

  SECTION("On wall, point to portal") {
    Room room = *factory.GenerateRandomRoom();
    HitPackage package = room.CurrentRoomPackage(glm::vec2(0, 1), glm::vec2(0, 5), 200, true);

    REQUIRE(package.HitCount() == 1);

    auto hits = package.GetHits();

    REQUIRE(hits[0].hit_type_ == kRoomWall);
  }

}

TEST_CASE("Connected Room HitPackage") {

  Room room = *factory.GenerateRoom("simple_room");
  Room& room2 = *room.GetConnectedRoom(kNorth, "simple_room");
  room2.GetConnectedRoom(kNorth, "simple_room");

  SECTION("Only Current Room visible") {
    SECTION("not enough range") {

      HitPackage package = room.GetVisible(glm::vec2(250, 0), glm::vec2(0, 5), 200);

      REQUIRE(package.HitCount() == 3);

      auto hits = package.GetHits();

      REQUIRE(hits[0].hit_type_ == kPortal);
      REQUIRE(hits[100].hit_type_ == kWall);
      REQUIRE(hits[200].hit_type_ == kPortal);
    }
    SECTION("Enough range, hits room wall") {
    }
  }
  SECTION("Multiple room visible") {
    SECTION("One room visible") {
      HitPackage package = room.GetVisible(glm::vec2(250, 0), glm::vec2(0, 5), 350);

      REQUIRE(package.HitCount() == 4);

      auto hits = package.GetHits();

      REQUIRE(hits[0].hit_type_ == kPortal);
      REQUIRE(hits[100].hit_type_ == kWall);
      REQUIRE(hits[200].hit_type_ == kPortal);
      REQUIRE(hits[300].hit_type_ == kWall);
    }
    SECTION("multiple room visible") {

    }
  }
}