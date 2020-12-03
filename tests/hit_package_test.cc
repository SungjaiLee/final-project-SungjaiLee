//
// Created by Jack Lee on 2020/12/03.
//

#include <core/hits.h>

#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("AddHit") {
  HitPackage package;
  SECTION("To empty") {
    SECTION("Add Invalid") {
      REQUIRE(package.HitCount() == 0);

      package.AddHit({});

      REQUIRE(package.HitCount() == 0);
    }
    SECTION("Add Wall") {
      REQUIRE(package.HitCount() == 0);

      package.AddHit({1, kWall, 5});

      REQUIRE(package.HitCount() == 1);

      auto hits = package.GetHits();

      REQUIRE(hits[1].hit_distance_ == 1);
      REQUIRE(hits[1].hit_type_ == kWall);
      REQUIRE(hits[1].texture_index_ == 5);
    }
    SECTION("Add Room Wall") {
      REQUIRE(package.HitCount() == 0);

      package.AddHit({1, kRoomWall, 5});

      REQUIRE(package.HitCount() == 1);

      auto hits = package.GetHits();

      REQUIRE(hits[1].hit_distance_ == 1);
      REQUIRE(hits[1].hit_type_ == kRoomWall);
      REQUIRE(hits[1].texture_index_ == 5);
    }
    SECTION("Add Portal") {
      REQUIRE(package.HitCount() == 0);

      package.AddHit({1, kPortal, 5});

      REQUIRE(package.HitCount() == 1);

      auto hits = package.GetHits();

      REQUIRE(hits[1].hit_distance_ == 1);
      REQUIRE(hits[1].hit_type_ == kPortal);
      REQUIRE(hits[1].texture_index_ == 5);
    }
  }
  SECTION("Add to already filled") {
    package.AddHit({1, kWall, 1});
    package.AddHit({2, kRoomWall, 1});
    package.AddHit({3, kPortal, 1});

    REQUIRE(package.HitCount() == 3);

    SECTION("No Overlap") {
      package.AddHit({0, kPortal, 1});

      REQUIRE(package.HitCount() == 4);
    }
    SECTION("OverLap") {
      SECTION("Add higher priority") {
        // RoomWall > Wall

        package.AddHit({1, kRoomWall, 1});

        REQUIRE(package.HitCount() == 3);

        auto hits = package.GetHits();

        REQUIRE(hits[1].hit_distance_ == 1);
        REQUIRE(hits[1].hit_type_ == kRoomWall);
        REQUIRE(hits[1].texture_index_ == 1);
      }
      SECTION("Add lower priority") {
        // Portal < RoomWall

        package.AddHit({2, kPortal, 5});

        REQUIRE(package.HitCount() == 3);

        auto hits = package.GetHits();

        REQUIRE(hits[2].hit_distance_ == 2);
        REQUIRE(hits[2].hit_type_ == kRoomWall);
        REQUIRE(hits[2].texture_index_ == 1);
      }
      SECTION("Add same priority") {
        package.AddHit({3, kPortal, 5});

        REQUIRE(package.HitCount() == 3);

        auto hits = package.GetHits();

        REQUIRE(hits[3].hit_distance_ == 3);
        REQUIRE(hits[3].hit_type_ == kPortal);
        REQUIRE(hits[3].texture_index_ == 1);
      }
    }
  }
}