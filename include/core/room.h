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

#include <core/hits.h>
#include <core/wall.h>

#include <set>

namespace room_explorer {

// Studs ==================
class RoomFactory;
// end of Studs ====


enum Direction {
  kNorth,
  kSouth,
  kEast,
  kWest,
  kUndefined
};

Direction OppositeDirection(const Direction& direction);

/**
 * Individual room of the map.
 *  Handles internal geometric interactions with a ray.
 *  Able to retrieve visible hit-packages from the calculated geometric data.
 * Connects with other adjacent rooms.
 * Holds Factory from which it was generated, and from which other adjacent rooms will be generated.
 */
class Room {
private:
  const RoomFactory* factory;

  //! The coordinate (0, 0) is the SW Corner. The map basically emulates cartesian coordinates.
  Room* north_;
  Room* south_;
  Room* east_;
  Room* west_;

  float width_;
  float height_;

  float ns_door_width_;
  float ew_door_width_;

  float ns_door_begin_;
  float ew_door_begin_;

  std::set<Wall> walls;

public:
  // Public Room Member Functions ===============================================

  // Room Connectivity Functions ===================================
  /**
   * Retrieves the adjacent room in the given direction.
   * If the current room is not yet linked with any adjacent room, generate a new room from factory,
   *    link it with this room, and return the pointer to the newly generated room.
   * @param direction Direction in which the room should be retrieved, or if necessary generated in.
   * @return    Pointer to the adjacent room in the given direction.
   *            Will never be a nullptr. Always a room-pointer that is linked to this room as well.
   */
  Room* GetConnectedRoom(const Direction& direction) ;

  /**
   * Checks if the given room is connected with the current room in one specific direction.
   * @param other_p Pointer to the other room, which is being considered for being connected to this room.
   * @param direction Direction of the adjacent room.
   *        Other room is being checked for being accessible through this direction.
   * @return True if both rooms are connected in this specified direction.
   *        Linkage must be reflective. Both rooms must be connected with each other.
   */
  bool IsConnectedWith(Room* other_p, const Direction& direction) const;
  /**
   * Checks if the given room is connected with the current room in any of the 4 direction.
   * @param other_p Pointer to the other room, which is being considered for being connected to this room.
   * @return True if both rooms are connected in any of the 4 directions.
   *        Linkage must be reflective. Both rooms must be connected with each other.
   */
  bool IsConnectedWith(Room* other_p) const;

  // End of Connectivity Functions =================


  // Room-Geometric Functions ========================================

  // Getters ================================
  float GetWidth() const;
  float GetHeight() const;

  float GetNSDoorWidth() const;
  float GetEWDoorWidth() const;

  /**
   * Retrieves point considered the "head" of segment that defines the portal/room-wall in a given direction.
   *    Head is considered the more clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern portal/room-wall, the right-edge is considered the head.
   * @param direction Direction of portal/room-wall.
   * @param of_portal Whether head if that of portal or room-wall of given direction.
   * @return 2D Vector describing head of portal/room-wall.
   */
  glm::vec2 GetHead(Direction direction, bool of_portal) const;
  /**
   * Retrieves point considered the "tail" of segment that defines the portal/room-wall in a given direction.
   *    Tail is considered the more counter-clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern portal/room-wall, the left-edge is considered the tail.
   * @param direction Direction of portal/room-wall.
   * @return 2D Vector describing tail of portal/room-wall.
   */
  glm::vec2 GetTail(Direction direction, bool of_portal) const;

  // End of getters ===============

  /**
   * Checks of the point if within the confines of the room-walls.
   *    Possible to check strictly within the walls by setting wall inclusivity to false.
   *        Being on the wall is not considered as being withing room if wall is not inclusive.
   * @param pos Point being check for being within room.
   * @param wall_inclusive Whether point should be strictly within the walls.
   *                        As default, true.
   * @return Whether the point is within the room, strictly or not determined by parameter.
   */
  bool WithinRoom(const glm::vec2& pos, bool wall_inclusive = true) const;

  /**
   * Retrieves the direction of wall that a given ray will hit.
   *    If the ray is outside of the room, considered invalid direction, throw InvalidDirectionException.
   *    The head of each wall is considered as part of the wall.
   *        Head is considered the more clock-wise point of a given segment, relative to the inside of the room.
   *           ie) On the northern portal/room-wall, the right-edge is considered the head.
   *    Point inclusivity should be used for calling hit-direction on the linked rooms.
   *        Point inclusivity should be called as false, so that calling from adjacent room will not immediate halt
   *        once the portal-wall is detected.
   * @param ray_pos Position where the ray begins.
   * @param ray_dir Direction of the ray.
   * @param point_inclusive Determines whether wall from which the way might be on should be considered as hit.
   *                            If true, the wall on which the ray begins will be considered a hit.
   *                            If false, the hit will be what the ray touches second, ignoring the current wall.
   *                                This includes the current wall, if the ray points directly into the wall.
   *                        As default, set to true.
   * @return Direction of the first valid wall that the ray hits, according to the passed parameters.
   */
  Direction GetSideHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir, bool point_inclusive = true) const;

  /**
   * Distance that ray must travel from its initial position to intersect with the wall in the given direction.
   *    Validity of this path is not checked. Wall is assumed to be infinitely extended.
   *    Negative distance is also considered valid.
   *    Post-processing of data should be handled outside of this method.
   *    Wall in this context include both room-wall and the portals.
   * @param direction Direction of wall, to which the ray is being shot.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Direction of the ray.
   * @return Distance, positive or negative, that the ray must travel in the direction to intersect with the wall.
   */
  float GetRoomWallHitDistance(const Direction& direction, const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  /**
   * Checks if the given ray Intersects with the portal on the wall of the given direction.
   *    Does not considered if the path is valid.
   *    Ray must intersect with the portal in the positive direction.
   * @param direction Direction of wall, to which the ray is being shot.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Direction of the ray.
   * @return True if ray will eventually intersect with the portal of the given direction in the room.
   */
  bool RayHitsPortal(const Direction& direction, const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  /**
   * Retrieve texture index of the portal/room-wall of given direction, intercepted by the given ray.
   *    Index is distance of intersecting point from the head.
   *    Index is not post-processed,
   *        and in most contexts will be assumed to be called only under appropriate conditions.
   *    Negative index is possible.
   *    In-Line ray will have index of 0, if ray is aimed at head,
   *        or have index of distance between head and ray-position.
   *        The appropriate nature of such result is not handled in this function.
   * @param direction Direction of wall, to which the ray is being shot.
   * @param of_portal Whether head if that of portal or room-wall of given direction.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Direction of the ray.
   * @return Index of texture that should be loaded.
   *            Synonymous with distance of intersection from the segment-head.
   */
  float GetWallTextureIndex(const Direction& direction, bool of_portal,
                            const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  Hit GetRoomWallHit(const glm::vec2& ray_pos, const glm::vec2& ray_dir) const;

  /**
   * Retrieves HitPackage of all the walls and portal/room-wall in the path of the ray.
   *    Will extend to the adjacent rooms if ray passes through the portal.
   * @param ray_pos Initial position of the ray.
   * @param ray_dir Direction of the ray.
   * @return HitPackage of all the elements intersected by the ray in the given direction of this room.
   *            Will include the package of the adjacent room if appropriate.
   */
  HitPackage GetVisible(const glm::vec2& ray_pos, const glm::vec2& ray_dir);

  // End of Room-Geometry Functions ==============


  // Room Element Functions ==============================================

  /**
   * @return Number of unique walls in the current room.
   */
  size_t GetWallCount() const;

  /**
   * @return Set of all the walls in the current room.
   */
  const std::set<Wall>& GetWalls() const;

  // End of Room Element Functions ==========

  // End of Public Room Member functions =============



private:
  // Private Room Member Functions ===============================================================

  /**
   * Retrieves pointer to adjacent room in the given direction.
   * @param direction Direction of the retrieved adjacent room.
   * @return Reference to the pointer to adjacent room.
   *         Allow direct alteration to the member pointer that is being retrieved.
   */
  Room*& GetLinkedRoomPointer(const Direction& direction);
  /**
   * Retrieves pointer to adjacent room in the given direction.
   * @param direction Direction of the retrieved adjacent room.
   * @return Pointer to adjacent room.
   */
  Room*  GetLinkedRoomPointer(const Direction& direction) const;

  /**
   * Link the current room with the given room in the direction relative to the current room.
   *    Only link of both rooms have not yet defined the adjacent room in the given direction yet.
   * @param direction     Direction relative to the current room.
   *                The opposite direction will be linked in the other room.
   * @param room_p  Pointer to the other room being linked.
   *                It should not have been linked in the opposite direction yet.
   * @return If the linkage was able to be completed properly.
   *                Link only occurs if both rooms are not already linked in the given direction.
   */
  bool LinkRoom(const Direction& direction, Room* room_p);

  /**
   * Retrieves point considered the "head" of segment that defines the portal in a given direction.
   *    Head is considered the more clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern portal, the right-edge is considered the head.
   * @param dir Direction of wall that portal is placed on.
   * @return 2D Vector describing head of portal.
   */
  glm::vec2 GetPortalHead(Direction dir) const;
  /**
   * Retrieves point considered the "tail" of segment that defines the portal in a given direction.
   *    Tail is considered the more counter-clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern portal, the left-edge is considered the tail.
   * @param dir Direction of wall that portal is placed on.
   * @return 2D Vector describing tail of portal.
   */
  glm::vec2 GetPortalTail(Direction dir) const;

  /**
   * Retrieves point considered the "head" of segment that defines the room-wall in a given direction.
   *    Head is considered the more clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern room-wall, the NE-corner is considered the head.
   * @param dir Direction of wall that portal is placed on.
   * @return 2D Vector describing head of room-wall.
   */
  glm::vec2 GetWallHead(Direction dir) const;
  /**
   * Retrieves point considered the "tail" of segment that defines the room-wall in a given direction.
   *    Tail is considered the more counter-clock-wise point of a given segment, relative to the inside of the room.
   *    ie) On the northern room-wall, the NW-corner is considered the tail.
   * @param dir Direction of wall that portal is placed on.
   * @return 2D Vector describing tail of room-wall.
   */
  glm::vec2 GetWallTail(Direction dir) const;

  // End of Private Member Functions ===========================


  // Friend Classes/Functions =====================================================

  //! Factory needs to be able to initialize private members of room object
  friend class RoomFactory;

  // End of Friends ====================
};

}

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_H
