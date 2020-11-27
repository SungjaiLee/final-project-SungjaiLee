//
// Created by Jack Lee on 2020/11/27.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_UTIL_H
#define NONEUCLIDEAN_RAY_CASTER_UTIL_H

#include <cinder/gl/gl.h>

namespace room_explorer {

// Numeric Utilities ===================================================================================
bool FloatingPointApproximation(float a, float b, float epsilon = .0000001f);
bool FloatingPointApproximation(const glm::vec2& vec_a, const glm::vec2& vec_b, float epsilon = .0000001f);
// end of Numeric Utilities

// Geometric Utilities ======================================================================================

bool IsUnitVector(const glm::vec2& vec);

// TODO write javadoc
//  pure distance, including negative and -1 as not intersecting, from ray at given position to line defined by
//  two head and tail
//
float GetRayToLineDistance(const glm::vec2& line_head, const glm::vec2& line_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir);

// end of Geometric Utilities

}

#endif //NONEUCLIDEAN_RAY_CASTER_UTIL_H