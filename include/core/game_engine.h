//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H
#define NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H

#include <core/room.h>

#include <cinder/gl/gl.h>

/**
 * Holds the current room, and position of the player.
 * //Perhaps player shoudl be its own structure/class
 */
namespace room_explorer {

/**
 * Holds the pointer to current room, and player position and orientation.
 * Need to be able to call ray-hits and send package of ray-hits to outter sources
 */
class GameEngine {
  RoomFactory factory_;

  Room* current_room_;
  glm::vec2 current_position_;
  glm::vec2 view_direction_;

public:
  /**
   * Room by default is loaded to "entry" id
   * @param room_template_path
   */
  GameEngine(const std::string& room_template_path);

  /**
   *
   * @param cos of rotatoin angle for each instance
   * @param sin Rotation value of rotation angle of each instance
   * @param resolution  number of hitpackages delievered. there will be resolution number of hitpackages to each side
   * @return 2*half_resolutoin + 1 size array
   */
  std::vector<HitPackage> GetVision(float cos, float sin, size_t half_resolution, float range_distance);

  void RotateDirection(float cos, float sin);
  /**
   * Also handmles collision
   * @param speed
   */
  void MoveForward(float speed);

};

}

#endif //NONEUCLIDEAN_RAY_CASTER_GAME_ENGINE_H