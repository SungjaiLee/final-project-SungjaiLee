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

const glm::vec2 &Wall::GetHead() const {
  return head_;
}

const glm::vec2 &Wall::GetTail() const {
  return tail_;
}

bool Wall::operator<(const Wall& wall) const {
  //!! doesn't matter what it is actually comparing, just need a valid comparision for them to work in a set
  return this < &wall;

  //First check by actual wall length,
  //then check by head length,
  //then by tail

  //TODO find some clever maths
}

bool Wall::operator==(const Wall& wall) const {
  // True if and only if both head and tail is equal
  return head_ == wall.head_ && tail_ == wall.tail_;
}

Wall::Wall(const glm::vec2& head, const glm::vec2& tail) {
  head_ = head;
  tail_ = tail;
}

float Wall::Distance(const glm::vec2& pos, const float angle) {
  //TODO
  glm::vec2 diff = head_ - tail_;

  //TODO find faster alternatives
  float cos = glm::cos(angle);
  float sin = glm::sin(angle);
  glm::vec2 dir(sin, -cos);

  float denom = glm::dot(diff, dir);
  if (denom == 0) {
    // Does not intersect. Parrellel
    return -1;
  }

  glm::vec2 m(head_.y, -head_.x);

  float r = glm::dot(diff, m);
  r /= denom;

  return r;
}

bool Wall::IntersectsWith(const glm::vec2& pos, const float angle) {

  float theta_0 = GetTheta(head_ - pos);
  float theta_1 = GetTheta(tail_ - pos);
  if (theta_1 < theta_0) {
    //Swapping variables
    theta_1 = theta_0 + 0 * (theta_1 = theta_0);
  }

  if (angle < theta_0 || angle > theta_1) {
    return false;
  }

  return true;
}

// static unit methods =================================================
float GetTheta(const glm::vec2& vec) {
  //TODO need be funiction
  float theta = std::atan2(vec.x, vec.y); //
  if (theta < 0) {
    theta = 2 * M_PI - theta;
  }
  return theta;
}


} // namespace room_explorer