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
#include <random>

using json = nlohmann::json;

/**
 * Responsibe for generating new rooms.
 * Knows all the room templates from which rooms can be generatde from
 */
namespace room_explorer {

class Room;

class RoomFactory {
public:
 struct RoomTemplate {
   std::set<Wall> walls_;
   friend void from_json(const json&, RoomTemplate& );

 public:
   size_t GetWallCount() const;
 };

private:
  float kRoomWidth, kRoomHeight;
  float kNSDoorWidth, kEWDoorWidth;

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
  friend void from_json(const json& json, RoomFactory::RoomTemplate& room_template);

  friend void from_json(const json& json, RoomFactory& room_factory);
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
