//
// Created by Jack Lee on 2020/11/20.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_HITS_H
#define NONEUCLIDEAN_RAY_CASTER_HITS_H

#include <core/util.h>

#include <cinder/gl/gl.h>

/**
 * handles collision with wall, room wall, portal, and entity.
 * Can return summary of distance and hit type
 *
 *
 * Hit should be calculated only in the room,
 * return package from where it shot,
 * package needs to be able to merge
 */
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

struct Hit {
  /**
   * As default, construct invalid hit.
   *    Supposed to represent no-hit.
   */
  Hit();
  Hit(float hit_distance, HitType hit_type, float texture_index);

  float hit_distance_;

  HitType hit_type_;
  float texture_index_; // loads different location of a texture

  bool IsNoHit() const; // if invalid, ignore this hit

  void ShiftDistance(float shift);

  bool WithinDistance(float max_distance) const;

  bool operator== (Hit hit) const;
  bool operator!= (Hit hit) const;
};

class HitPackage {

  std::map<float, Hit> hits_;
public:
  size_t HitCount() const;

  const std::map<float, Hit>& GetHits() const;

  bool AddHit(Hit hit);

  void Merge(const HitPackage& package);

  void ShiftHits(float shift);
};

}


#endif //NONEUCLIDEAN_RAY_CASTER_HITS_H
