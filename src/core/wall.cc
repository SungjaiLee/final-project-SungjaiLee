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
  return Distance(pos, glm::vec2(std::cos(angle), std::sin(angle)));
}

float Wall::Distance(const glm::vec2& pos, const glm::vec2& dir) const {
  if (IntersectsWith(pos, dir)) {
    return GetRayToLineDistance(head_, tail_, pos, dir);
  } else {
    return -1;
  }
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
  // Trivial Cases
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

} // namespace room_explorer