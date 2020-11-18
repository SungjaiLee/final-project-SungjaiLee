//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_H

#include "wall.h"

#include <set>

namespace room_explorer {


enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest
};

Direction OppositeDirection(Direction dir) {
  switch (dir) {
    case kNorth:
      return kSouth;
    case kSouth:
      return kNorth;
    case kEast:
      return kWest;
    case kWest:
      return kEast;
  }
}

class Room {
private:
  Room* north_ = nullptr;
  Room* east_ = nullptr;
  Room* south_ = nullptr;
  Room* west_ = nullptr;

  /**
   * Contains wall layout information.
   */
  struct Layout {
    std::set<Wall>  walls_;
  };

  Layout layout_;

  /**
   * Generate a new room
   * @return
   */
  Room* GenerateRoom();

  //TODO consider making it referecne to pointer so we can modify it
  Room* GetRoomPointer(Direction dir);

  /**
   * Link the room in the given direction.
   * Thge other room also will be linked to this room by the opposite direction
   * Returns false if room in the given direction was already populated.
   * @param dir Diretion of this room where room_p will be connected.
   * @param room_p Should not be null
   * @return
   */
  bool LinkRoom(Direction dir, Room* room_p);

  bool CutLink(Direction dir);

public:
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
