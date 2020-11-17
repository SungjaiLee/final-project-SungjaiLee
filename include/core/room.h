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
  __attribute__((unused)) Room* north_ = nullptr;
  __attribute__((unused)) Room* east_ = nullptr;
  __attribute__((unused)) Room* south_ = nullptr;
  __attribute__((unused)) Room* west_ = nullptr;

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
  Room* GetRoomPointer(Direction dir) {
    switch (dir) {
      case kNorth:
        return north_;
      case kSouth:
        return south_;
      case kEast:
        return east_;
      case kWest:
        return west_;
    }
  }

  /**
   * Link the room in the given direction.
   * Thge other room also will be linked to this room by the opposite direction
   * Returns false if room in the given direction was already populated.
   * @param dir Diretion of this room where room_p will be connected.
   * @param room_p Should not be null
   * @return
   */
  bool LinkRoom(Direction dir, Room* room_p) {

    if (GetRoomPointer(dir) != nullptr) {
      // Room in the direction already linked. should not override
      return false;
    }

    Direction opposite = OppositeDirection(dir);
    if (room_p->GetRoomPointer(opposite) != nullptr) {
      return false;
    }

    switch (dir) {
      case kNorth:
        north_ = room_p;
        break;
      case kSouth:
        south_ = room_p;
        break;
      case kEast:
        east_ = room_p;
        break;
      case kWest:
        west_ = room_p;
        break;
    }

    switch(opposite) {
      case kNorth:
        room_p->north_ = this;
        break;
      case kSouth:
        room_p->south_ = this;
        break;
      case kEast:
        room_p->east_ = this;
        break;
      case kWest:
        room_p->west_ = this;
        break;
    }
    return true;
  }

  bool CutLink(Direction dir);

public:
  Room& GetRoom(Direction direction) {

    Room* destination = GetRoomPointer(direction);

    if (destination == nullptr) {
      destination = GenerateRoom();
      //Link
    }

    return *destination;
  }
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
