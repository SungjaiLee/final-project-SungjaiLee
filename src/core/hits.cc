//
// Created by Jack Lee on 2020/11/28.
//

#include <core/hits.h>

namespace room_explorer {

// Constructors ========================================================================================================
Hit::Hit()
    : hit_type_(kInvalid), hit_distance_(0), texture_index_(0) {}

Hit::Hit(float hit_distance, HitType hit_type, float texture_index)
    : hit_type_(hit_type), hit_distance_(hit_distance), texture_index_(texture_index) {}
// End of Constructors =================================================================================================

// Field Checker =======================================================================================================
bool Hit::IsNoHit() const {
  return hit_type_ == kInvalid;
}
// End of Field Checker ================================================================================================


// Distance Manipulator ================================================================================================
void Hit::ShiftDistance(float shift) {
  hit_distance_ += shift;
}

void Hit::ScaleDistance(float scale) {
  hit_distance_ *= scale;
}

bool Hit::WithinDistance(float max_distance) const {
  // Avoid using equality on floats
  if (hit_distance_ < 0 || hit_distance_ > max_distance) {
    return false;
  }
  return true;
}
// End of Distance Manipulator =========================================================================================


// Comparison Overloads ================================================================================================
bool Hit::operator==(const Hit& hit) const {
  if (hit_type_ != hit.hit_type_) {
    return false;
  }
  return  FloatApproximation(hit_distance_, hit.hit_distance_) &&
          FloatApproximation(texture_index_, hit.texture_index_);
}

bool Hit::operator!=(const Hit& hit) const {
  return !(*this == hit);
}
// End of Comparison Overloads =========================================================================================

} //namespace room_explorer
