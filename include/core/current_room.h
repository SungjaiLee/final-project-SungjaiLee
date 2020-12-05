//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H

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
class CurrentRoom {
  __unused Room* current_room_;
  __unused glm::vec2 current_position;
  __unused glm::vec2 main_view_direction;

  __unused const std::string room_template_path_;

  __unused RoomFactory factory_;

public:
  /**
   * Room by default is loaded to "entry" id
   * @param room_template_path
   */
  CurrentRoom(const std::string& room_template_path);

  /**
   *
   * @param cos of rotatoin angle for each instance
   * @param sin Rotation value of rotation angle of each instance
   * @param resolution  number of hitpackages delievered. there will be resolution number of hitpackages to each side
   * @return 2*half_resolutoin + 1 size array
   */
  std::vector<HitPackage> GetVision(float cos, float sin, size_t half_resolution, float range_distance);
};

}

#endif //NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H
