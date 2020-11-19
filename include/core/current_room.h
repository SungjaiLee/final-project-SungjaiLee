//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H

#include <core/room.h>

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
};

}

#endif //NONEUCLIDEAN_RAY_CASTER_CURRENT_ROOM_H
