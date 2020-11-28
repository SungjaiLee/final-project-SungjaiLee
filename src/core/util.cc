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


bool RayIntersectsWithSegment(const glm::vec2& segment_head, const glm::vec2& segment_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir) {
  // Trivial Cases
  // If position if head or tail itself, will be considered to have intersected with wall as a whole
  if (segment_head == ray_pos || segment_tail == ray_pos) {
    return true;
  }

  // Calculation is based on relative position to head and tail from the position of ray-head
  glm::vec2 head = segment_head - ray_pos;
  glm::vec2 tail = segment_tail - ray_pos;

  // complementarty normals
  glm::vec2 tail_c(tail.y, -tail.x);

  float a = glm::dot(head, tail_c);
  if (FloatingPointApproximation(a, 0)) {
    // head and tail are collinear


    if (glm::dot(head, tail) < 0) {
      // theta is pi, meaning pos is between the head and tail
      // definitely intersects
      return true;
    } else {
      // theta is 0
      // head and tail are collinear with the pos, or worse a point wall
      glm::vec2 dir_c(ray_dir.y, -ray_dir.x);

      if (FloatingPointApproximation(glm::dot(head, dir_c), 0)) {
        if (glm::dot(head, ray_dir) > 0) {
          // direction is 0
          return true;
        } else {
          // direction is pi
          //  wrong direction
          return false;
        }
      } else {
        // direction not 0 or pi
        // not facing either of correct orrientaiotn
        return false;
      }
    }

  } else {
    glm::vec2 dir_c(ray_dir.y, -ray_dir.x);
    float b = glm::dot(head, dir_c);
    // not colinear, either in I, II quad or III, IV quad
    if (a > 0) {
      // theta in (0, pi)
      if (b > 0 || FloatingPointApproximation(b, 0)) {
        // dir in [0, pi]

        float c = glm::dot(tail, dir_c);
        if (c < 0 || FloatingPointApproximation(c, 0)) {
          // dir <= theat
          // dir in [0, theta]
          return true;
        } else {
          return false;
        }

      } else {
        // dir in (pi, 2pi)
        return false;
      }
    } else {
      // theta in (pi, 2pi)

      if (b < 0 || FloatingPointApproximation(b, 0)) {
        // dir in [pi, 2pi]
        float c = glm::dot(tail, dir_c);

        if (c > 0 || FloatingPointApproximation(c, 0)) {
          // dir >= theta
          // dir in [theta, 2pi]
          return true;
        } else {
          return false;
        }

      } else {
        // dir in (0, pi)
        return false;
      }
    }
  }
}

float GetTheta(const glm::vec2& vec) {
  //TODO need be funiction
  float theta = std::atan2(vec.y, vec.x); //
  if (theta < 0) {
    theta = 2 * M_PI + theta;
  }
  return theta;
}

bool Collinear(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
  // if two are same, always colinear
  return FloatingPointApproximation(glm::dot(a - b, glm::vec2(b.y - c.y, c.x - b.x)), 0);
}

bool Parallel(const glm::vec2& a, const glm::vec2& b) {
  return FloatingPointApproximation(glm::dot(a, glm::vec2(b.y, -b.x)), 0);
}

// end of Geometric Utilities


} // namespace room_explorer