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
    REQUIRE(room.RoomWallHitDistance(kNorth, glm::vec2(0,0), glm::vec2(0, 1)) == 200);
  }
  SECTION("South") {
    REQUIRE(room.RoomWallHitDistance(kSouth, glm::vec2(0,200), glm::vec2(0, -1)) == 200);
  }
  SECTION("East") {
    REQUIRE(room.RoomWallHitDistance(kEast, glm::vec2(0,200), glm::vec2(1, 0)) == 500);
  }
  SECTION("West") {
    REQUIRE(FloatingPointApproximation(room.RoomWallHitDistance(kWest, glm::vec2(-100,200), glm::vec2(1, -1)),
                                       141.4213562373f));
  }
}