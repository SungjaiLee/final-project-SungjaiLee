//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_WALL_H
#define NONEUCLIDEAN_RAY_CASTER_WALL_H


#include <core/hits.h>
#include <core/util.h>

#include "cinder/gl/gl.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace room_explorer {

class Wall : std::string {
private:
  glm::vec2 head_;
  glm::vec2 tail_;

public:

  Wall() = default;

  Wall(const glm::vec2& head, const glm::vec2& tail);

  const glm::vec2& GetHead() const;

  const glm::vec2& GetTail() const;

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
  float Distance(const glm::vec2& pos, const glm::vec2& dir) const;

  float TextureIndex(const glm::vec2& pos, const glm::vec2& dir) const; // Texture is calculated from head
  // bascially just getting distance but treating ray as line, and treating head as ray-head and tail-head as dir
  // if head=tail, always 0

  Hit GetWallHit(const glm::vec2& pos, const glm::vec2& dir) const;


  //TODO add fast sin, fast cos which will be just look-up of pre-calculated sin and cos value

  friend void from_json(const json& j, Wall& wall);
};


void from_json(const json& j, Wall& wall);

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_WALL_H
