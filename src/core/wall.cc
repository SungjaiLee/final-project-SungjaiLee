//
// Created by Jack Lee on 2020/11/18.
//

#include <core/wall.h>

namespace room_explorer {

// Load Wall from JSON =====================================================
void from_json(const json& json, Wall& wall) {
  wall.head_ = glm::vec2(json.at("head_x"), json.at("head_y"));
  wall.tail_ = glm::vec2(json.at("tail_x"), json.at("tail_y"));
}
// End of Load Wall from JSON ==============================================


// Wall Methods ========================================================================================================

// Constructors ============================================================
Wall::Wall(const glm::vec2& head, const glm::vec2& tail)
    : head_(head), tail_(tail) {}
// End of Constructors =====================================================


// Getter ==================================================================
const glm::vec2 &Wall::GetHead() const {
  return head_;
}

const glm::vec2 &Wall::GetTail() const {
  return tail_;
}
// End fo Getters ==========================================================


// Comparison Overload =====================================================
bool Wall::operator<(const Wall& wall) const {
  // arbitrary Comparison. No meaningful interpretation.
  return this < &wall;
}

bool Wall::operator==(const Wall& wall) const {
  // True if and only if both head and tail is equal
  return head_ == wall.head_ && tail_ == wall.tail_;
}
// End of Comparison Overload ==============================================


// Geometric Functions =====================================================
bool Wall::IntersectsWith(const glm::vec2& ray_pos, float dir_angle) const{
  return IntersectsWith(ray_pos, /*vec2*/{std::cos(dir_angle), std::sin(dir_angle)});
}

bool Wall::IntersectsWith(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const {
  return RayIntersectsWithSegment(head_, tail_, ray_pos, ray_dir);
}

float Wall::Distance(const glm::vec2& ray_pos, float dir_angle) const {
  // Handle in vector-form.
  return Distance(ray_pos, /*vec2*/{std::cos(dir_angle), std::sin(dir_angle)});
}

float Wall::Distance(const glm::vec2& pos, const glm::vec2& dir) const {
  if (IntersectsWith(pos, dir)) {
    // If the ray begins at collinear point to the wall,
    // the distance or ray to segment behaves differently to distance of ray to line.
    if (AreCollinear(pos, head_, tail_)) {
      // If ray-pos is collinear, ray will either be in-between the two end-points, in which case distance is 0
      //  or bet outside of the segment, in which case distance to the closer of head/tail is the distance
      //    In the second case, it is certain that ray must hit the head or tail, directly
      //    because it is already confirmed to intersect.

      // Ray can be confirmed to be outside of segment, if direction from ray to head and ray to tail is equal
      //  If ray was within the segment, the direction must be opposite, and thus their dot product positive or zero.
      if (glm::dot(head_ - pos, tail_ - pos) > 0) {
        return std::min(glm::length(head_ - pos),
                        glm::length(tail_ - pos));
      } else {
        return 0;
      }
    }

    // Certainty of intersection ensures that the returned value must be positive number
    return GetRayToLineDistance(head_, tail_, pos, dir);
  } else {
    // If does not intersect, default to -1.
    return -1;
  }
}

float Wall::TextureIndex(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const {
  // Outsourced to util method. Handles collinear collisions internally.
  return TextureIndexOnLineOfRay(head_, tail_,
                                 ray_pos, ray_dir);
}


// Hit Summaries ===================================================
Hit Wall::GetWallHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const {
  // Intersection deduced from Distance. Distance will return -1 if not intersected.
  float wall_intersection_distance = Distance(ray_pos, ray_dir);
  if (wall_intersection_distance == -1) {
    return {}; // Return invalid hit.
  }

  float texture_index = TextureIndex(ray_pos, ray_dir);
  return {wall_intersection_distance, kWall, texture_index};
}
// End of Hit Summaries ============================================

// End of Geometric Functions ==============================================


// End of Wall Methods =================================================================================================

} // namespace room_explorer