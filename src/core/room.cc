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
    default:
      throw exceptions::InvalidDirectionException();
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
    default:
      throw exceptions::InvalidDirectionException();
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
    default:
      throw exceptions::InvalidDirectionException();
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

Direction Room::GetSideHit(const glm::vec2& pos, const glm::vec2& dir, bool point_inclusive) const {
  // what if outside of room?

  //assume dir is non-zero
  if (point_inclusive) {
    bool north = FloatingPointApproximation(dir.y, height_);
    bool south = FloatingPointApproximation(dir.y, 0);
    bool east = FloatingPointApproximation(dir.x, width_);
    bool west = FloatingPointApproximation(dir.y, 0);

    //if corner, we will prioritieze the walls by "rotation" the corners clockwise
    //  NE corner will be north
    //  SE will be EAST
    //  WS will be south
    //  NW will be west

    // This means that the edge is north if not west and north
    // east is not north adn east
    // south if not east and south
    // west if not sotuh and west

    if (!west && north) {
      return kNorth;
    }
    if (!north && east) {
      return kEast;
    }
    if (!east && south) {
      return kSouth;
    }
    if (!south && west) {
      return kWest;
    }
  }

  //check if in room
  if (!WithinRoom(pos, true)) {
    // if not within a room, we will not define any direction to the ray
    throw exceptions::InvalidDirectionException();
  }
  // This requires that the ray is within the room, and therefore will guarentee hit a specific wall

  // if point if not inclusive, have to
  if (dir.x < 0) {
    // can only be west
    if (dir.y < 0) {
      //coult be south
      // must check against South West (0, 0)
      float corner_slope = (-pos.y) / (-pos.x); // valid because we know the pos to not be 0
      float dir_slope = dir.y / dir.x;

      if (dir_slope < corner_slope) {
        return kWest;
      } else {
        // if hits the corcner, consdiered south
        return kSouth;
      }

    } else if (dir.y > 0) {
      //could be north
      // Must check against North West (0, height)
      float corner_slope = (height_ - pos.y) / (-pos.x); // valid because we know the pos to not be 0
      float dir_slope = dir.y / dir.x;


      //equal, west
      if (dir_slope > corner_slope) {
        return kNorth;
      } else {
        // if corner hit, this is a west corner
        return kWest;
      }

    } else {
      //dir.y == 0, not moving up or down
      return kWest;
    }

  } else if (dir.x > 0) {
    // can only be east
    if (dir.y < 0) {
      //coult be south
      // must check against south east corner (width, 0)


      float corner_slope = (-pos.y) / (width_ - pos.x); // valid because we know the pos to not be 0
      float dir_slope = dir.y / dir.x;

      if (dir_slope < corner_slope) {
        return kSouth;
      } else {
        return kEast;
      }

    } else if (dir.y > 0) {
      //could be north
      // must check agsint east north (width, height)

      float corner_slope = (height_ - pos.y) / (width_ - pos.x); // valid because we know the pos to not be 0
      float dir_slope = dir.y / dir.x;

      if (dir_slope < corner_slope) {
        return kEast;
      } else {
        return kNorth;
      }

    } else {
      //dir.y == 0 not moving up or down
      return kEast;
    }

  } else {
    // can only be south or north
    if (dir.y > 0) {
      // must be south
      return kSouth;
    }
    if (dir.y < 0) {
      // must be north
      return kNorth;
    }
  }

  throw exceptions::InvalidDirectionException();
}

bool Room::WithinRoom(const glm::vec2& pos, bool wall_inclusive) const {
  if (!wall_inclusive) {
    return (pos.x > 0) && (pos.x < width_) && (pos.y > 0) && (pos.y < height_);
  } else {
    return WithinRoom(pos, false) || (FloatingPointApproximation(pos.x, 0)
                                              &&  FloatingPointApproximation(pos.x, width_)
                                              &&  FloatingPointApproximation(pos.y, 0)
                                              &&  FloatingPointApproximation(pos.y, height_));
  }
}

// End of Room Member handlers =====================================



//======================================================================================================================



} //namespace room_explorer