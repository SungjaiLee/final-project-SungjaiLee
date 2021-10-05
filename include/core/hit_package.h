//
// Created by Jack Lee on 2020/12/07.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H
#define NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H

#include <core/hits.h>
#include <core/util.h>

#include <cinder/gl/gl.h>

namespace room_explorer {

/**
 * Summary of all the hits intersected by a ray in a single direction.
 * Allows merging of two hit-packages.
 * Allow simple shifting and scaling of distances of all the hits in the package.
 */
class HitPackage {
private:
  std::map<float, Hit> hits_;

public:
  // Getters =======================================================
  size_t HitCount() const;

  const std::map<float, Hit>& GetHits() const;
  // End of Getters ================================================

  // Package Addition Methods ==========================================================================================
  /**
   * Add passed hit into the package.
   * If a hit already exist at the given distance, hit with higher priority will take over.
   * Does not add invalid hits.
   * @param hit Constant reference to hit to be added.
   * @return True if hit was able to be added. False if failed, be it due to existing hit, or invalid hit,
   */
  bool AddHit(const Hit& hit);

  /**
   * Merges two hit packages into the current package.
   * If a hit already exist at the given distance, hit with higher priority will take over.
   *    Hit from this package is prioritized over the hits from other.
   * @param package Constant reference of the package to be merged into this package.
   */
  void Merge(const HitPackage& package);
  // End of Package Addition Methods ===================================================================================


  // Distance Manipulator ==============================================================================================
  void ShiftHits(float shift);

  void ScaleDistances(float scale);
  // End of Distance Manipulator =======================================================================================
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_HIT_PACKAGE_H
