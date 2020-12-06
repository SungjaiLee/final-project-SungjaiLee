//
// Created by Jack Lee on 2020/12/05.
//

#include <core/current_room.h>

using namespace room_explorer;

using json = nlohmann::json;

CurrentRoom::CurrentRoom(const std::string& room_template_path)
    : room_template_path_(room_template_path) {
  json factory_json;
  std::ifstream(room_template_path) >> factory_json;

  factory_ = factory_json;

  current_room_ = factory_.GenerateRoom("entry");

  current_position = {factory_.RoomWidth() / 2, factory_.RoomHeight() / 2};
  main_view_direction = {1, 0};
}

std::vector<HitPackage> CurrentRoom::GetVision(float cos, float sin, size_t half_resolution, float range_distance) {
  std::vector<HitPackage> packages;

  //main hit
  packages.push_back(current_room_->GetVisible(current_position, main_view_direction, range_distance));

  glm::vec2 counter_clockwise_direction{main_view_direction};
  glm::vec2 clockwise_direction{main_view_direction};

  for (size_t i = 0; i < half_resolution; ++i) {
    FastRotate(counter_clockwise_direction, cos, -sin);
    FastRotate(clockwise_direction, cos, sin);

    // add to front
    HitPackage pack1 = current_room_->GetVisible(current_position, counter_clockwise_direction, range_distance);
    pack1.ScaleDistances(std::abs(glm::dot(main_view_direction, counter_clockwise_direction)));
    packages.insert(packages.begin(), pack1);

    HitPackage pack2 = current_room_->GetVisible(current_position, clockwise_direction, range_distance);
    pack2.ScaleDistances(std::abs(glm::dot(main_view_direction, clockwise_direction)));
    packages.push_back(pack2);
  }

  return packages;
}

void CurrentRoom::RotateDirection(float cos, float sin) {
  FastRotate(main_view_direction, cos, sin);
}

void CurrentRoom::MoveForward(float speed) {
  // get all the item on the path, and see if the closest is closer than speed
  HitPackage package_on_path{current_room_->GetVisible(current_position, speed * main_view_direction, std::abs(speed))};
  auto it{package_on_path.GetHits().begin()};
  //
  if (it != package_on_path.GetHits().end()) {

    float distance = it->first;
    switch (it->second.hit_type_) {

      case kRoomWall:
      case kWall:
        // walls treated the same
        if (std::abs(speed) > std::abs(distance)) {
          float abs = distance - 1;
          if (speed > 0) {
            speed = abs;
          } else {
            speed = -abs;
          }
        }
        break;

      case kPortal: // todo Need to behave comepletely differently
//        if (std::abs(speed) > std::abs(distance)) {
//          float abs = distance - 1;
//          if (speed > 0) {
//            speed = abs;
//          } else {
//            speed = -abs;
//          }
//        }
        {
          Direction traversal_direction{current_room_->GetSideHit(current_position, speed * main_view_direction)};
          current_room_ = current_room_->GetConnectedRoom(traversal_direction);

          current_position += speed * main_view_direction;
          switch (traversal_direction) {

            case kNorth:
              current_position.y -= current_room_->GetHeight();
              break;
            case kSouth:
              current_position.y += current_room_->GetHeight();
              break;
            case kEast:
              current_position.x -= current_room_->GetWidth();
              break;
            case kWest:
              current_position.x += current_room_->GetWidth();
              break;

            case kUndefined:
              throw exceptions::InvalidDirectionException();
          }
        }

        // clamp in-room. Only one traversal allowed at a time
        if (current_position.x < 0) {
          current_position.x = 0;
        }
        if (current_position.x > current_room_->GetWidth()) {
          current_position.x =  current_room_->GetWidth();
        }
        if (current_position.y < 0) {
          current_position.y = 0;
        }
        if (current_position.y > current_room_->GetHeight()) {
          current_position.y =  current_room_->GetHeight();
        }

        return;

      case kVoid:
      case kInvalid:
        // skip
        break;
    }
  }
// else
  // nothing on path
  // just move

  current_position += speed * main_view_direction;
}

