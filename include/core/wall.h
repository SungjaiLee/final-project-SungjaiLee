//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_WALL_H
#define NONEUCLIDEAN_RAY_CASTER_WALL_H


#include "cinder/gl/gl.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace room_explorer {

class Wall {
private:
  glm::vec2 head_;
  glm::vec2 tail_;

public:

  Wall() = default;

  Wall(const glm::vec2&, const glm::vec2&);

  const glm::vec2 &getHead() const;

  const glm::vec2 &getTail() const;

  friend void from_json(const json& j, Wall& wall);

  bool operator< (const Wall& wall) const;
  bool operator== (const Wall& wall) const;

};

void from_json(const json& j, Wall& wall);

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_WALL_H
