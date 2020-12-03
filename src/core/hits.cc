//
// Created by Jack Lee on 2020/11/28.
//

#include <core/hits.h>

namespace room_explorer {

Hit::Hit() {
  hit_type_ = kInvalid;
}

bool Hit::IsNoHit() const {
  return hit_type_ == kInvalid;
}

Hit::Hit(float hit_distance, HitType hit_type, float texture_index) {
  hit_distance_ = hit_distance;
  hit_type_ = hit_type;
  texture_index_ = texture_index;
}

bool Hit::operator==(Hit hit) const {
  if (hit_type_ != hit.hit_type_) {
    return false;
  }
  if (!FloatApproximation(hit_distance_, hit.hit_distance_)) {
    return false;
  }
  if (!FloatApproximation(texture_index_, hit.texture_index_)) {
    return false;
  }
  return true;
}

bool Hit::operator!=(Hit hit) const {
  return !(*this == hit);
}

bool Hit::WithinDistance(float max_distance) const {
  // TODO use float approximation?
  return hit_distance_ <= max_distance;
}


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
} //namespace room_explorer
