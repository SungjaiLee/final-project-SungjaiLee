//
// Created by Jack Lee on 2020/11/20.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_RAY_H
#define NONEUCLIDEAN_RAY_CASTER_RAY_H

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

class Ray {

};

enum HitType {
  kWall,
  kPortal
};

class HitPackage {

  struct Hit {
    ci::Color
  };

  std::map<float, Hit> hits_;

  void merge(const HitPackage& );
};

}


#endif //NONEUCLIDEAN_RAY_CASTER_RAY_H
