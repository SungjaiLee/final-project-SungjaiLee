//
// Created by Jack Lee on 2020/12/03.
//

#include <catch2/catch.hpp>

TEST_CASE("AddHit") {
  SECTION("To empty") {
    SECTION("Add Invalid") {

    }
    SECTION("Add Wall") {

    }
    SECTION("Add Room Wall") {

    }
    SECTION("Add Portal") {

    }
  }
  SECTION("Add to already filled") {
    SECTION("No Overlap") {

    }
    SECTION("OverLap") {
      SECTION("Add higher priority") {

      }
      SECTION("Add lower priotity") {

      }
      SECTION("Add same priority") {

      }
    }
  }
}