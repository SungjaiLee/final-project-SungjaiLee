//
// Created by Jack Lee on 2020/11/27.
//

#include <core/util.h>

namespace room_explorer {

// Numeric Utilities ===================================================================================
bool FloatingPointApproximation(float a, float b, float epsilon) {
  float diff = std::abs(a - b);
  // If either is zero, ratio-comparison is invalid. Resort to regular epsilon-Approximation
  if (a == 0 || b == 0) {
    if (diff > epsilon) {
      return false;
    } else {
      return true;
    }
  }

  // Weak Approximation.
  //  If either if relatively small enough, passes the approximation.
  // To avoid using equality, which is at this context invalid, use strict inequality
  if (  diff / std::abs(a) > epsilon
    &&  diff / std::abs(b) > epsilon) {
    return false; // If both relative difference is too large, two are not approximate
  }
  return true;
}

bool FloatingPointApproximation(const glm::vec2& vec_a, const glm::vec2& vec_b, float epsilon) {
  // Vectors are equal if both components are equal
  return FloatingPointApproximation(vec_a.x, vec_b.x, epsilon)
      && FloatingPointApproximation(vec_a.y, vec_b.y, epsilon);
}

// end of Numeric Utilities


// Geometric Utilities ======================================================================================

bool IsUnitVector(const glm::vec2& vec) {
  return FloatingPointApproximation(vec.x * vec.x + vec.y * vec.y, 1);
}

float GetRayToLineDistance(const glm::vec2& line_head, const glm::vec2& line_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir) {

  // If position already is on head or tail, can by-pass further calculation.
  if (FloatingPointApproximation(line_head, ray_pos)) {
    return 0;
  }
  if (FloatingPointApproximation(line_tail, ray_pos)) {
    return 0;
  }

  // Single Point.
  if (FloatingPointApproximation(line_head, line_tail)) {
    // Single point does not have valid line-representation.
    //  If ray not at the point, will assume that line which passes this point and the ray-pos is a valid assumption.
    //  Thus pure distance to point is a valid length.
    return glm::length(line_head - ray_pos);
  }

  /* Utilize polar-coordinate representation of linear graph to calculate distance.
   * distance = (H - T)•<H.y - P.y, P.x - H.x> / (H - T)•<sin(θ), -cos(θ)>
   * This Calculation is carried out over the following segment in pieces.
   */

  // (H - T)
  glm::vec2 diff = line_head - line_tail;

  // <sin(θ), -cos(θ)>
  glm::vec2 normal_dir(ray_dir.y, -ray_dir.x);
  // Direction needs to be normal for correct calculation
  if (!IsUnitVector(normal_dir)) {
    normal_dir = glm::normalize(normal_dir);
  }

  // <H.y - P.y, P.x - H.x>
  glm::vec2 ref(line_head.y - ray_pos.y, ray_pos.x - line_head.x);

  float r = glm::dot(diff, ref);
  // If (H - T)•<H.y - P.y, P.x - H.x> is zero, regardless of denominator distance will be zero
  //  This also handles in-line case, where denominator==0 test will fail
  if(FloatingPointApproximation(r, 0)) {
    return 0;
  }

  float denominator = glm::dot(diff, normal_dir);
  if (FloatingPointApproximation(denominator, 0)) {
    // If demon is zero, means that ray and line are parallel, meaning distance should be infinite
    return std::numeric_limits<float>::infinity();
  }

  return r / denominator; // Refer to formula above for clarification.
}

float GetTheta(const glm::vec2& vec) {
  //TODO need be funiction
  float theta = std::atan2(vec.y, vec.x); //
  if (theta < 0) {
    theta = 2 * M_PI + theta;
  }
  return theta;
}

// end of Geometric Utilities


} // namespace room_explorer