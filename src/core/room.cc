//
// Created by Jack Lee on 2020/11/16.
//

#include <core/room.h>

namespace room_explorer {

Direction OppositeDirection(const Direction& direction) {
  switch (direction) {
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

Room*& Room::GetRoomPointer(const Direction& dir) {
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

Room* Room::GetRoomPointer(const Direction& dir) const {
  // This repetition is necessary to allow simple manipulation for both const and non-const context
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



bool Room::LinkRoom(const Direction& dir, Room* room_p) {

//  if (GetRoomPointer(dir) != nullptr) {
//    // Room in the direction already linked. should not override
//    return false;
//  }
//
//  Direction opposite = OppositeDirection(dir);
//  if (room_p->GetRoomPointer(opposite) != nullptr) {
//    return false;
//  }
//
//  switch (dir) {
//    case kNorth:
//      north_ = room_p;
//      break;
//    case kSouth:
//      south_ = room_p;
//      break;
//    case kEast:
//      east_ = room_p;
//      break;
//    case kWest:
//      west_ = room_p;
//      break;
//  }
//
//  switch(opposite) {
//    case kNorth:
//      room_p->north_ = this;
//      break;
//    case kSouth:
//      room_p->south_ = this;
//      break;
//    case kEast:
//      room_p->east_ = this;
//      break;
//    case kWest:
//      room_p->west_ = this;
//      break;
//  }
//  return true;

  Room*& curr_link = GetRoomPointer(dir);
  if (curr_link != nullptr) {
    return false;
  }

  Room*& other_link = room_p->GetRoomPointer(OppositeDirection(dir));
  if (other_link != nullptr) {
    return false;
  }

  curr_link = room_p;
  other_link = this;
  return true;
}

float Room::GetWidth() const {
  return width_;
}

float Room::GetHeight() const {
  return height_;
}

size_t Room::GetWallCount() const {
  return walls.size();
}

const std::set<Wall>& Room::GetWalls() const {
  return walls;
}

Room* Room::GetConnectedRoom(const Direction& direction) {
  Room* room = GetRoomPointer(direction);
  if (room == nullptr) {
    room = factory->GenerateRandomRoom();
    LinkRoom(direction, room);
  }
  return room;
}

bool Room::ConnectedWith(Room* other, Direction direction) const {
  if (other != GetRoomPointer(direction)) {
    return false;
  }
  if (this != other->GetRoomPointer(OppositeDirection(direction))) {
    return false;
  }
  return true;
}

bool Room::ConnectedWith(Room* other) const {
  return  ConnectedWith(other, kNorth)  ||
          ConnectedWith(other, kSouth)  ||
          ConnectedWith(other, kEast)   ||
          ConnectedWith(other, kWest);
}



//======================================================================================================================



} //namespace room_explorer