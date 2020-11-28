//
// Created by Jack Lee on 2020/11/28.
//



#include <core/util.h>

#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("IsUnitVector") {
  SECTION("Zero Vector") {
    REQUIRE_FALSE(IsUnitVector(glm::vec2(0, 0)));
  }
  SECTION("Unit Vector") {
    SECTION("Cardinal") {
      REQUIRE(IsUnitVector(glm::vec2(1, 0)));
    }
    SECTION("Sloping") {
      REQUIRE(IsUnitVector(glm::vec2(0.7071067812f, 0.7071067812f)));
    }
  }
  SECTION("Non Unit") {
    SECTION("Bigger than 1") {
      REQUIRE_FALSE(IsUnitVector(glm::vec2(2, .3f)));
    }
    SECTION("Less than 1") {
      REQUIRE_FALSE(IsUnitVector(glm::vec2(.1f, .3f)));
    }
  }
}

TEST_CASE("GetRayToLineDistance") {
  SECTION("Positive Direction") {
    SECTION("Hit") {
      SECTION("Mid") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, -.5f), glm::vec2(0, 1)) == 1);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, -.5f), glm::vec2(0, 5)) == 1);
        }
      }SECTION("Head") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, 0), glm::vec2(1, 0)) == .5f);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, 0), glm::vec2(.1f, 0)) == .5f);
        }
      }SECTION("Tail") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(0, .5f), glm::vec2(0, 1)) == .5f);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(0, .5f), glm::vec2(0, .1f)) == .5f);
        }
      }
    }SECTION("On Line") {
      SECTION("Head") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 1), glm::vec2(0, .1f)) == 0);
      }SECTION("Tail") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(1, 0), glm::vec2(0, .1f)) == 0);
      }SECTION("Mid") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(.5f, .5f), glm::vec2(0, .1f)) == 0);
      }
    }SECTION("Parallel") {
      SECTION("Unit dir") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 0), glm::vec2(1, -1)) == std::numeric_limits<float>::infinity());
      }SECTION("Non-unit Dir") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 0), glm::vec2(-.5, .5)) == std::numeric_limits<float>::infinity());
      }
    }

    SECTION("In-Line") {
      // direction and line colinear
      REQUIRE(GetRayToLineDistance(glm::vec2(1, 0), glm::vec2(2, 0),
                                   glm::vec2(0, 0), glm::vec2(1, 0)) == 0);

      REQUIRE(GetRayToLineDistance(glm::vec2(1, 1), glm::vec2(2, 2),
                                   glm::vec2(0, 0), glm::vec2(1, 1)) == 0);
    }
  }

  SECTION("Negative Direction") {
    SECTION("Hit") {
      SECTION("Mid") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, -.5f), glm::vec2(0, -1)) == -1);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, -.5f), glm::vec2(0, -5)) == -1);
        }
      }SECTION("Head") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, 0), glm::vec2(-1, 0)) == -.5f);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(.5f, 0), glm::vec2(-.1f, 0)) == -.5f);
        }
      }SECTION("Tail") {
        SECTION("Unit dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(0, .5f), glm::vec2(0, -1)) == -.5f);
        }SECTION("Non-unit Dir") {
          REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                       glm::vec2(0, .5f), glm::vec2(0, -.1f)) == -.5f);
        }
      }
    }SECTION("On Line") {
      SECTION("Head") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 1), glm::vec2(0, -.1f)) == 0);
      }SECTION("Tail") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(1, 0), glm::vec2(0, -.1f)) == 0);
      }SECTION("Mid") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(.5f, .5f), glm::vec2(0, -.1f)) == 0);
      }
    }
    SECTION("Parallel") {
      SECTION("Unit dir") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 0), glm::vec2(-1, 1)) == std::numeric_limits<float>::infinity());
      }SECTION("Non-unit Dir") {
        REQUIRE(GetRayToLineDistance(glm::vec2(0, 1), glm::vec2(1, 0),
                                     glm::vec2(0, 0), glm::vec2(.5, -.5)) == std::numeric_limits<float>::infinity());
      }
    }
  }
}

TEST_CASE("Collinear") {
  SECTION("Collinear") {
    SECTION("All different points") {
      REQUIRE(Collinear(glm::vec2(1,2), glm::vec2(2,4), glm::vec2(3,6)));
    }
    SECTION("Two same points") {
      SECTION("a,b") {
        REQUIRE(Collinear(glm::vec2(1,2), glm::vec2(1,2), glm::vec2(3,6)));
      }
      SECTION("b,c") {
        REQUIRE(Collinear(glm::vec2(1,2), glm::vec2(2,4), glm::vec2(2,4)));
      }
      SECTION("a,c") {
        REQUIRE(Collinear(glm::vec2(1,2), glm::vec2(2,4), glm::vec2(1,2)));
      }
    }
    SECTION("All same Points") {
      REQUIRE(Collinear(glm::vec2(5,2), glm::vec2(5,2), glm::vec2(5,2)));
    }
  }
  SECTION("Not Collinear") {
    SECTION("a out of line") {
      REQUIRE_FALSE(Collinear(glm::vec2(4,2), glm::vec2(2,4), glm::vec2(3,6)));
    }
    SECTION("b ut of line") {
      REQUIRE_FALSE(Collinear(glm::vec2(1,2), glm::vec2(2,5), glm::vec2(3,6)));
    }
    SECTION("c out of line") {
      REQUIRE_FALSE(Collinear(glm::vec2(1,2), glm::vec2(2,4), glm::vec2(9,6)));
    }
  }
}