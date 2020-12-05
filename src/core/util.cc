//
// Created by Jack Lee on 2020/11/27.
//

#include <core/util.h>

namespace room_explorer {

// Numeric Utilities ===================================================================================================
bool FloatApproximation(float a, float b, float epsilon) {
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
  if (diff / std::abs(a) > epsilon &&
      diff / std::abs(b) > epsilon) {
    return false; // If both relative difference is too large, two are not approximate
  }
  return true;
}

bool FloatApproximation(const glm::vec2& vec_a, const glm::vec2& vec_b, float epsilon) {
  // Vectors are equal if both components are equal
  return FloatApproximation(vec_a.x, vec_b.x, epsilon) &&
         FloatApproximation(vec_a.y, vec_b.y, epsilon);
}
// end of Numeric Utilities ============================================================================================


// Geometric Utilities =================================================================================================
bool IsUnitVector(const glm::vec2& vec) {
  // Dot with self yields distance squared, which, in case of unit vector, should remain 1.
  return FloatApproximation(vec.x * vec.x + vec.y * vec.y, 1);
}

bool AreCollinear(const glm::vec2& point_a, const glm::vec2& point_b, const glm::vec2& point_c) {
  // Collinearity is implied by 2 vectors being parallel.
  //  This also satisfies co-point condition.
  return AreParallel(point_a - point_b, point_c - point_b);
}

bool AreParallel(const glm::vec2& vec_a, const glm::vec2& vec_b) {
  // Vectors are parallel if dot of one with the normal of other is zero.
  //  Also satisfies zero-vector condition.
  return FloatApproximation(glm::dot(vec_a, glm::vec2(vec_b.y, -vec_b.x)), 0);
}


float GetRayToLineDistance(const glm::vec2& line_head, const glm::vec2& line_tail,
                           const glm::vec2& ray_pos, const glm::vec2& ray_dir) {
  // If position already is on head or tail, trivially 0 distance
  if (FloatApproximation(line_head, ray_pos) ||
      FloatApproximation(line_tail, ray_pos)) {
    return 0;
  }
  // Single Point. Assume line passes through ray-pos.
  if (FloatApproximation(line_head, line_tail)) {
    return 0;
  }

  /* Utilize polar-coordinate representation of linear graph to calculate distance.
   * distance = (H - T)•<H.y - P.y, P.x - H.x> / (H - T)•<sin(θ), -cos(θ)>
   * This Calculation is carried out over the following segment in pieces.
   */

  // (H - T)
  glm::vec2 diff = line_head - line_tail; // Not zero-vector. Such case is already caught in pre-processing.

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
  //  This also handles in-line case, where denominator == 0 test will fail
  if(FloatApproximation(r, 0)) {
    return 0;
  }

  float denominator = glm::dot(diff, normal_dir);
  if (FloatApproximation(denominator, 0)) {
    // If demon is zero, means that ray and line are parallel, meaning distance should be infinite
    return std::numeric_limits<float>::infinity();
  }

  return r / denominator; // Refer to formula above for clarification.
}


bool RayIntersectsWithSegment(const glm::vec2& segment_head, const glm::vec2& segment_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir,
                              bool parallel_hit_valid) {
  // Trivial Cases
  // If position if head or tail itself, will be considered to have intersected with wall as a whole
  if (FloatApproximation(segment_head, ray_pos) ||
      FloatApproximation(segment_tail, ray_pos)) {
    return true;
  }

  //TODO calculation based on flowchart I created. Post will be up on my math blog later,
  // but current implementation is purely based on the dot-product representation,
  // which has valid geometric interpretation, but such are not yet implemented in this version
  //TODO make this more readable

  // Calculation is based on relative position to head and tail from the position of ray-head
  glm::vec2 head = segment_head - ray_pos;
  glm::vec2 tail = segment_tail - ray_pos;

  // Complementary normals
  glm::vec2 tail_c(tail.y, -tail.x);

  float a = glm::dot(head, tail_c);
  if (FloatApproximation(a, 0)) {
    // head and tail are collinear

    if (glm::dot(head, tail) < 0) {
      // theta is pi, meaning pos is between the head and tail
      // definitely intersects
      return true;

    } else {
      // theta is 0
      // head and tail are collinear with the pos, or worse a point wall
      if (!parallel_hit_valid) {
        // If does not allow parallel hit, immediately false
        return false;
      }

      glm::vec2 dir_c(ray_dir.y, -ray_dir.x);

      if (FloatApproximation(glm::dot(head, dir_c), 0)) {
        if (glm::dot(head, ray_dir) > 0) {
          // direction is 0 rad
          return true;
        } else {
          // direction is pi rad
          //  wrong direction
          return false;
        }
      } else {
        // direction not 0 or pi rad
        // not facing either of correct orrientaiotn
        return false;
      }
    }

  } else {
    glm::vec2 dir_c(ray_dir.y, -ray_dir.x);
    float b = glm::dot(head, dir_c);
    // not collinear, either in I, II quad or III, IV quad
    if (a > 0) {
      // theta in (0, pi) rad
      if (b > 0 || FloatApproximation(b, 0)) {
        // dir in [0, pi] rad

        float c = glm::dot(tail, dir_c);
        if (c < 0 || FloatApproximation(c, 0)) {
          // dir <= theta rad
          // dir in [0, theta] rad
          return true;
        } else {
          return false;
        }

      } else {
        // dir in (pi, 2pi) rad
        return false;
      }
    } else {
      // theta in (pi, 2pi) rad

      if (b < 0 || FloatApproximation(b, 0)) {
        // dir in [pi, 2pi] rad
        float c = glm::dot(tail, dir_c);

        if (c > 0 || FloatApproximation(c, 0)) {
          // dir >= theta rad
          // dir in [theta, 2pi] rad
          return true;
        } else {
          return false;
        }

      } else {
        // dir in (0, pi) rad
        return false;
      }
    }
  }
}


float TextureIndexOnLineOfRay(const glm::vec2& line_head, const glm::vec2& line_tail,
                              const glm::vec2& ray_pos, const glm::vec2& ray_dir) {
  // If ray and segment are parallel, texture can be computed in a more efficient way
  if (AreParallel(ray_dir, line_head - line_tail)) {
    // Only when also collinear when parallel, texture index will be finite
    if (AreCollinear(ray_pos, line_head, line_tail)) {

      // Ray can now either point towards the head, in which case is guaranteed to intersect with head
      if (glm::dot(ray_dir, line_head - ray_pos) > 0) {
        return 0;
      }
      // Or point way from head, in which case index should simply be distance to the head
      return glm::length(ray_pos - line_head);

    } else {
      // If not collinear, will ray never intersects with segment, and so index also is infinite.
      return std::numeric_limits<float>::infinity();
    }
  }

  // Ignore true intersection case. Negative intersection is also valid

  // Texture in any other regular cases are simply distance of head to ray.
  //  This can be accomplished easily by treating segment now as ray and ray as line and outsourcing to util method.
  return GetRayToLineDistance(ray_pos, ray_pos + ray_dir,
                              line_head, line_tail - line_head);
}

// Rotate counter clockwise
glm::vec2& FastRotate(glm::vec2& vec, const float rotation_cos, const float rotation_sin) {
  float x =  vec.x * rotation_cos + vec.y * rotation_sin;
  float y = -vec.x * rotation_sin + vec.y * rotation_cos;

  vec.x = x;
  vec.y = y;

  return vec;
}

// end of Geometric Utilities ==========================================================================================


} // namespace room_explorer