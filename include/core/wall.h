//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_WALL_H
#define NONEUCLIDEAN_RAY_CASTER_WALL_H


#include <core/ray.h>
#include "cinder/gl/gl.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace room_explorer {

class Wall {
private:
  glm::vec2 head_;
  glm::vec2 tail_;

  float PureDistance(const glm::vec2& pos, const float angle) const;

public:

  Wall() = default;

  Wall(const glm::vec2& head, const glm::vec2& tail);

  const glm::vec2& GetHead() const;

  const glm::vec2 &GetTail() const;

  bool operator< (const Wall& wall) const;
  bool operator== (const Wall& wall) const;

  /**
   * Only checks if ray makes contact with this walls
   * @param pos
   * @param angle
   * @return
   */
  bool IntersectsWith(const glm::vec2& pos, const float angle) const;

  bool IntersectsWith(const glm::vec2& pos, const glm::vec2& dir) const;

  float Distance(const glm::vec2& pos, const float angle) const;

  HitPackage GetVisible(const glm::vec2& pos, const float angle);


  //TODO add fast sin, fast cos which will be just look-up of pre-calculated sin and cos value

  friend void from_json(const json& j, Wall& wall);
};

float GetTheta(const glm::vec2& vec);
//TODO move to own util file
// Also find a better more precise epsilon
bool FloatingPointApproximation(float a, float b, float epsilon = .0000001f);


void from_json(const json& j, Wall& wall);

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_WALL_H
