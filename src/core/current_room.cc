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
  current_position += speed * main_view_direction;
}

