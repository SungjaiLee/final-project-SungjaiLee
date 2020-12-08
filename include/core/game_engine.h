//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H
#define NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H

#define WALL_MARGIN 0.01f

#include <core/room.h>

#include <cinder/gl/gl.h>

namespace room_explorer {

/**
 * Holds the pointer to current room, and player position and orientation.
 * Need to be able to call ray-hits and send package of ray-hits to outter sources
 */
class GameEngine {
  // Map Variables ====================================================
  RoomFactory factory_;
  Room* current_room_;
  // End of Map Variables =============================================

  // Player orientation Variables =====================================
  glm::vec2 current_position_;
  glm::vec2 view_direction_;
  // End of Player orientation Variables ==============================

public:
 /**
  * Generate a new Game Engine from given room-template path.
  * Current Room is set to template of "entry"
  * If Initial entry position is also given set current position as such.
  *     Otherwise, position is placed in the center of the room.
  * @param room_template_path Path to json file describing the factory
  *     that will handle generation of the entire map.
  */
  GameEngine(const std::string& room_template_path);

   /**
    * Vector of HitPackages in the direction of left-most to right-most strips of the vision range.
    * Essentially 2D parameterization of map in the vision field.
    * Provides enough information to perform ray-casting at any complexity.
    * @param cos Cosine of the angle of each angle between rays.
    * @param sin Sine of the angle of each angle between rays.
    * @param half_resolution Number of rays in each direction of main direction.
    * @param range_distance Maximum distance which the ray can detect intersection with an element.
    * @return Summary of all the hits in the given range.
    */
  std::vector<HitPackage> GetVision(float cos, float sin, size_t half_resolution, float range_distance);

  /**
   * Rotation the view direction by given angle.
   * @param cos Cosine of the angle of rotation
   * @param sin Sine of the angle of rotation
   */
  void RotateDirection(float cos, float sin);

  /**
   * Move the position forwards of backwards in direction of main view.
   * If movement intersects with wall, handle collision and stop before the wall.
   * If movement passes a portal, handle room transfer.
   *    Motion can only traverse one room at a time.
   *    If 2 portals are passed at once, the second portal is treated like wall and position will
   *    stop at the second portal.
   * @param speed Speed of displacement. Can be negative for backwards motion.
   */
  void MoveForward(float speed);
};

}

#endif //NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H
