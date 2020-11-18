//
// Created by Jack Lee on 2020/11/18.
//

#include <core/wall.h>

namespace room_explorer {

void from_json(const json& json, Wall& wall) {
  float h_x = json.at("head_x");
  float h_y = json.at("head_y");
  float t_x = json.at("tail_x");
  float t_y = json.at("tail_y");
  wall.head_ = glm::vec2(h_x, h_y);
  wall.tail_ = glm::vec2(t_x, t_y);
}

const glm::vec2 &Wall::getHead() const {
  return head_;
}

const glm::vec2 &Wall::getTail() const {
  return tail_;
}

} // namespace room_explorer