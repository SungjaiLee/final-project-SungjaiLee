//
// Created by Jack Lee on 2020/11/27.
//

#include <core/util.h>

namespace room_explorer {
float GetRayToLineDistance(const glm::vec2& wall_head, const glm::vec2& wall_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir) {
  if (wall_tail == wall_tail) {
    if (wall_tail == ray_pos) {
      return 0;
    }
  }

  glm::vec2 diff = wall_head - wall_tail;


  // perpendicular to head - pos, it is head-pos rotated by 90
  glm::vec2 m(wall_head.y - ray_pos.y, ray_pos.x - wall_head.x);

  if (diff == glm::vec2(0, 0)) {
    // point wall, need different handling
    // if angle hits the point, its distance of m,
    // TODO technically does not make sense to have a distance, so it is completely valid to assume distance is to the point wall
    return glm::length(m);
  }

  float r = glm::dot(diff, m);
  // if r is zero, means the pos is on the line, which regardless to directoin, has distance of 0
  if (r == 0) {
    return 0;
  }

  //TODO find faster alternatives
  // !!!! REQUIRE NORMALIZED dir
  glm::vec2 dir_c(ray_dir.y, -ray_dir.x);

  float denom = glm::dot(diff, dir_c);
  if (denom == 0) {
    // Does not intersect. Parrellel
    return -1;
  }

  r /= denom;

  return r;
}

} // namespace room_explorer