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
// End of Direction Enum Method =============================================


// Public Room functions ===============================================================================================

// Elementary Getters ===========================================================================
size_t Room::GetWallCount() const {
  return walls.size();
}

const std::set<Wall>& Room::GetWalls() const {
  return walls;
}
// End of elementary getters ====================================================================


// Room Connectivity Functions ==================================================================
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

bool Room::IsConnectedWith(Room* other_p, const Direction& direction) const {
  if (other_p != GetLinkedRoomPointer(direction)) {
    return false;
  }
  if (this != other_p->GetLinkedRoomPointer(OppositeDirection(direction))) {
    return false;
  }
  return true;
}

bool Room::IsConnectedWith(Room* other_p) const {
  return IsConnectedWith(other_p, kNorth) ||
         IsConnectedWith(other_p, kSouth) ||
         IsConnectedWith(other_p, kEast) ||
         IsConnectedWith(other_p, kWest);
}
// End of Connectivity Functions ================================================================


// Room Geometric Functions =====================================================================

// Getters ==================================================================
float Room::GetWidth() const {
  return width_;
}

float Room::GetHeight() const {
  return height_;
}

float Room::GetNSDoorWidth() const {
  return ns_door_width_;
}

float Room::GetEWDoorWidth() const {
  return ew_door_width_;
}

glm::vec2 Room::GetHead(Direction dir, bool of_portal) const {
  if (of_portal) {
    return GetPortalHead(dir);
  } else {
    return GetWallHead(dir);
  }
}

glm::vec2 Room::GetTail(Direction dir, bool of_portal) const {
  if (of_portal) {
    return GetPortalTail(dir);
  } else {
    return GetWallTail(dir);
  }
}
// End of getters ==========================================================

bool Room::WithinRoom(const glm::vec2& pos, bool wall_inclusive) const {
  if (!wall_inclusive) {
    return (pos.x > 0) && (pos.x < width_) && (pos.y > 0) && (pos.y < height_);
  } else {
    return WithinRoom(pos, false)
           || FloatApproximation(pos.x, 0)
           || FloatApproximation(pos.x, width_)
           || FloatApproximation(pos.y, 0)
           || FloatApproximation(pos.y, height_);
  }
}

// TODO make more efficent
Direction Room::GetSideHit(const glm::vec2& ray_pos,
                           const glm::vec2& ray_dir,
                           bool point_inclusive) const {
  // what if outside of room?

  // calculated here to allow utility outside of the following if statment
  bool north = FloatApproximation(ray_pos.y, height_);
  bool south = FloatApproximation(ray_pos.y, 0);
  bool east = FloatApproximation(ray_pos.x, width_);
  bool west = FloatApproximation(ray_pos.x, 0);
  //assume dir is non-zero
  if (point_inclusive) {

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
  if (!WithinRoom(ray_pos, false)) { // wall exclusive, because if it was touching the wall, would have been caught above
    // this eliminates uneeded on-edge pointing out while being exclusive problem

    // if not within a room, we will not define any direction to the ray

    if (!WithinRoom(ray_pos, true)) { //if not within the room wall inclusive, means totally outside of the room
      // TODO make more clean, combine and distribute to form a more cohrent method to handle this, "on thr wall" case
      throw exceptions::InvalidDirectionException();
    }

    // there is still possibility that direction could point "inwards
    //  for this, we must check here too
    //  here we will only eliminate the awful kinds
    //  only eliminate outwards pointers
    if (north) {
      if (ray_dir.y > 0) {
        throw exceptions::InvalidDirectionException();
      }
      // if does not move in y direction, must hit thew wall in the rightnext point
      if (FloatApproximation(ray_dir.y, 0)) {
        return kNorth;
      }
    }
    if (south) {
      if (ray_dir.y < 0) {
        throw exceptions::InvalidDirectionException();
      }
      if (FloatApproximation(ray_dir.y, 0)) {
        return kSouth;
      }
    }
    if (east) {
      if (ray_dir.x > 0) {
        throw exceptions::InvalidDirectionException();
      }
      if (FloatApproximation(ray_dir.x, 0)) {
        return kEast;
      }
    }
    if (west) {
      if (ray_dir.x < 0) {
        throw exceptions::InvalidDirectionException();
      }
      if (FloatApproximation(ray_dir.x, 0)) {
        return kWest;
      }
    }
  }
  // This requires that the ray is within the room, and therefore will guarentee hit a specific wall

  // if point if not inclusive, have to
  if (ray_dir.x < 0) {
    // can only be west
    if (ray_dir.y < 0) {
      //coult be south
      // must check against South West (0, 0)
      float corner_slope = (-ray_pos.y) / (-ray_pos.x); // valid because we know the pos to not be 0
      float dir_slope = ray_dir.y / ray_dir.x;

      if (dir_slope < corner_slope) {
        return kWest;
      } else {
        // if hits the corcner, consdiered south
        return kSouth;
      }

    } else if (ray_dir.y > 0) {
      //could be north
      // Must check against North West (0, height)
      float corner_slope = (height_ - ray_pos.y) / (-ray_pos.x); // valid because we know the pos to not be 0
      float dir_slope = ray_dir.y / ray_dir.x;


      //equal, west
      if (dir_slope < corner_slope) { // less because this is neagtvie slope, so less means more extreme
        return kNorth;
      } else {
        // if corner hit, this is a west corner
        return kWest;
      }

    } else {
      //dir.y == 0, not moving up or down
      return kWest;
    }

  } else if (ray_dir.x > 0) {
    // can only be east
    if (ray_dir.y < 0) {
      //coult be south
      // must check against south east corner (width, 0)


      float corner_slope = (-ray_pos.y) / (width_ - ray_pos.x); // valid because we know the pos to not be 0
      float dir_slope = ray_dir.y / ray_dir.x;

      if (dir_slope < corner_slope) {
        return kSouth;
      } else {
        return kEast;
      }

    } else if (ray_dir.y > 0) {
      //could be north
      // must check agsint east north (width, height)

      float corner_slope = (height_ - ray_pos.y) / (width_ - ray_pos.x); // valid because we know the pos to not be 0
      float dir_slope = ray_dir.y / ray_dir.x;

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
    if (ray_dir.y < 0) {
      // must be south
      return kSouth;
    }
    if (ray_dir.y > 0) {
      // must be north
      return kNorth;
    }
  }

  throw exceptions::InvalidDirectionException();
}

float Room::GetRoomWallHitDistance(const Direction& direction,
                                   const glm::vec2& ray_pos,
                                   const glm::vec2& ray_dir) const {
  return GetRayToLineDistance(GetHead(direction, false),
                              GetTail(direction, false),
                              ray_pos, ray_dir);
}

bool Room::RayHitsPortal(const Direction& direction,
                         const glm::vec2& ray_pos,
                         const glm::vec2& ray_dir) const {

  // TODO break down into simpler calculation. No need to check for unmet conditions when cases are much more specifed
  switch (direction) {

    case kNorth:
      return RayIntersectsWithSegment(glm::vec2(ns_door_begin_, height_),
                                      glm::vec2(ns_door_begin_ + ns_door_width_, height_),
                                      ray_pos, ray_dir);

    case kSouth:
      return RayIntersectsWithSegment(glm::vec2(ns_door_begin_, 0),
                                      glm::vec2(ns_door_begin_ + ns_door_width_, 0),
                                      ray_pos, ray_dir);

    case kEast:
      return RayIntersectsWithSegment(glm::vec2(width_, ew_door_begin_),
                                      glm::vec2(width_, ew_door_begin_ + ew_door_width_),
                                      ray_pos, ray_dir);

    case kWest:
      return RayIntersectsWithSegment(glm::vec2(0, ew_door_begin_),
                                      glm::vec2(0, ew_door_begin_ + ew_door_width_),
                                      ray_pos, ray_dir);

    case kUndefined:
      throw exceptions::InvalidDirectionException();
  }
}

float Room::GetWallTextureIndex(const Direction& direction, bool of_portal,
                                const glm::vec2& ray_pos, const glm::vec2& ray_dir) const {
  return TextureIndexOnLineOfRay(GetHead(direction, of_portal),
                                 GetTail(direction, of_portal),
                                 ray_pos,
                                 ray_dir);
}
// End of Room geometric functions ==============================================================

// End of public room functions ========================================================================================


// Private Room Functions ==============================================================================================

// Getters ======================================================================================
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
// End of Getters ===============================================================================


// Private Geometric Functions ==================================================================
glm::vec2 Room::GetPortalHead(Direction dir) const {
  using glm::vec2;

  switch (dir) {
    case kNorth:
      return vec2(ns_door_begin_ + ns_door_width_, height_);

    case kSouth:
      return vec2(ns_door_begin_, 0);

    case kEast:
      return vec2(width_, ew_door_begin_);

    case kWest:
      return vec2(0, ew_door_begin_ + ew_door_width_);

    case kUndefined:
      throw exceptions::InvalidDirectionException();
  }
}

glm::vec2 Room::GetPortalTail(Direction dir) const {
  using glm::vec2;

  switch (dir) {
    case kNorth:
      return vec2(ns_door_begin_, height_);

    case kSouth:
      return vec2(ns_door_begin_ + ns_door_width_, 0);

    case kEast:
      return vec2(width_, ew_door_begin_ + ew_door_width_);

    case kWest:
      return vec2(0, ew_door_begin_);

    case kUndefined:
      throw exceptions::InvalidDirectionException();
  }
}

glm::vec2 Room::GetWallHead(Direction dir) const {
  using glm::vec2;

  switch (dir) {
    case kNorth:
      return vec2(width_, height_);

    case kSouth:
      return vec2(0, 0);

    case kEast:
      return vec2(width_, 0);

    case kWest:
      return vec2(0, height_);

    case kUndefined:
      throw exceptions::InvalidDirectionException();
  }
}

glm::vec2 Room::GetWallTail(Direction dir) const {
  using glm::vec2;

  switch (dir) {
    case kNorth:
      return vec2(0, height_);

    case kSouth:
      return vec2(width_, 0);

    case kEast:
      return vec2(width_, height_);

    case kWest:
      return vec2(0, 0);

    case kUndefined:
      throw exceptions::InvalidDirectionException();
  }
}
// end of private geometric functions ===========================================================

// End of Private Room Functions =======================================================================================

} //namespace room_explorer