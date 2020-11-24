//
// Created by Jack Lee on 2020/11/18.
//

#include <core/wall.h>

namespace room_explorer {

void from_json(const json& json, Wall& wall) {
  float h_x = json.at("head_x");
  float h_y = json.at("head_y");
  float t_x = json.at("tail_x");
  float t_y = json.at("tail_y");
  wall.head_ = glm::vec2(h_x, h_y);
  wall.tail_ = glm::vec2(t_x, t_y);
}

const glm::vec2 &Wall::GetHead() const {
  return head_;
}

const glm::vec2 &Wall::GetTail() const {
  return tail_;
}

bool Wall::operator<(const Wall& wall) const {
  //!! doesn't matter what it is actually comparing, just need a valid comparision for them to work in a set
  return this < &wall;

  //First check by actual wall length,
  //then check by head length,
  //then by tail

  //TODO find some clever maths
}

bool Wall::operator==(const Wall& wall) const {
  // True if and only if both head and tail is equal
  return head_ == wall.head_ && tail_ == wall.tail_;
}

Wall::Wall(const glm::vec2& head, const glm::vec2& tail) {
  head_ = head;
  tail_ = tail;
}

float Wall::Distance(const glm::vec2& pos, const float angle) const {
  if (IntersectsWith(pos, angle)) {
    return PureDistance(pos, angle);
  } else {
    return -1;
  }
}


//TODO take in normal direction vector instead of angle, easier to use dot than trig. Calculation already uses trig to obtain direction vector anyway
float Wall::PureDistance(const glm::vec2& pos, const float angle) const {
  if (head_ == tail_) {
    if (head_ == pos) {
      return 0;
    }
  }

  glm::vec2 diff = head_ - tail_;


  // perpendicular to head - pos, it is head-pos rotated by 90
  glm::vec2 m(head_.y - pos.y, pos.x - head_.x);

  if (diff == glm::vec2(0, 0)) {
    // point wall, need different handling
    // if angle hits the point, its distance of m,
    // TODO technically does not make sense to have a distance, so it is completely valid to assume distance is to the point wall
    return glm::length(m);
  }

  float r = glm::dot(diff, m);
  // if r is zero, means the pos is on the line, which regardless to directoin, has distance of 0
  if (r == 0) {
    return 0;
  }

  //TODO find faster alternatives
  float cos = glm::cos(angle);
  float sin = glm::sin(angle);
  glm::vec2 dir(sin, -cos);

  float denom = glm::dot(diff, dir);
  if (denom == 0) {
    // Does not intersect. Parrellel
    return -1;
  }

  r /= denom;

  return r;
}

// Todo, convert to normal vector dot product computation instead
//  TODO:: normal computation, find normal of the direction vector
//         find dot f each head-pos and tail-pos
//         if the two dots of different sign, means that angle must be inside
//         this will then require orientation check, which can be done by checking if both dot of just direciton vectpr with wall vector is both positve, else wrong direction
bool Wall::IntersectsWith(const glm::vec2& pos, const float angle) const{
  return IntersectsWith(pos, glm::vec2(std::cos(angle), std::sin(angle)));

//  //head, tail == pos check
//  if (pos == head_ || pos == tail_) {
//    return true;
//  }
//
//  float theta_0 = GetTheta(head_ - pos);
//  float theta_1 = GetTheta(tail_ - pos);
//
//  //Swapping variables so theta_0 is less than theta_1
//  if (theta_1 < theta_0) {
//    float temp = theta_0;
//    theta_0 = theta_1;
//    theta_1 = temp;
//  }
//
//  // Check for case pos is in the line, in which case the angle to head and tail will differ by pi
//  // THis also avoids on-line but not segment check, this will only be true if pos is between the head and tail, so theta's differ exactly by pi
//  if (FloatingPointApproximation(theta_1 -theta_0, M_PI)) {
//    return true;
//  }
//
//  // require head/tail hit check
//  // use knuth approximation
//  // TODO check if necessary
//  if (FloatingPointApproximation(angle, theta_0) || FloatingPointApproximation(angle, theta_1)) {
//    return true;
//  }
//
//
//  if (angle < theta_0 || angle > theta_1) {
//    return false;
//  }
//
//  return true;
}

bool Wall::IntersectsWith(const glm::vec2& pos, const glm::vec2& dir) const {
  // If position if head or tail itself, will be considered to have intersected with wall as a whole
  if (head_ == pos || tail_ == pos) {
    return true;
  }

  // Calculation is based on relative position to head and tail from the position of ray-head
  glm::vec2 head = head_ - pos;
  glm::vec2 tail = tail_ - pos;

  // complementarty normals
  glm::vec2 tail_c(tail.y, -tail.x);

  float a = glm::dot(head, tail_c);
  if (FloatingPointApproximation(a, 0)) {
    // head and tail are colinear


    if (glm::dot(head, tail) < 0) {
      // theta is pi, meaning pos is between the head and tail
      // definitely intersects
      return true;
    } else {
      // theta is 0
      // head and tail are colinear with the pos, or worse a point wall
      glm::vec2 dir_c(dir.y, -dir.x);

      if (FloatingPointApproximation(glm::dot(head, dir_c), 0)) {
        if (glm::dot(head, dir) > 0) {
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
    glm::vec2 dir_c(dir.y, -dir.x);
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



//  const float h_parallel = glm::dot(head, dir);
//  const float t_parallel = glm::dot(tail, dir);
//
//  // if the head and tail are parrellel, must be on the line,
//  if (FloatingPointApproximation(glm::dot(head, glm::vec2(tail.y, -tail.x)), 0)
//    && h_parallel * t_parallel < 0) {
//    return true;
//  }
//
//
//  // Direction of normal is irrelevant. Simply required that it is normal to direction
//  const glm::vec2 normal_dir(dir.y, -dir.x);
//  float h_normal = glm::dot(head, normal_dir);
//  float t_normal = glm::dot(tail, normal_dir);
//
//
//  if (h_parallel < 0 || t_parallel < 0) {
//    // if both negative. dir cannot be between the head, tail vector
//    // must not intersect
//
//  if ((FloatingPointApproximation(h_normal, 0) && h_parallel > 0 )|| (FloatingPointApproximation(t_normal, 0) && t_parallel > 0)) {
//    // if either head, tail same direction as dir, means the dir is pointing straight at hed or tail, thus intersects
//    return true;
//  }
//
//    return false;
//  }
//
//  /* The ray intersects with wall if the direction vector is "between" the head and tail vector.
//   * This will obviously require that the normal component of direction vector, when projected to head and tail,
//   *  to point "inwards" and therefore in opposite directions.
//   * For this, we need to find the complement normal vector of the given direction, and its dot product with
//   *  each tail and head will give this normal component. The direction of the normal does not matter, because
//   *  we only need that this product be of opposite sign.
//   * This holds also true if the direction was between the head and tail in the negative direction.
//   *  To avoid this case, we will eliminate the case there
//   */
//  if (h_normal * t_normal < 0) {
//    return true;
//  }
//
//  //!!TODO massive fix needed
//  //   Does nto hold absolutley true
//
//
//  // TODO
//  if ((FloatingPointApproximation(h_normal, 0) && h_parallel > 0 )|| (FloatingPointApproximation(t_normal, 0) && t_parallel > 0)) {
//    // if either head, tail same direction as dir, means the dir is pointing straight at hed or tail, thus intersects
//    return true;
//  }
//
////  if (FloatingPointApproximation(h_normal, 0) && FloatingPointApproximation(t_normal, 0)) {
////    // means that pos in the same line as head, tail
////    // need to check if pos inbetween the two
////    if (h_parallel * t_parallel < 0) {
////      // means the parrellel to head and tail is in diffferent diretion, suggesting inbetween
////      return true;
////    }
////    return false;
////  }
//
//  return false;
}

// static unit methods =================================================
float GetTheta(const glm::vec2& vec) {
  //TODO need be funiction
  float theta = std::atan2(vec.y, vec.x); //
  if (theta < 0) {
    theta = 2 * M_PI + theta;
  }
  return theta;
}

bool FloatingPointApproximation(float a, float b, float epsilon) {
  float diff = std::abs(a - b);
  if (a == 0 || b == 0) {
    if (diff > epsilon) {
      return false;
    } else {
      return true;
    }
  }

  // waeaker approximation
  if (diff / std::abs(a) <= epsilon) {
    return true;
  }
  if (diff / std::abs(b) > epsilon) {
    return true;
  }

  return false;
}

} // namespace room_explorer