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

/**
 * Wall consists of a segment, defined by two end points.
 *      Wall handles geometric interactions with a ray and can return hit-summary from a given ray.
 */
class Wall : std::string {
private:
  glm::vec2 head_;
  glm::vec2 tail_;

public:
  // Constructors =============================================================
  Wall() = default;

  Wall(const glm::vec2& head, const glm::vec2& tail);
  // End of Constructors ======================================================

  // Getters ==================================================================
  const glm::vec2& GetHead() const;
  const glm::vec2& GetTail() const;
  // End of Getters ===========================================================

  // Geometric Functions ======================================================
   /**
    * Checks if ray intersects with the segment defined by the wall.
    * @param ray_pos Initial position of the ray.
    * @param dir_angle Direction in standard polar coordinate of rht ray.
    * @return If the ray intersects with the current wall in positive direction.
    */
  bool IntersectsWith(const glm::vec2& ray_pos, float dir_angle) const;
  /**
   * Checks if ray intersects with the segment defined by the wall.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Vector direction of the ray.
   *                No need to be normalized.
   * @return If the ray intersects with the current wall in positive direction.
   */
  bool IntersectsWith(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  /**
   * Distance the ray must travel to intersect with the wall-segment.
   *    If ray does not intersect with the wall, returns -1. This information can be used to deduce intersection.
   *    Valid intersection, thus, must result in positive or zero distance.
   * @param ray_pos Initial position of the ray.
   * @param dir_angle Direction in standard polar coordinate of rht ray.
   * @return Pure distance, including negative and infinity, that ray must travel to intersect with the wall.
   */
  float Distance(const glm::vec2& ray_pos, float dir_angle) const;
  /**
   * Distance the ray must travel to intersect with the wall-segment.
   *    If ray does not intersect with the wall, returns -1. This information can be used to deduce intersection.
   *    Valid intersection, thus, must result in positive or zero distance.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Vector direction of the ray.
   *                No need to be normalized.
   * @return Pure distance, including negative and infinity, that ray must travel to intersect with the wall.
   */
  float Distance(const glm::vec2& pos, const glm::vec2& dir) const;

  /**
   * Retrieves Texture index of the current wall, intercepted by the ray.
   *    Index is distance of intersecting point from the head.
   *    Index is not post-processed,
   *        and in most contexts will be assumed to be called only under appropriate conditions.
   *    Negative index is possible.
   *    In-Line ray will have index of 0, if ray is aimed at head,
   *        or have index of distance between head and ray-position.
   *        The appropriate nature of such result is not handled in this function.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Vector direction of the ray.
   *                No need to be normalized.
   * @return Index of texture that should be loaded.
   *            Synonymous with distance of intersection from the segment-head.
   */
  float TextureIndex(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  /**
   * Returns Hit, a summary of intercept pf ray with the current wall.
   * Holds information such as hit-distance, hit-type (kWall), and texture index.
   *    Individual Hits should be combined into a HitPackage.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Vector direction of the ray.
   *                No need to be normalized.
   * @return Hit summary of ray-intersection with the wall.
   */
  Hit GetWallHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;
  // End of Geometric Functions ===============================================

  // Comparison Overloading ===================================================
  /**
   * Arbitrary Comparison.
   * Compares pure address pointer. No real geometric interpretation.
   *    Mapping of 2D object onto 1D value very difficult.
   * @param wall Wall that is being compared to current wall.
   * @return Whether the other wall's address is higher than the currents'.
   */
  bool operator< (const Wall& wall) const;
  /**
   * Comparison deemed equal if both head and tail occupy the same position.
   * @param wall Wall that is being compared to current wall.
   * @return Whether the other wall is equal to the current room in head and tail position.
   *            Ordering of head and tail important.
   */
  bool operator== (const Wall& wall) const;
  // End of Comparison  Overloading ==========================================

  // JSON constructor ========================================================
  friend void from_json(const json& j, Wall& wall);
  // end of JSON constructor =================================================
};


void from_json(const json& j, Wall& wall);

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_WALL_H
