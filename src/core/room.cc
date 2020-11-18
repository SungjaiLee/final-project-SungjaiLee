//
// Created by Jack Lee on 2020/11/16.
//

#include <core/room.h>


namespace room_explorer {

Room* Room::GetRoomPointer(Direction dir) {
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

bool Room::LinkRoom(Direction dir, Room* room_p) {

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

//======================================================================================================================



} //namespace room_explorer