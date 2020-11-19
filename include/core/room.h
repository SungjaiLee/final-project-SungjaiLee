#include <climits>
//
// Created by Jack Lee on 2020/11/19.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_H

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
#include <core/room_factory.h>
#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H

#include <core/wall.h>

#include <set>

namespace room_explorer {

enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest
};

Direction OppositeDirection(const Direction& direction);


class RoomFactory;

class Room {
private:
  const RoomFactory* factory;

  Room* north_;
  Room* south_;
  Room* east_;
  Room* west_;

  float width_;
  float height_;

  std::set<Wall> walls;

  /**
   * Returns pointer to room in the given direction.
   * @param direction
   * @return
   */
  Room*& GetLinkedRoomPointer(const Direction& direction);
  Room* GetLinkedRoomPointer(const Direction& direction) const;

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

  size_t GetWallCount() const;

  const std::set<Wall>& GetWalls() const;

  bool ConnectedWith(Room* other, Direction direction) const;

  bool ConnectedWith(Room* other) const;


  // Factory need to be able to populate the private room members
  friend class RoomFactory;
};



}

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
