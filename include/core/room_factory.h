//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_H
#include <core/room.h>
#endif  // NONEUCLIDEAN_RAY_CASTER_ROOM_H

#include <core/wall.h>

#include <exceptions/room_explorer_exception.h>

#include <nlohmann/json.hpp>

#include <string>
#include <fstream>

using json = nlohmann::json;

namespace room_explorer {
// Stud for Room =====================================
// Necessary so that the RoomFactory recognizes existence of room.
class Room;
// End of Stud for Room ==============================

/**
 * Responsible for room generation.
 * Contains all the templates from which a room can be generated from.
 * A factory will be contained in each room.
 *      Each room will generate and pass on the same factory for linkage and connections.
 */
class RoomFactory {
public:
 /**
  * Template which holds all the walls that defines a specific room.
  * Each room generated from the template will refer to all the walls from the template
  *     rather than copying each wall to each rooms.
  */
 struct RoomTemplate {
 private:
   std::set<Wall> walls_;

   friend void from_json(const json&, RoomTemplate& );
   // Limit access to outer classes, but allow RoomFactory to use this freely.
   //  Also prevents the sensitive walls_ field to be not exposed.
   friend class RoomFactory;
 public:
   /**
    * @return Number of walls  in this room-template.
    */
   size_t GetWallCount() const;
 };

public:
  float kRoomHeight;
  float kRoomWidth;
  float kNSDoorWidth, kEWDoorWidth;
  float kNSDoorBegin, kEWDoorBegin;

  std::map<std::string, RoomTemplate> template_rooms_;

  std::set<std::string> ids_;

  size_t counts_;
public:
  float RoomWidth() const;
  float RoomHeight() const;

  size_t RoomTemplateCount() const;
  const std::set<std::string>& GetAvailableIds() const;

  bool ContainsRoomId(const std::string& id) const;

  const std::string& RandomId() const;

  /**
   * Generate room of given id.
   * If the id is not recognized, return pointer to nullptr.
   * @param id ID of the room template.
   * @return Room generated from the template of given id.
   *            If id does not exist, return nullptr.
   */
  Room* GenerateRoom(const std::string& id) const;

  /**
   * Generate random room from available template.
   * @return Return pointer to a newly generated room from random id.
   */
  Room* GenerateRandomRoom() const;

  //json parse needs access to private RoomTemplate
  friend void from_json(const json& json, RoomFactory& room_factory);
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
