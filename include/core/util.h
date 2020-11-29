//
// Created by Jack Lee on 2020/11/27.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_UTIL_H
#define NONEUCLIDEAN_RAY_CASTER_UTIL_H

#include <cinder/gl/gl.h>

namespace room_explorer {

// Numeric Utilities ===================================================================================
bool FloatApproximation(float a, float b, float epsilon = .0000005f);
bool FloatApproximation(const glm::vec2& vec_a, const glm::vec2& vec_b, float epsilon = .0000005f);
// end of Numeric Utilities

// Geometric Utilities ======================================================================================

bool IsUnitVector(const glm::vec2& vec);

// TODO write javadoc
//  pure distance, including negative and -1 as not intersecting, from ray at given position to line defined by
//  two head and tail
//
float GetRayToLineDistance(const glm::vec2& line_head, const glm::vec2& line_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir);

//TODO??
bool RayIntersectsWithSegment(const glm::vec2& segment_head, const glm::vec2& segment_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir);

// returns distance from line-head to intersection position of ray adn line
//  use in texture index calculation
float TextureIndexOnLineOfRay(const glm::vec2& line_head, const glm::vec2& line_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir);

float GetTheta(const glm::vec2& vec);

bool Collinear(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

bool Parallel(const glm::vec2& a, const glm::vec2& b);

// end of Geometric Utilities

}

#endif //NONEUCLIDEAN_RAY_CASTER_UTIL_H
