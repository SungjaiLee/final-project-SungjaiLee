//
// Created by Jack Lee on 2020/12/03.
//

#include <core/hit_package.h>

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

TEST_CASE("Merge") {
  SECTION("Empty") {
    SECTION("Both empoty") {
      HitPackage p1;
      HitPackage p2;

      REQUIRE(p1.HitCount() == 0);
      REQUIRE(p2.HitCount() == 0);

      p1.Merge(p2);

      REQUIRE(p1.HitCount() == 0);
      REQUIRE(p2.HitCount() == 0);
    }
    SECTION("One filled") {
      HitPackage p1;
      HitPackage p2;

      p1.AddHit({1, kRoomWall, 5});
      p2.AddHit({10, kRoomWall, 5});
      p2.AddHit({11, kRoomWall, 5});

      REQUIRE(p1.HitCount() == 1);
      REQUIRE(p2.HitCount() == 2);

      p1.Merge(p2);

      REQUIRE(p1.HitCount() == 3);
      REQUIRE(p2.HitCount() == 2);
    }
  }
  SECTION("Filled") {
    HitPackage p1;
    HitPackage p2;

    p1.AddHit({1, kWall, 5});
    p1.AddHit({2, kWall, 5});
    p1.AddHit({3, kWall, 5});

    REQUIRE(p1.HitCount() == 3);

    SECTION("No Overlap") {
      p2.AddHit({1.5, kPortal, 3});

      REQUIRE(p2.HitCount() == 1);

      p1.Merge(p2);

      REQUIRE(p1.HitCount() == 4);
      REQUIRE(p2.HitCount() == 1);
    }
    SECTION("Some overlap") {
      p2.AddHit({1.5, kPortal, 3});
      p2.AddHit({1, kPortal, 3});

      REQUIRE(p2.HitCount() == 2);

      p1.Merge(p2);

      REQUIRE(p1.HitCount() == 4);
      REQUIRE(p2.HitCount() == 2);
    }
    SECTION("All overlap") {
      p2.AddHit({1, kWall, 5});
      p2.AddHit({2, kWall, 5});
      p2.AddHit({3, kWall, 5});

      REQUIRE(p2.HitCount() == 3);

      p1.Merge(p2);

      REQUIRE(p1.HitCount() == 3);
      REQUIRE(p2.HitCount() == 3);
    }
  }
}

TEST_CASE("Shift Back package") {
  SECTION("Empty") {
    HitPackage hitPackage;
    REQUIRE(hitPackage.HitCount() == 0);

    hitPackage.ShiftHits(4);

    REQUIRE(hitPackage.HitCount() == 0);
  }
  SECTION("Non Empty") {
    SECTION("No overlap during shift") {
      HitPackage hitPackage;
      hitPackage.AddHit({4, kWall, 5});
      hitPackage.AddHit({5, kRoomWall, 5});
      hitPackage.AddHit({6, kPortal, 5});
      hitPackage.AddHit({7, kWall, 5});

      hitPackage.ShiftHits(4);

      REQUIRE(hitPackage.HitCount() == 4);

      auto hits = hitPackage.GetHits();

      REQUIRE(hits[8].hit_type_ == kWall);
      REQUIRE(hits[9].hit_type_ == kRoomWall);
      REQUIRE(hits[10].hit_type_ == kPortal);
      REQUIRE(hits[11].hit_type_ == kWall);
    }
    SECTION("May be overlap while shifting") {
      HitPackage hitPackage;
      hitPackage.AddHit({4, kWall, 5});
      hitPackage.AddHit({5, kRoomWall, 5});
      hitPackage.AddHit({6, kPortal, 5});
      hitPackage.AddHit({7, kWall, 5});

      hitPackage.ShiftHits(1);

      REQUIRE(hitPackage.HitCount() == 4);

      auto hits = hitPackage.GetHits();

      REQUIRE(hits[5].hit_type_ == kWall);
      REQUIRE(hits[6].hit_type_ == kRoomWall);
      REQUIRE(hits[7].hit_type_ == kPortal);
      REQUIRE(hits[8].hit_type_ == kWall);
    }
  }
}