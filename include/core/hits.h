//
// Created by Jack Lee on 2020/11/20.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_HITS_H
#define NONEUCLIDEAN_RAY_CASTER_HITS_H

#include <core/util.h>

#include <cinder/gl/gl.h>

namespace room_explorer {

/**
 * Types of objects that rays will intersect with.
 * Values associated refer to priority.
 *      If two objects exist in same distant, the one with higher priority will replace the one object lower.
 */
enum HitType : int {
  kRoomWall = 4,
  kWall = 3,
  kPortal = 2,
  kVoid = 1,
  kInvalid = 0
};

/**
 * Summarizes a single intersection of vision ray with a room-element.
 * Holds essential information such as intersection distance, intersection type, and texture index.
 * Allow simple manipulation of distance, such as shifting and scaling hit distance.
 */
class Hit {
public:
  /**
   * As default, construct invalid hit.
   *    Supposed to represent no-hit.
   */
  Hit();
  Hit(float hit_distance, HitType hit_type, float texture_index);

  // Public struct-like variables ======================================================================
  HitType hit_type_;
  float hit_distance_{};
  float texture_index_{};
  // End of Public struct-like variables ===============================================================


  // Field Checker ===============================================
  /**
   * Checks if Hit does not have invalid hit type.
   * @return Type if not invalid.
   */
  bool IsNoHit() const; // if invalid, ignore this hit
  // End ofField Checker =========================================

  // Distance Manipulator ===================================================================
  void ShiftDistance(float shift);
  void ScaleDistance(float scale);

  bool WithinDistance(float max_distance) const;
  // End of Distance Manipulator ============================================================

  // Comparison Overload ====================================================================
  /**
   * Equals if and only if all three fields match.
   * @param hit The other Hit to compare to.
   * @return If this hit is equal to other.
   */
  bool operator== (const Hit& hit) const;
  /**
   * Equals if and only if all three fields match.
   * @param hit The other Hit to compare to.
   * @return If this hit is not equal to other.
   */
  bool operator!= (const Hit& hit) const;
  // End of Comparison Overload =============================================================
};

} // namespace room_explorer


#endif //NONEUCLIDEAN_RAY_CASTER_HITS_H
