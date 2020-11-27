#include <climits>
//
// Created by Jack Lee on 2020/11/19.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_H

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
#include <core/room_factory.h>
#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H

#include <exceptions/invalid_direction_exception.h>

#include <core/ray.h>
#include <core/wall.h>

#include <set>

namespace room_explorer {

enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest,
  kUndefined
};

Direction OppositeDirection(const Direction& direction);


class RoomFactory;

//!!! the (0,0) will be at the SW corner
class Room {
private:
  const RoomFactory* factory;

  Room* north_;
  Room* south_;
  Room* east_;
  Room* west_;

  float width_;
  float height_;

  float ns_door_width_;
  float ew_door_width_;

  std::set<Wall> walls;

  /**
   * Returns pointer to room in the given direction.
   * @param direction
   * @return
   */
  Room*& GetLinkedRoomPointer(const Direction& direction);
  Room*  GetLinkedRoomPointer(const Direction& direction) const;

  /**
   * Link this currnt room's portal in the given direction to the give room.
   * The corresponding room is also linekd back.
   * If either rooms are already linked in the diretion, returns fakse.
   * @param dir
   * @param room_p
   * @return
   */
  bool LinkRoom(const Direction& dir, Room* room_p);


public:

  /**
   * Always get a room. If not yet connnected, connect it from factory
   * @param direction
   * @return
   */
  Room* GetConnectedRoom(const Direction& direction);

  float GetWidth() const;
  float GetHeight() const;

  float GetNSDoorWidth() const;
  float GetEWDoorWidth() const;

  size_t GetWallCount() const;

  const std::set<Wall>& GetWalls() const;

  bool ConnectedWith(Room* other, Direction direction) const;

  bool ConnectedWith(Room* other) const;

  // Point inclusivity just means the position of th ray shoots from is cincluded in consideraiton
  //  inclusivity should be false from secondary rays.
  //  this will allow the viewer to shoot and find the wall if the ray stands on the wall, but once non-inclusive ray is shot from the door of the other linekd room, it will ignore the wall/portalt it stands on
  Direction GetSideHit(const glm::vec2& pos, const glm::vec2& dir, bool point_inclusive = true) const;

  // Pure distamce
  float RoomWallHitDistance(Direction direction, const glm::vec2& pos, const glm::vec2& dir) const;

  bool PortalHit(Direction side, const glm::vec2& pos, const glm::vec2& dir) const;

  bool WithinRoom(const glm::vec2& pos, bool wall_inclusive = true) const;

  /**
   * Retrieves any  kak
   * @param pos
   * @param angle
   * @return
   */
  HitPackage GetVisible(const glm::vec2& pos, const float angle);

  // Factory need to be able to populate the private room members
  friend class RoomFactory;
};



}

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
