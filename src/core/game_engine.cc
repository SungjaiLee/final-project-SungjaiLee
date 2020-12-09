//
// Created by Jack Lee on 2020/12/05.
//

#include <core/game_engine.h>

using namespace room_explorer;

using json = nlohmann::json;

GameEngine::GameEngine(const std::string& room_template_path) {
  // Factory must be loaded from json
  json factory_json;
  std::ifstream(room_template_path) >> factory_json;

  factory_ = factory_json;

  // Initial room will be generated from id "entry"
  current_room_ = factory_.GenerateRoom("entry");

  // Set initial player position.
  current_position_ = factory_.GetEntryPosition();
  view_direction_ = {1, 0}; // Direction must be non-zero, and so be assigned any direction, in this case (1,0).
}

std::vector<HitPackage> GameEngine::GetVision(float cos, float sin, size_t half_resolution, float range_distance) {
  std::vector<HitPackage> packages;

  // Main direction. Center Screen
  packages.push_back(current_room_->GetVisible(current_position_, view_direction_, range_distance));

  // Define two vectors to allow easier and faster rotaiton of direction in which ryas should be shot
  glm::vec2 counter_clockwise_direction{view_direction_};
  glm::vec2 clockwise_direction{view_direction_};

  for (size_t i = 0; i < half_resolution; ++i) {
    // Rotate the ray each time, preparing to shoot the next ray
    FastRotation(counter_clockwise_direction, cos, -sin); // negative sign for counter clockwise rotation
    FastRotation(clockwise_direction, cos, sin); // positive sin for clockwise rotation

    //To avoid fish-eye effect, each hit must be scaled down by cos of angle of ray,
    //  which can be computed by dot product. This must be absolute dot product, to avoid negative wall height

    HitPackage pack1 = current_room_->GetVisible(current_position_, counter_clockwise_direction, range_distance);
    pack1.ScaleDistances(std::abs(glm::dot(view_direction_, counter_clockwise_direction)));
    packages.insert(packages.begin(), pack1); // Left-most Hit add at front

    HitPackage pack2 = current_room_->GetVisible(current_position_, clockwise_direction, range_distance);
    pack2.ScaleDistances(std::abs(glm::dot(view_direction_, clockwise_direction)));
    packages.push_back(pack2); // Right-most Hit add at back
  }

  return packages;
}

void GameEngine::RotateDirection(float cos, float sin) {
  FastRotation(view_direction_, cos, sin);
}

void GameEngine::MoveForward(float speed) {
  // Movement must be in direction of vision.
  //  For quick check of any major intersection, shoot ray in direction of predicted motion
  //  Check if any hits occur withing |speed|. Absolute because distance must be positive
  HitPackage package_on_path{current_room_->GetVisible(current_position_,
                                                       speed * view_direction_,
                                                       std::abs(speed))};

  // Find the first hit in the package
  if (!package_on_path.GetHits().empty()) {
    // If not empty, must contain certain form of hit
    const Hit& hit{package_on_path.GetHits().begin()->second};

    switch (hit.hit_type_) {

      case kPortal:
        {
          // Must traverse room.
          // Need to know the direction of traversal
          Direction traversal_direction{current_room_->GetSideHit(current_position_, speed * view_direction_)};
          current_room_ = current_room_->GetConnectedRoom(traversal_direction); // Change room

          // Position must transported to corresponding position on the other side of portal
          current_position_ += speed * view_direction_;

          switch (traversal_direction) {
            case kNorth:
              current_position_.y -= current_room_->GetHeight();
              break;

            case kSouth:
              current_position_.y += current_room_->GetHeight();
              break;

            case kEast:
              current_position_.x -= current_room_->GetWidth();
              break;

            case kWest:
              current_position_.x += current_room_->GetWidth();
              break;

            case kUndefined:
              throw exceptions::InvalidDirectionException();
          }

          // Clamp Position within the room. Add Margin to prevent unexpected void-view
          if (current_position_.x < 0) {
            current_position_.x = WALL_MARGIN;
          }
          if (current_position_.x > current_room_->GetWidth()) {
            current_position_.x =  current_room_->GetWidth() - WALL_MARGIN;
          }
          if (current_position_.y < 0) {
            current_position_.y = WALL_MARGIN;
          }
          if (current_position_.y > current_room_->GetHeight()) {
            current_position_.y =  current_room_->GetHeight() - WALL_MARGIN;
          }
        }
        return; // No need to translate position afterwards

      case kRoomWall:
      case kWall:
        // Only limit motion
        if (std::abs(speed) > std::abs(hit.hit_distance_)) {
          // Need to limit speed to stop before the intersection
          float abs = hit.hit_distance_ - WALL_MARGIN; // Margin between player and thr wall.
          // Prevents undesirable wall-trapping
          if (speed > 0) {
            // Positive speed
            speed = abs;
          } else {
            // Negative distance
            speed = -abs;
          }
        }
        break;

      case kVoid:
      case kInvalid:
      default:
        // Expect to not occur. Can ignore
        break;
    }
  }

  current_position_ += speed * view_direction_;
}

