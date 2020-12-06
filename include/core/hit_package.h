//
// Created by Jack Lee on 2020/12/07.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H
#define NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H

#include <core/hits.h>
#include <core/util.h>

#include <cinder/gl/gl.h>

namespace room_explorer {

class HitPackage {

  std::map<float, Hit> hits_;
public:
  size_t HitCount() const;

  const std::map<float, Hit>& GetHits() const;

  bool AddHit(Hit hit);

  void Merge(const HitPackage& package);

  void ShiftHits(float shift);

  void ScaleDistances(float scale);
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H
