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

    json json = R"aa(
    {
      "room_dimension" : {
        "width" : 500,
        "height" : 200
      },
      "__comment" : "rooms are assisgned as map of string with value of room template",
      "rooms" : {
      }
    }
    )aa"_json;

    RoomFactory factory = json;

    REQUIRE(factory.RoomWidth() == 500);
    REQUIRE(factory.RoomHeight() == 200);

    REQUIRE(factory.RoomTemplateCount() == 0);

    std::set<std::string> ids = factory.GetAvailableIds();
    REQUIRE(ids.empty());
  }

  SECTION("Many Rooms") {

    SECTION("One Room") {

      json json = R"aa(
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
          }
        }
      })aa"_json;

      RoomFactory factory = json;

      REQUIRE(factory.RoomWidth() == 500);
      REQUIRE(factory.RoomHeight() == 200);

      REQUIRE(factory.RoomTemplateCount() == 1);

      std::set<std::string> ids = factory.GetAvailableIds();
      REQUIRE(!ids.empty());
      REQUIRE(ids.find("default") != ids.end());

    }

    SECTION("Many Rooms") {

      json json = R"aa(
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
              }
            ]
          }
        }
      })aa"_json;

      RoomFactory factory = json;

      REQUIRE(factory.RoomWidth() == 500);
      REQUIRE(factory.RoomHeight() == 200);

      REQUIRE(factory.RoomTemplateCount() == 3);

      std::set<std::string> ids = factory.GetAvailableIds();
      REQUIRE(!ids.empty());
      REQUIRE(ids.find("default") != ids.end());
      REQUIRE(ids.find("second") != ids.end());
      REQUIRE(ids.find("third") != ids.end());
    }

  }
}

TEST_CASE("Get Rand ID") {
  SECTION("No Id") {
    json json = R"aa(
      {
        "room_dimension" : {
          "width" : 500,
          "height" : 200
        },
        "__comment" : "rooms are assisgned as map of string with value of room template",
        "rooms" : {}
      })aa"_json;
    RoomFactory factory = json;

    REQUIRE_THROWS_AS(factory.RandomId(), exceptions::NoRoomTemplateException);
  }
  SECTION("One ID") {
    json json = R"aa(
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
          }
        }
      })aa"_json;
    RoomFactory factory = json;

    std::string id = factory.RandomId();

    REQUIRE(id == "default");
  }
  SECTION("More Id") {
    json json = R"aa(
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
              }
            ]
          }
        }
      })aa"_json;
    RoomFactory factory = json;

    std::string id = factory.RandomId();

    bool a = id == "default" || id == "second" || id == "third";
    REQUIRE(a);
  }
}

TEST_CASE("Generate New Room from template") {

  json json = R"aa(
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

  RoomFactory factory = json;

  SECTION("Generation Check") {

    SECTION("No matching Id") {
      Room* room = factory.GenerateRoom("none");
      REQUIRE(room == nullptr);
    }

    SECTION("Valid id") {
      Room* room = factory.GenerateRoom("default");
      REQUIRE(room != nullptr);

      REQUIRE(room->GetWidth() == 500);
      REQUIRE(room->GetHeight() == 200);

      REQUIRE(room->GetWallCount() == 2);
    }

  }

  SECTION("Distinction Check") {
    Room* room1 = factory.GenerateRoom("default");
    Room* room2 = factory.GenerateRoom("third");

    REQUIRE(room1 != room2);

    REQUIRE(room1->GetWidth() == 500);
    REQUIRE(room1->GetHeight() == 200);
    REQUIRE(room2->GetWidth() == 500);
    REQUIRE(room2->GetHeight() == 200);

    REQUIRE(room1->GetWallCount() != room2->GetWallCount());
  }
}