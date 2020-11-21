//
// Created by Jack Lee on 2020/11/16.
//

#include <core/room.h>

namespace room_explorer {

// Direction Enum Methods ===================================================

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

// End of Direction Enum Method =========

// Room Member handlers =========================================================


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


Room*& Room::GetLinkedRoomPointer(const Direction& direction) {
  switch (direction) {
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

Room* Room::GetLinkedRoomPointer(const Direction& direction) const {
  // This repetition is necessary to allow simple manipulation for both const and non-const context
  switch (direction) {
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

  // If either room-connection is already populated, aboard linking
  Room*& curr_link = GetLinkedRoomPointer(dir);
  if (curr_link != nullptr) {
    return false;
  }

  Room*& other_link = room_p->GetLinkedRoomPointer(OppositeDirection(dir));
  if (other_link != nullptr) {
    return false;
  }

  // Are possible because in non-const context, theses are references to pointers to actual rooms.
  curr_link = room_p;
  other_link = this;
  return true;
}


Room* Room::GetConnectedRoom(const Direction& direction) {
  Room* room = GetLinkedRoomPointer(direction);
  if (room == nullptr) {
    room = factory->GenerateRandomRoom();
    LinkRoom(direction, room);
  }
  return room;
}


bool Room::ConnectedWith(Room* other, Direction direction) const {
  if (other != GetLinkedRoomPointer(direction)) {
    return false;
  }
  if (this != other->GetLinkedRoomPointer(OppositeDirection(direction))) {
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

float Room::GetNSDoorWidth() const {
  return ns_door_width_;
}

float Room::GetEWDoorWidth() const {
  return ew_door_width_;
}

// End of Room Member handlers =====================================



//======================================================================================================================



} //namespace room_explorer