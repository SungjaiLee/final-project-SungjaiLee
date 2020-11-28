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


} //namespace room_explorer
