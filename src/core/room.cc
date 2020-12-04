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
  return walls_.size();
}

const std::set<Wall>& Room::GetWalls() const {
  return walls_;
}
// End of elementary getters ====================================================================


// Room Connectivity Functions ==================================================================
bool Room::LinkRoom(const Direction& direction, Room* room_p) {
  // If either room-connection is already populated, aboard linking
  Room*& curr_link = GetLinkedRoomPointer(direction);
  if (curr_link != nullptr) {
    return false;
  }

  Room*& other_link = room_p->GetLinkedRoomPointer(OppositeDirection(direction));
  if (other_link != nullptr) {
    return false;
  }

  // Update connected rooms. Assignment possible because link-pointers are references to actual members.
  curr_link = room_p;
  other_link = this;
  return true;
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

Room* Room::GetConnectedRoom(const Direction& direction) {
  Room* room = GetLinkedRoomPointer(direction);
  // If room is not yet linked, indicated by link being null, generate a new room from factory and link them
  if (room == nullptr) {
    room = factory->GenerateRandomRoom();
    LinkRoom(direction, room);
  }
  return room;
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

glm::vec2 Room::GetHead(Direction direction, bool of_portal) const {
  return of_portal  ? GetPortalHead(direction)
                    : GetWallHead(direction);
}

glm::vec2 Room::GetTail(Direction direction, bool of_portal) const {
  return of_portal  ? GetPortalTail(direction)
                    : GetWallTail(direction);
}
// End of getters ==========================================================

bool Room::WithinRoom(const glm::vec2& pos, bool wall_inclusive) const {

  if (!wall_inclusive){
    // Just set edges as always being un-intersected for the same result.
    return WithinRoom((pos.x > 0) && (pos.x < width_),
                      (pos.y > 0) && (pos.y < height_),
                      false,
                      false);
  } else {
    return WithinRoom((pos.x > 0) && (pos.x < width_),
                      (pos.y > 0) && (pos.y < height_),
                      FloatApproximation(pos.x, 0) || FloatApproximation(pos.x, width_),
                      FloatApproximation(pos.y, 0) || FloatApproximation(pos.y, height_));
  }
}

Direction Room::GetSideHit(const glm::vec2& ray_pos,
                           const glm::vec2& ray_dir,
                           bool point_inclusive) const {
  // Edge border cases are checked first.
  bool touch_north = FloatApproximation(ray_pos.y, height_);
  bool touch_south = FloatApproximation(ray_pos.y, 0);
  bool touch_east = FloatApproximation(ray_pos.x, width_);
  bool touch_west = FloatApproximation(ray_pos.x, 0);

  // Necessary boolean characteristics for private within-wall check
  bool strictly_within_x = (ray_pos.x > 0) && (ray_pos.x < width_);
  bool strictly_within_y = (ray_pos.y > 0) && (ray_pos.y < height_);

  // Outsource within-room calculation to character-wise within check
  if (!WithinRoom(strictly_within_x,
                  strictly_within_y,
                  touch_west || touch_east,
                  touch_north || touch_south)) {
    throw exceptions::InvalidDirectionException();
  }

  if (touch_north || touch_south || touch_east || touch_west) {
    // If point-inclusive, means that current wall on which the ray begins on should be considered a hit.
    if (point_inclusive) {
      // Heads are included as being hart of the wall, but tails aren't.
      //  Tails are detected by being at a corner, indicated by 2 invalid touch-condition.
      if (!touch_west && touch_north) {
        return kNorth;
      }
      if (!touch_north && touch_east) {
        return kEast;
      }
      if (!touch_east && touch_south) {
        return kSouth;
      }
      if (!touch_south && touch_west) {
        return kWest;
      }

    } else {
      // If not point inclusive, but still on-wall, there are few more cases that can be immediately handled
      //  by simple direction-check

      // If on wall and ray points out-wards, ray will never hit a wall, and should throw exception.
      // If on wall and  ray points directly parallel to the wall, ray will intersect with the current wall.

      if (touch_north) {
        if (ray_dir.y > 0) {
          throw exceptions::InvalidDirectionException();
        }
        // If y = 0, ray must move only in x-direction, directly parallel to north wall.
        if (FloatApproximation(ray_dir.y, 0)) {
          return kNorth;
        }
      }
      if (touch_south) {
        if (ray_dir.y < 0) {
          throw exceptions::InvalidDirectionException();
        }
        // If y = 0, ray must move only in x-direction, directly parallel to south wall.
        if (FloatApproximation(ray_dir.y, 0)) {
          return kSouth;
        }
      }
      if (touch_east) {
        if (ray_dir.x > 0) {
          throw exceptions::InvalidDirectionException();
        }
        // If x = 0, ray must move only in y-direction, directly parallel to east wall.
        if (FloatApproximation(ray_dir.x, 0)) {
          return kEast;
        }
      }
      if (touch_west) {
        if (ray_dir.x < 0) {
          throw exceptions::InvalidDirectionException();
        }
        // If x = 0, ray must move only in y-direction, directly parallel to west wall.
        if (FloatApproximation(ray_dir.x, 0)) {
          return kWest;
        }
      }
      // Else, ray is either pointing into the current wall, and nor pointing out into invalid void.
      //  Rest can be computed through non-on-wall conditions.
    }
  }

  // All other cases can be handled by observing cardinal direction, then slope of ray-dir/
  // Basic concept: if the ray is pointing to the right, there should be no need to check for left-wall.
  //  Continue eliminating possibilities through this process.
  //  When only two adjacent walls are left as possibilities, compare slope of direction to needed slope to the corner.

  if (ray_dir.x < 0) { // Must be in westwards direction.
    if (ray_dir.y < 0) { // Must be southern direction.
      // Check slope against SW corner (0, 0)
      float corner_slope = (-ray_pos.y) / (-ray_pos.x); // ray.pos = 0 is eliminated through on-wall checks.
      float dir_slope = ray_dir.y / ray_dir.x;

      // SW corner is part of Southern Wall.
      //  If slope is less than slope to corner, must mean Western wall is strictly hit.
      if (dir_slope < corner_slope) {
        return kWest;
      } else {
        // Includes both strictly southern, and also hitting southern-head.
        return kSouth;
      }

    } else if (ray_dir.y > 0) { // Must be northern direction.
      // Check slope against NW corner (0, height)
      float corner_slope = (height_ - ray_pos.y) / (-ray_pos.x); // ray.pos = 0 is eliminated through on-wall checks.
      float dir_slope = ray_dir.y / ray_dir.x;

      // NW corner is part of Western Wall.
      //  If slope is greater than slope to corner, must mean Northern wall is strictly hit.
      if (dir_slope < corner_slope) {
        return kNorth;
      } else {
        // Includes both strictly western, and also hitting western-head.
        return kWest;
      }

    } else {
      // If y = 0, is not moving up or down, meaning must be moving towards the western wall.
      return kWest;
    }

  } else if (ray_dir.x > 0) { // Must be in eastern direction.
    if (ray_dir.y < 0) { // Must be southern direction.
      // Check slope against SE corner (width, 0)
      float corner_slope = (-ray_pos.y) / (width_ - ray_pos.x); // width - x = 0 is eliminated through on-wall checks.
      float dir_slope = ray_dir.y / ray_dir.x;

      // SE corner is part of Eastern Wall.
      //  If slope is greater than slope to corner, must mean Southern wall is strictly hit.
      if (dir_slope < corner_slope) {
        return kSouth;
      } else {
        // Includes both strictly eastern, and also hitting eastern-head.
        return kEast;
      }

    } else if (ray_dir.y > 0) { // Must be northern direction.
      // Check slope against NE corner (width, height)
      float corner_slope = (height_ - ray_pos.y) / (width_ - ray_pos.x);
      // width - x = 0 is eliminated through on-wall checks.
      float dir_slope = ray_dir.y / ray_dir.x;

      // NE corner is part of Northern Wall.
      //  If the slope is less than slope to the corner, must mean Eastern wall is strictly hit.
      if (dir_slope < corner_slope) {
        return kEast;
      } else {
        // Includes both strictly northern, and also hitting northern-head.
        return kNorth;
      }

    } else {
      // If y = 0, is not moving up or down, meaning must move towards the eastern wall.
      return kEast;
    }

  } else {
    // If x = 0, cannot hit either Eastern or Western walls. Only Northern and Southern and possible
    if (ray_dir.y < 0) {
      // Ray moving down, must hit southern wall.
      return kSouth;
    }
    if (ray_dir.y > 0) {
      // Ray moving up, must hit northern wall.
      return kNorth;
    }
  }

  // Suggests ray is a zero-ray. Invalid direction.
  throw exceptions::InvalidDirectionException();
}

float Room::GetRoomWallHitDistance(const Direction& direction,
                                   const glm::vec2& ray_pos,
                                   const glm::vec2& ray_dir) const {
  // Out-sourced to general util function. Line is defined by head and tail of wall, on which the ray itself lies on.
  return GetRayToLineDistance(GetWallHead(direction),
                              GetWallTail(direction),
                              ray_pos, ray_dir);
}

bool Room::RayHitsPortal(const Direction& direction,
                         const glm::vec2& ray_pos,
                         const glm::vec2& ray_dir) const {
  // Out-source to general util method. Retrieves if ray intersects with the segment defined by portal head and tail.
  return RayIntersectsWithSegment(GetPortalHead(direction),
                                  GetPortalTail(direction),
                                  ray_pos, ray_dir);
}

float Room::GetWallTextureIndex(const Direction& direction, bool of_portal,
                                const glm::vec2& ray_pos, const glm::vec2& ray_dir) const {
  // Out-sourced to general util texture-index getter.
  // Retrieves distance ot intersection from the head of either portal or wall in given direction.
  return TextureIndexOnLineOfRay(GetHead(direction, of_portal),
                                 GetTail(direction, of_portal),
                                 ray_pos,
                                 ray_dir);
}

Hit Room::GetPrimaryWallHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir, bool wall_inclusive) const {
  // dummy for reference
  __unused Direction direction;
  return GetPrimaryWallHit(ray_pos, ray_dir, direction, wall_inclusive);
}

Hit Room::GetPrimaryWallHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir, Direction& direction, bool wall_inclusive) const {
  try {
    // If invalid direction, will throw
    direction = GetSideHit(ray_pos, ray_dir, wall_inclusive);
    bool is_portal{RayHitsPortal(direction, ray_pos, ray_dir)};

    return {GetRoomWallHitDistance(direction, ray_pos, ray_dir),
            is_portal ? kPortal : kRoomWall,
            GetWallTextureIndex(direction, is_portal, ray_pos, ray_dir)};
  } catch (const exceptions::InvalidDirectionException& e) {
    return {}; // Return invalid hit package
  }
}



HitPackage Room::CurrentRoomPackage(const glm::vec2& ray_pos, const glm::vec2& ray_dir, float visible_range, bool point_inclusive) const {
  HitPackage package;

  Hit exclusive_primary_hit{GetPrimaryWallHit(ray_pos, ray_dir, false)};

  if (!exclusive_primary_hit.IsNoHit() && exclusive_primary_hit.WithinDistance(visible_range)) {
    package.AddHit(exclusive_primary_hit);

    // only this exclusive primary hit can be a portal
  }

  if (point_inclusive) {
    // check for inclusive hit
    Hit inclusive_primary_hit{GetPrimaryWallHit(ray_pos, ray_dir, true)};

    // only include different hits
    if (!inclusive_primary_hit.IsNoHit() &&
        inclusive_primary_hit.WithinDistance(visible_range) &&
        exclusive_primary_hit != inclusive_primary_hit) {
      package.AddHit(inclusive_primary_hit);
    }
  }


  for (const Wall& wall : walls_) {
    Hit hit{wall.GetWallHit(ray_pos, ray_dir)};
    if (hit.WithinDistance(visible_range)) {
      package.AddHit(hit);
    }
  }

  // if not point inclusive, can only be uninclusive
  return package;
}


HitPackage Room::GetVisible(const glm::vec2& ray_pos, const glm::vec2& ray_dir, float visible_range, bool point_inclusive) {
  HitPackage package;

  Direction direction;
  Hit exclusive_primary_hit{GetPrimaryWallHit(ray_pos, ray_dir, direction, false)};

  if (!exclusive_primary_hit.IsNoHit() && exclusive_primary_hit.WithinDistance(visible_range)) {
    package.AddHit(exclusive_primary_hit);

    // only this exclusive primary hit can be a portal
    if (exclusive_primary_hit.hit_type_ == kPortal) {
      // requestion from room of directio

      float& texture_shift = exclusive_primary_hit.texture_index_;

      glm::vec2 entry_pos;

      // define entry point, on the other room
      //  texture index must be from current, the other compoennt needs to be opposite
      switch (direction) {
        case kNorth:
          // entry at (nw_begin + width - texture, 0) :: on south
          entry_pos = {ns_door_begin_ + ns_door_width_ - texture_shift, 0};
          break;
        case kSouth:
          // entry at (nw_begin + texture, height) :: on north
          entry_pos = {ns_door_begin_ + texture_shift, height_};
          break;
        case kEast:
          // entry at (0, ew_begin + texture) :: on west
          entry_pos = {0, ew_door_begin_ + texture_shift};
          break;
        case kWest:
          // entry at (width, ew_begin + ew_wdith - texture) :: on east
          entry_pos = {width_, ew_door_begin_ + ew_door_width_ - texture_shift};
          break;

        case kUndefined:
          throw exceptions::InvalidDirectionException();
      }

      float new_range = visible_range - exclusive_primary_hit.hit_distance_;
      HitPackage next_room = GetConnectedRoom(direction)->GetVisible(entry_pos, ray_dir, new_range, false);

      next_room.ShiftHits(exclusive_primary_hit.hit_distance_);

      package.Merge(next_room);
    }
  }

  if (point_inclusive) {
    // check for inclusive hit
    Hit inclusive_primary_hit{GetPrimaryWallHit(ray_pos, ray_dir, true)};

    // only include different hits
    if (!inclusive_primary_hit.IsNoHit() &&
        inclusive_primary_hit.WithinDistance(visible_range) &&
        exclusive_primary_hit != inclusive_primary_hit) {
      package.AddHit(inclusive_primary_hit);
    }
  }

  for (const Wall& wall : walls_) {
    Hit hit{wall.GetWallHit(ray_pos, ray_dir)};
    if (hit.WithinDistance(visible_range)) {
      package.AddHit(hit);
    }
  }

//  auto hit_it = --package.GetHits().end();

  // if on a portal, ignore it, only find the last portal, which is what the ray will 'look into'
//  Direction direction{GetSideHit(ray_pos, ray_dir, false)};


  return package;
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
  // Returns copy of pointer rather than reference to it.
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

bool Room::WithinRoom(bool strictly_within_width, bool strictly_within_height,
                      bool width_edge, bool height_edge) const {
  if (strictly_within_width && strictly_within_height) {
    return true;
  }
  if (width_edge && height_edge) {
    return true;
  }
  if (width_edge && strictly_within_height) {
    return true;
  }
  if (strictly_within_width && height_edge) {
    return true;
  }
  return false;
}

// end of private geometric functions ===========================================================

// End of Private Room Functions =======================================================================================

} //namespace room_explorer