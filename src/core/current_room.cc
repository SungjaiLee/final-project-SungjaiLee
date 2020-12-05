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
    packages.insert(packages.begin(), current_room_->GetVisible(current_position, counter_clockwise_direction, range_distance));
    packages.push_back(current_room_->GetVisible(current_position, clockwise_direction, range_distance));
  }

  return packages;
}

