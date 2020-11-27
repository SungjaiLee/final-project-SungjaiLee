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


} //namespace room_explorer
