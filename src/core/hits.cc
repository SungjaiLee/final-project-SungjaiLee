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
  return hit_distance_ >= 0 && hit_distance_ <= max_distance;
}

void Hit::ShiftDistance(float shift) {
  hit_distance_ += shift;
}

void Hit::ScaleDistance(float scale) {
  hit_distance_ *= scale;
}

} //namespace room_explorer
