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

    if (Collinear(pos, head_, tail_)) {
      //in-line. Pure distance will give 0, whereas a segment needs to yield distance to head or tail
      // Intserection assumes that direction is in correct orientation
      // return the closer of the two

      // need to be sure that position is outside of the wall segment
      if (glm::dot(head_ - pos, tail_ - pos) > 0) {
        return std::min(glm::length(head_ - pos), glm::length(tail_ - pos));
      } else {
        return 0;
      }
    }

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
}

bool Wall::IntersectsWith(const glm::vec2& pos, const glm::vec2& dir) const {

  return RayIntersectsWithSegment(head_, tail_, pos, dir);

//  // Trivial Cases
//  // If position if head or tail itself, will be considered to have intersected with wall as a whole
//  if (head_ == pos || tail_ == pos) {
//    return true;
//  }
//
//  // Calculation is based on relative position to head and tail from the position of ray-head
//  glm::vec2 head = head_ - pos;
//  glm::vec2 tail = tail_ - pos;
//
//  // complementarty normals
//  glm::vec2 tail_c(tail.y, -tail.x);
//
//  float a = glm::dot(head, tail_c);
//  if (FloatApproximation(a, 0)) {
//    // head and tail are collinear
//
//
//    if (glm::dot(head, tail) < 0) {
//      // theta is pi, meaning pos is between the head and tail
//      // definitely intersects
//      return true;
//    } else {
//      // theta is 0
//      // head and tail are collinear with the pos, or worse a point wall
//      glm::vec2 dir_c(dir.y, -dir.x);
//
//      if (FloatApproximation(glm::dot(head, dir_c), 0)) {
//        if (glm::dot(head, dir) > 0) {
//          // direction is 0
//          return true;
//        } else {
//          // direction is pi
//          //  wrong direction
//          return false;
//        }
//      } else {
//        // direction not 0 or pi
//        // not facing either of correct orrientaiotn
//        return false;
//      }
//    }
//
//  } else {
//    glm::vec2 dir_c(dir.y, -dir.x);
//    float b = glm::dot(head, dir_c);
//    // not colinear, either in I, II quad or III, IV quad
//    if (a > 0) {
//      // theta in (0, pi)
//      if (b > 0 || FloatApproximation(b, 0)) {
//        // dir in [0, pi]
//
//        float c = glm::dot(tail, dir_c);
//        if (c < 0 || FloatApproximation(c, 0)) {
//          // dir <= theat
//          // dir in [0, theta]
//          return true;
//        } else {
//          return false;
//        }
//
//      } else {
//        // dir in (pi, 2pi)
//        return false;
//      }
//    } else {
//      // theta in (pi, 2pi)
//
//      if (b < 0 || FloatApproximation(b, 0)) {
//        // dir in [pi, 2pi]
//        float c = glm::dot(tail, dir_c);
//
//        if (c > 0 || FloatApproximation(c, 0)) {
//          // dir >= theta
//          // dir in [theta, 2pi]
//          return true;
//        } else {
//          return false;
//        }
//
//      } else {
//        // dir in (0, pi)
//        return false;
//      }
//    }
//  }
}

Hit Wall::GetWallHit(const glm::vec2& pos, const glm::vec2& dir) const {

  float wall_intersection_distance = Distance(pos, dir);
  if (wall_intersection_distance < 0) {
    return Hit(); // invalid hit
  }

  // treating ray now as line, and line now as ray, we can calculate the texture index
  float texture_index = TextureIndex(pos, dir);

  return Hit(wall_intersection_distance, kWall, texture_index);
}

float Wall::TextureIndex(const glm::vec2& pos, const glm::vec2& dir) const {

  return TextureIndexOnLineOfRay(head_, tail_,
                                 pos, dir);

  // if collinear, do not consider direction/parrellel ray adn line
  // just return distance to pos, where true intersecction occurs
  // must check if parrelel, if it is, then only valid texture distance is if also collinear and distancr to head


  //!This shouyld also handle on-wall case
//  if (Parallel(dir, head_ - tail_)) {
//    if (Collinear(pos, head_, tail_)) {
//        // If ray is already set to intsersect with the wall, the distance of head to ray is 0
//        if (glm::dot(dir, head_ - pos) > 0) {
//          return 0;
//        }
//
//      // this suggests that ray is pointing away from the head, meaning that the distance must be actual ray-head distance
//      return glm::length(pos - head_);
//    }
//  }
//
//
//  return GetRayToLineDistance(pos, pos + dir, head_, tail_ - head_);
}

// static unit methods =================================================

} // namespace room_explorer