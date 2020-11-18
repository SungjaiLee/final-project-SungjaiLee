#include <climits>
//
// Created by Jack Lee on 2020/11/19.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_H

#include <core/wall.h>

#include <set>

namespace room_explorer {

enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest
};

Direction OppositeDirection(Direction direction);

class Room {
private:
  Room* north_;
  Room* south_;
  Room* east_;
  Room* west_;

  __unused float width_;
  __unused float height;

  std::set<Wall> walls;

public:
  /**
   * Returns pointer to room in the given direction.
   * @param dir
   * @return
   */
  Room*& GetRoomPointer(Direction dir);

  /**
   * Link this currnt room's portal in the given direction to the give room.
   * The corresponding room is also linekd back.
   * If either rooms are already linked in the diretion, returns fakse.
   * @param dir
   * @param room_p
   * @return
   */
  bool LinkRoom(Direction dir, Room* room_p);


  friend class RoomFactory;
};



}

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
