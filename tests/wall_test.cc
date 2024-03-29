//
// Created by Jack Lee on 2020/11/18.
//

#include <core/wall.h>

#include <catch2/catch.hpp>

using namespace room_explorer;

TEST_CASE("Sanity Check Wall") {
  std::cout << "Wall Tests Ready" << std::endl;
}

TEST_CASE("Load From JSON") {

  SECTION("Single Wall") {
      json json = R"aa(
      {
        "head_x" : 100.5,
        "head_y" : 55.555,
        "tail_x" : -1002.34,
        "tail_y" : 0
      }
    )aa"_json;

    Wall wall = json;

    REQUIRE(wall.GetHead() == glm::vec2(100.5, 55.555));
    REQUIRE(wall.GetTail() == glm::vec2(-1002.34, 0));
  }

  SECTION("Load into Set of Walls") {

    json json = R"(
    {
      "walls" : [
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        },
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        }
      ]
    }
    )"_json;

    std::set<Wall> walls;

    std::copy(json.at("walls").begin(), json.at("walls").end(),
              std::inserter(walls, walls.begin()));

    REQUIRE(walls.size() == 2);

    auto itr = walls.begin();

    REQUIRE(itr->GetHead() == glm::vec2(100.5, 55.555));
    REQUIRE(itr->GetTail() == glm::vec2(-1002.34, 0));
    ++itr;
    REQUIRE(itr->GetHead() == glm::vec2(100.5, 55.555));
    REQUIRE(itr->GetTail() == glm::vec2(-1002.34, 0));
  }

  SECTION("Load into Vector of Walls") {

    json json = R"(
    {
      "walls" : [
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        },
        {
          "head_x" : 100.5,
          "head_y" : 55.555,
          "tail_x" : -1002.34,
          "tail_y" : 0
        }
      ]
    }
    )"_json;

    std::vector<Wall> walls;

    walls.resize(json.at("walls").size());
    std::copy(json.at("walls").begin(), json.at("walls").end(), walls.begin() );

    REQUIRE(walls.size() == 2);

    REQUIRE(walls[0].GetHead() == glm::vec2(100.5, 55.555));
    REQUIRE(walls[0].GetTail() == glm::vec2(-1002.34, 0));
    REQUIRE(walls[1].GetHead() == glm::vec2(100.5, 55.555));
    REQUIRE(walls[1].GetTail() == glm::vec2(-1002.34, 0));
  }
}

TEST_CASE("Ray Intersection Test") {

  Wall wall(glm::vec2(1,0), glm::vec2(0,1));

  SECTION("No Intersection") {
    SECTION("Miss") {
      REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(1,1), 2));

      REQUIRE(wall.Distance(glm::vec2(1,1), 2) == -1);
    }

    SECTION("Wrong direction") {
      SECTION("Mid") {
        REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(0, 0), 5 * M_PI / 4));

        REQUIRE(wall.Distance(glm::vec2(0, 0), 5 * M_PI / 4) == -1);
      }

      SECTION("Head") {
        // bad approximation of sin and cos causes massive rounding error
        // suprisingly other tests manages to catch and result in correct answer, but still is dangerous
        SECTION("PI") {
          REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(0, 0), M_PI));

          REQUIRE(wall.Distance(glm::vec2(0, 0), M_PI) == -1);
        }

        SECTION("0") {
          REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(2, 0), 0));

          REQUIRE(wall.Distance(glm::vec2(2, 0), 0) == -1);
        }
      }

      SECTION("Tail") {
        REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(1, 1), 0));

        REQUIRE(wall.Distance(glm::vec2(1, 1), 0) == -1);
      }
    }

    SECTION("Parralle") {
      REQUIRE_FALSE(wall.IntersectsWith(glm::vec2(1, 1), 7 * M_PI / 4));

      REQUIRE(wall.Distance(glm::vec2(1, 1),  7 * M_PI / 4) == -1);
    }
  }

  SECTION("Intersection") {
    SECTION("Head Hit") {
      REQUIRE(wall.IntersectsWith(glm::vec2(1, 1), 3.f * M_PI / 2));

      REQUIRE(wall.Distance(glm::vec2(1, 1), 3.f * M_PI / 2) == 1);
//      REQUIRE(FloatApproximation(wall.Distance(glm::vec2(1, 1), 3 * M_PI / 2), 1, .1));
    }

    SECTION("Tail Hit") {
      REQUIRE(wall.IntersectsWith(glm::vec2(-1, 1), 0));

//      REQUIRE(wall.Distance(glm::vec2(-1, 1), 0) == 1.f);
      REQUIRE(FloatApproximation(wall.Distance(glm::vec2(-1, 1), 0), 1));
    }

    SECTION("Middle Hit") {
      REQUIRE(wall.IntersectsWith(glm::vec2(1.f/2, 5.f/2), 3 * M_PI / 2));

      REQUIRE(wall.Distance(glm::vec2(1.f/2, 5.f/2 ), 3 * M_PI / 2) == 2);

    }

    SECTION("In-Line") {
      SECTION("To Head") {
        REQUIRE(wall.IntersectsWith(glm::vec2(2, -1), 3 * M_PI / 4));

        REQUIRE(FloatApproximation(wall.Distance(glm::vec2(2, -1), 3 * M_PI / 4), std::sqrt(2)));
      }
      SECTION("To tail") {
        REQUIRE(wall.IntersectsWith(glm::vec2(-1, 2), 7 * M_PI / 4));

        REQUIRE(FloatApproximation(wall.Distance(glm::vec2(-1, 2), 7 * M_PI / 4), std::sqrt(2)));
      }
    }

    SECTION("Ray begin on the wall") {
      SECTION("Not Parralel") {
        SECTION("Head") {
          REQUIRE(wall.IntersectsWith(glm::vec2(1, 0), 0));

          REQUIRE(wall.Distance(glm::vec2(1, 0), 0) == 0);
        }

        SECTION("Tail") {
          REQUIRE(wall.IntersectsWith(glm::vec2(0, 1), 0));

          REQUIRE(wall.Distance(glm::vec2(0, 1), 0) == 0);
        }

        SECTION("Mid") {
          REQUIRE(wall.IntersectsWith(glm::vec2(.5f, .5f), 0));

          REQUIRE(wall.Distance(glm::vec2(.5f, .5f), 0) == 0);
        }

      }

      SECTION("Parrallel") {
        SECTION("Head") {
          SECTION("Into wall") {
            REQUIRE(wall.IntersectsWith(glm::vec2(1, 0), 3 * M_PI / 4));

            REQUIRE(wall.Distance(glm::vec2(1, 0), 3* M_PI / 4) == 0);
          }

          SECTION("Away from wall") {
            REQUIRE(wall.IntersectsWith(glm::vec2(1, 0), 7 * M_PI / 4));

            REQUIRE(wall.Distance(glm::vec2(1, 0), 7 * M_PI / 4) == 0);
          }
        }

        SECTION("Tail") {
          SECTION("Into wall") {
            REQUIRE(wall.IntersectsWith(glm::vec2(0, 1), 3 * M_PI / 4));

            REQUIRE(wall.Distance(glm::vec2(0, 1), 3* M_PI / 4) == 0);
          }

          SECTION("Away from wall") {
            REQUIRE(wall.IntersectsWith(glm::vec2(0, 1), 7 * M_PI / 4));

            REQUIRE(wall.Distance(glm::vec2(0, 1), 7 * M_PI / 4) == 0);
          }
        }

        SECTION("Mid") {
          REQUIRE(wall.IntersectsWith(glm::vec2(.5f, .5f), 7 * M_PI / 4));

          REQUIRE(wall.Distance(glm::vec2(.5f, .5f), 7 * M_PI / 4) == 0);
        }
      }
    }

    SECTION("Point wall") {
      Wall wall2(glm::vec2(1,1), glm::vec2(1,1));
      SECTION("Pos = point") {
        REQUIRE(wall2.IntersectsWith(glm::vec2(1, 1), 7 * M_PI / 4));

        REQUIRE(wall2.Distance(glm::vec2(1, 1), 7 * M_PI / 4) == 0);
      }
      SECTION("pos outside of point") {
        SECTION("ray hits point") {
          REQUIRE(wall2.IntersectsWith(glm::vec2(0, 0), M_PI / 4));

          REQUIRE(FloatApproximation(wall2.Distance(glm::vec2(0, 0), M_PI / 4), std::sqrt(2)));
        }
        SECTION("ray misses") {
          REQUIRE_FALSE(wall2.IntersectsWith(glm::vec2(0, 0), M_PI));

          REQUIRE(wall2.Distance(glm::vec2(0, 0), M_PI) == -1);
        }
      }
    }
  }
}

TEST_CASE("Texture Index") {

  Wall wall(glm::vec2(0, 1), glm::vec2(1, 0));

  SECTION("Before Head") {
    // negative
    SECTION("Positive Dir") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(-2, 1), glm::vec2(1, 1)), -std::sqrt(2)));
    }
    SECTION("On Wall") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(-1, 2), glm::vec2(-1, -1)), -std::sqrt(2)));
    }
    SECTION("Negative Direction") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(-2, 1), glm::vec2(-1, -1)), -std::sqrt(2)));
    }
  }

  SECTION("Head") {
    // zero
    SECTION("Positive Dir") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, 0), glm::vec2(0, 1)), 0));
    }
    SECTION("On Wall") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, 1), glm::vec2(0, 1)), 0));
    }
    SECTION("Negative Direction") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, 2), glm::vec2(0, 1)), 0));
    }
  }

  SECTION("Mid") {
    // between length
    SECTION("Positive Dir") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, 0), glm::vec2(1, 1)), std::sqrt(2) / 2));
    }
    SECTION("On Wall") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(.5f, .5f), glm::vec2(1, 1)), std::sqrt(2) / 2));
    }
    SECTION("Negative Direction") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(5, 5), glm::vec2(1, 1)), std::sqrt(2) / 2));
    }
  }

  SECTION("Tail") {
    // length
    SECTION("Positive Dir") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, -1), glm::vec2(1, 1)), std::sqrt(2)));
    }
    SECTION("On Wall") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(1, 0), glm::vec2(1, 1)), std::sqrt(2)));
    }
    SECTION("Negative Direction") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(2, 1), glm::vec2(1, 1)), std::sqrt(2)));
    }
  }
  SECTION("Beyond Tail") {
    // beyond length
    SECTION("Positive Dir") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(0, -1), glm::vec2(1, 0)), 2 * std::sqrt(2)));
    }
    SECTION("On Wall") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(2, -1), glm::vec2(1, 0)), 2 * std::sqrt(2)));
    }
    SECTION("Negative Direction") {
      REQUIRE(FloatApproximation(wall.TextureIndex(glm::vec2(3, -1), glm::vec2(1, 0)), 2 * std::sqrt(2)));
    }
  }
}

TEST_CASE("Hit") {

  Wall wall(glm::vec2(0, 1), glm::vec2(1, 0));

  SECTION("No Hit") {
    SECTION("Miss") {
      Hit hit = wall.GetWallHit(glm::vec2(-5,2), glm::vec2(-1,1));

      REQUIRE(hit.IsNoHit());
    }
    SECTION("Parallel") {
      Hit hit = wall.GetWallHit(glm::vec2(0,0), glm::vec2(-1,1));

      REQUIRE(hit.IsNoHit());
    }
  }

  SECTION("Valid Hit") {
    SECTION("Shoot and Hit") {
      SECTION("Head") {
        Hit hit = wall.GetWallHit(glm::vec2(0,0), glm::vec2(0,1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(hit.hit_distance_ == 1);
        REQUIRE(hit.texture_index_ == 0);
      }
      SECTION("Tail") {
        Hit hit = wall.GetWallHit(glm::vec2(0, 0), glm::vec2(1, 0));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(hit.hit_distance_ == 1);
        REQUIRE(FloatApproximation(hit.texture_index_, std::sqrt(2)));
      }
      SECTION("Mid") {
        Hit hit = wall.GetWallHit(glm::vec2(0, 0), glm::vec2(1, 1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(FloatApproximation(hit.hit_distance_, std::sqrt(2) / 2));
        REQUIRE(FloatApproximation(hit.texture_index_, std::sqrt(2) / 2));
      }
      SECTION("In-Line") {
        Hit hit = wall.GetWallHit(glm::vec2(-1, 2), glm::vec2(1, -1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(FloatApproximation(hit.hit_distance_, std::sqrt(2)));

        REQUIRE(FloatApproximation(hit.texture_index_, 0));
      }
    }
    SECTION("On wall") {
      SECTION("Head") {
        Hit hit = wall.GetWallHit(glm::vec2(0, 1), glm::vec2(1, -1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(FloatApproximation(hit.hit_distance_, 0));
        REQUIRE(FloatApproximation(hit.texture_index_, 0));
      }
      SECTION("Mid") {
        Hit hit = wall.GetWallHit(glm::vec2(.5f, .5f), glm::vec2(1, -1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(FloatApproximation(hit.hit_distance_, 0));
        REQUIRE(FloatApproximation(hit.texture_index_, std::sqrt(2) / 2));
      }
      SECTION("Tail") {
        Hit hit = wall.GetWallHit(glm::vec2(1, 0), glm::vec2(1, -1));

        REQUIRE_FALSE(hit.IsNoHit());

        REQUIRE(hit.hit_type_ == kWall);
        REQUIRE(FloatApproximation(hit.hit_distance_, 0));
        REQUIRE(FloatApproximation(hit.texture_index_, std::sqrt(2)));
      }
    }
  }
}