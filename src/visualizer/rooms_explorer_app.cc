//
// Created by Jack Lee on 2020/11/15.
//

#include <visualizer/rooms_explorer_app.h>

using namespace room_explorer::visualizer;

RoomsExplorerApp::RoomsExplorerApp()
    : game_engine_(kTemplatePath) {
  // Application itself requires meta data, loaded from json path
  json meta_json;
  std::ifstream(kMetaPath) >> meta_json;

  // Only the essential components are loaded, rest are deduced.

  kScreenWidth_ = meta_json.at("screen_width");
  kScreenHeight_ = meta_json.at("screen_height");

  kHalfResolution_ = meta_json.at("half_resolution");
  kTotalResolution_ = 2 * kHalfResolution_ + 1;

  kStripWidth_ = kScreenWidth_ / kTotalResolution_;

  kHalfVisualFieldRange_ = meta_json.at("half_vision_field");

  kResolutionAngle_ = 2 * kHalfVisualFieldRange_ / kTotalResolution_;
  kResolutionCosine_ = std::cos(kResolutionAngle_);
  kResolutionSine_ = std::sin(kResolutionAngle_);

  kVisibleDistance_ = meta_json.at("visible_distance");

  kFloorHeight_ = meta_json.at("floor_height");

  // Adjuster adjusts brightness function so that distant enough elements are shaded enough.
  //  LN_EPSILON marks log of brightness of furthest possible element in range
  kBrightnessDepthAdjuster = LN_EPSILON / kVisibleDistance_;


  kMovementRotationAngle_ = meta_json.at("movement_rotation_angle");
  kMovementRotationCosine_ = std::cos(kMovementRotationAngle_);
  kMovementRotationSine_ = std::sin(kMovementRotationAngle_);

  kMovementSpeed_ = meta_json.at("movement_speed");

  kProjectionPlaneDistanceCoefficient_ = meta_json.at("projection_distance_coefficient");

  ci::app::setWindowSize(kScreenWidth_, kScreenHeight_);

  // Tick initially 0
  ticks_ = 0;
}

void RoomsExplorerApp::draw() {
  // Ceiling
  ci::Color8u background_color(0, 25, 64);
  ci::gl::clear(background_color);

  // Floor
  ci::gl::color(ci::Color8u(74, 2, 9));
  ci::Rectf floor{glm::vec2(0, kScreenHeight_),
                   glm::vec2(kScreenWidth_, kScreenHeight_ - kFloorHeight_)};
  ci::gl::drawSolidRect(floor);


  // Room-Elements
  // Load through HitPackages in each strip direction
  std::vector<HitPackage> packages{ game_engine_.GetVision(kResolutionCosine_, kResolutionSine_,
                                                          kHalfResolution_,
                                                          kVisibleDistance_) };
  for (size_t i = 0; i < kTotalResolution_; ++i) { // Must draw each strip in order
    const HitPackage& package = packages.at(i); // Hit Packages contain ordered individual Hits of room-element

    // the furthest element must be rendered first, so nearer object can be layered on top, giveing transparent depth
    // Start at the end, and iterate down
    auto it = package.GetHits().end();
    while (it != package.GetHits().begin()) {
      --it;
      DrawStrip(i, it->second);
    }
  }
}

void RoomsExplorerApp::update() {
  ++ticks_;

  // Opposite movement keys can be held at the same time.
  //  To avoid useless computation, record all the movement using these counters first,
  //  then only perform movement if valid direction occurs.
  size_t curr_rotation_direction{0};
  size_t curr_movement_direction{0};
  for (int key : held_keys_) {
    switch (key) {
      case ci::app::KeyEvent::KEY_LEFT:
        --curr_rotation_direction;
        break;
      case ci::app::KeyEvent::KEY_RIGHT:
        ++curr_rotation_direction;
        break;

      case ci::app::KeyEvent::KEY_UP:
        ++curr_movement_direction;
        break;
      case ci::app::KeyEvent::KEY_DOWN:
        --curr_movement_direction;
        break;

      default:
        // Do nothing
        break;
    }
  }

  if (curr_rotation_direction == 1) {
    // rotation clockwise
    game_engine_.RotateDirection(kMovementRotationCosine_, kMovementRotationSine_);
  } else if (curr_rotation_direction == -1) {
    // rotate counter clockwise
    game_engine_.RotateDirection(kMovementRotationCosine_, -kMovementRotationSine_);
  }

  if (curr_movement_direction == 1) {
    // Move forwards
    game_engine_.MoveForward(kMovementSpeed_);
  } else if (curr_movement_direction == -1) {
    // Move backwards
    game_engine_.MoveForward(-kMovementSpeed_);
  }

}

void RoomsExplorerApp::keyDown(ci::app::KeyEvent event) {
  // Add key to list of keys being held. Handle later
  held_keys_.insert( event.getCode() );
}

void RoomsExplorerApp::keyUp(ci::app::KeyEvent event) {
  // Add key to list of keys being held. Handle later
  held_keys_.erase( event.getCode() );
}


float RoomsExplorerApp::GetBrightness(float distance) const {
  // Brightness is yielded by exponential reciprocals of distance adjusted by BrightnessDepthAdjuster
  float brightness{std::exp(-distance * kBrightnessDepthAdjuster)};
  // Clamped between 0 and 1
  if (brightness < 0) {
    return 0;
  }
  if (brightness > 1) {
    return 1;
  }
  return brightness;
}

void RoomsExplorerApp::DrawStrip(float left_index, const Hit& hit) const {
  //! Remember, design of each element if not mathematically defined, and thus involved many empirical magic numbers
  //! Do excuse the messy formulas discovered by trial and error

  // Height of wall at given distance is yielded by inverse stretched by projection distance
  float room_height{kProjectionPlaneDistanceCoefficient_ / hit.hit_distance_};

  // Position of lower and upper edge of wall
  float lower_height{kScreenHeight_ - kFloorHeight_ + room_height / 9}; // 1/8th of the wall is below floor,
  float upper_height{kScreenHeight_ - kFloorHeight_ - 8 * room_height / 9}; // for more realistic look

  // EAch room-elements are rendered uniquely
  switch (hit.hit_type_) {
    case kRoomWall:
      {
        // Simple solid wall of shaded color
        ci::Rectf wall{{left_index * kStripWidth_,       lower_height},
                       {(left_index + 1) * kStripWidth_, upper_height}};
        float shade{GetBrightness(hit.hit_distance_)};
        ci::ColorA col{1 * shade, .2f * shade, .3f * shade};

        ci::gl::color(col);
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kWall:
      {
        // More complicated pattern, requiring texture-index
        float sub_index{static_cast<float>(std::fmod(hit.texture_index_, 50))};

        // Wall will involve transparent window with alternating large and small strips of window
        float lower_window;
        float upper_window;
        if (sub_index >= 30) {
          // Small strip, high up on the wall
          lower_window = 3 * room_height / 5;
          upper_window = 7 * room_height / 10;
        } else {
          // Big strip, Window is most of the wall
          lower_window = 0;
          upper_window = 8 * room_height / 10;
        }

        float shade{GetBrightness(hit.hit_distance_)};
        // Wall needs to be rendered in pieces
        // Solid Wall, no transparency
        ci::ColorA col{1 * shade, 1 * shade, 1 * shade};
        ci::gl::color(col);
        // lower section
        ci::Rectf wall {{left_index * kStripWidth_,       lower_height},
                        {(left_index + 1) * kStripWidth_, lower_height - lower_window}};
        ci::gl::drawSolidRect(wall);
        // upper Section
        wall = {{left_index * kStripWidth_,      lower_height - upper_window},
               {(left_index + 1) * kStripWidth_, upper_height}};
        ci::gl::drawSolidRect(wall);

        // Transparent Wall, green with greater transparency as player nears it
        col = {.012f * shade, .270f * shade, .078f * shade, 1.2f - shade};
        ci::gl::color(col);
        wall = {{left_index * kStripWidth_,       lower_height - lower_window},
                {(left_index + 1) * kStripWidth_, lower_height - upper_window}};
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kPortal:
      {
        using std::abs;
        // Most complex patterns.
        // Portals will have flowing veil-like waves depending on tick counts
        //  Red Fluctuation flows to the right, whereas blue flows left
        float red_fluctuation{static_cast<float>(
                            abs(std::fmod(hit.texture_index_ + ticks_, 20) - 10)  / 30)};
        float blue_fluctuation{static_cast<float>(
            abs(std::fmod(abs(hit.texture_index_ - 2 * ticks_), 52) - 26) / 100)};

        // Transparent to allow viewing of adjacent rooms
        //  For more unreal effect, portals do not get effected by distance
        ci::ColorA col{.359f + red_fluctuation, .074f, .336f + blue_fluctuation, .3f};
        ci::gl::color(col);
        ci::Rectf wall = {{left_index * kStripWidth_, lower_height},
                {(left_index + 1) * kStripWidth_, upper_height}};
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kVoid:
    case kInvalid:
      // Invalids, do not render
      break;
  }
}



