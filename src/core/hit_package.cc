//
// Created by Jack Lee on 2020/12/07.
//

#include <core/hit_package.h>

namespace room_explorer {

size_t HitPackage::HitCount() const {
  return hits_.size();
}

const std::map<float, Hit>& HitPackage::GetHits() const {
  return hits_;
}

bool HitPackage::AddHit(Hit hit) {
  if (hit.IsNoHit()) {
    return false;
  }
  if (hits_.find(hit.hit_distance_) != hits_.end()) {
    Hit prev = hits_[hit.hit_distance_];

    if (hit.hit_type_ > prev.hit_type_) {
      // If eual priority, leave th eprevious one
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
  auto hit_map = package.GetHits();
  auto hit_map_iterator = hit_map.begin();

  while (hit_map_iterator != hit_map.end()) {
    this->AddHit(hit_map_iterator->second);
    ++hit_map_iterator;
  }

}

void HitPackage::ShiftHits(float shift) {
  auto temp_hit = std::move(hits_);
  // assume hits_ is not cleared
  assert(hits_.empty());

  auto temp_hit_it = temp_hit.begin();
  while (temp_hit_it != temp_hit.end()) {

    Hit hit(temp_hit_it->second);
    hit.ShiftDistance(shift);
    AddHit(hit);
    ++temp_hit_it;
  }
}

void HitPackage::ScaleDistances(float scale) {
  auto temp_hit = std::move(hits_);
  // assume hits_ is not cleared
  assert(hits_.empty());

  auto temp_hit_it = temp_hit.begin();
  while (temp_hit_it != temp_hit.end()) {

    Hit hit(temp_hit_it->second);
    hit.ScaleDistance(scale);
    AddHit(hit);
    ++temp_hit_it;
  }
}

} //namespace room_explorer