//
// Created by Jack Lee on 2020/12/07.
//

#include <core/hit_package.h>

namespace room_explorer {

// Getters ==============================================================================
size_t HitPackage::HitCount() const {
  return hits_.size();
}

const std::map<float, Hit>& HitPackage::GetHits() const {
  return hits_;
}
// Getters ==============================================================================


// Package Addition Methods ============================================================================================
bool HitPackage::AddHit(const Hit& hit) {
  // Ignore invalid hits
  if (hit.IsNoHit()) {
    return false;
  }

  // If a hit already exists on the given distance, one with higher priority should take over
  if (hits_.find(hit.hit_distance_) != hits_.end()) {
    const Hit& prev = hits_[hit.hit_distance_];

    // Only replace the hit if priority if strictly higher
    if (hit.hit_type_ > prev.hit_type_) {
      hits_[hit.hit_distance_] = hit;
      return true;

    } else {
      return false;
    }
  }

  hits_[hit.hit_distance_] = hit;
  return true;
}

void HitPackage::Merge(const HitPackage& package) {
  // const auto& is pair of float and Hit
  for (const auto& hit_pair : package.GetHits()) {
    AddHit(hit_pair.second); // in the
  }
}
// End of  Package Addition Methods ====================================================================================


// Distance Manipulator ================================================================================================
void HitPackage::ShiftHits(float shift) {
  // To shift the maps back, first remove the current map, then add shifted hits back
  auto temp_hits = std::move(hits_);

  // Add all the hits back, after shifting them
  for(auto& hit_pairs : temp_hits) {
    Hit& hit{hit_pairs.second};
    hit.ShiftDistance(shift);
    AddHit(hit); // Add Back using usual method
  }
}

void HitPackage::ScaleDistances(float scale) {
  // To shift the maps back, first remove the current map, then add scaled hits back
  auto temp_hits = std::move(hits_);

  // Add all the hits back, after scaling them
  for(auto& hit_pairs : temp_hits) {
    Hit& hit{hit_pairs.second};
    hit.ScaleDistance(scale);
    AddHit(hit); // Add Back using usual method
  }
}
// End of Distance Manipulator =========================================================================================

} //namespace room_explorer