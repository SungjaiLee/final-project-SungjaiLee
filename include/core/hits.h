//
// Created by Jack Lee on 2020/11/20.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_HITS_H
#define NONEUCLIDEAN_RAY_CASTER_HITS_H

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

enum HitType : int { // associated value is their priotity, if two hits exist in the same distanmce, which should overcome what
  // lower number means highe rpriotity
  kWall = 0,
  kPortal = 1,
  kVoid = 2
};

struct Hit {

  float hit_distance_;

  HitType hit_type_;
  float texture_index_; // loads different location of a texture

};

class HitPackage {

  std::map<float, Hit> hits_;

  bool AddHit(Hit hit);

  void merge(const HitPackage& );
};

}


#endif //NONEUCLIDEAN_RAY_CASTER_HITS_H
