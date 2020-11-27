//
// Created by Jack Lee on 2020/11/27.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_UTIL_H
#define NONEUCLIDEAN_RAY_CASTER_UTIL_H

#include <cinder/gl/gl.h>

namespace room_explorer {

float GetRayToLineDistance(const glm::vec2& wall_head, const glm::vec2& wall_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir);

}

#endif //NONEUCLIDEAN_RAY_CASTER_UTIL_H
