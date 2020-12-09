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
  public:
   // Getters ==========================================================================================================
   size_t GetWallCount() const;
   // End of Getters ===================================================================================================

   // Friends :) =======================================================================================================
   // JSON loader ==================================================================
   friend void from_json(const json&, RoomTemplate& );
   // End of JSON loader ===========================================================

   // Room Factory. Allow only Factory full access to privates of template =========
   friend class RoomFactory;
   // End of Room Factory ==========================================================
   // End of Friends :( ================================================================================================
 };

private:
  // Effectively Constant Fields =======================================================================================
  float kRoomHeight_;
  float kRoomWidth_;
  float kNSDoorWidth_, kEWDoorWidth_;
  float kNSDoorBegin_, kEWDoorBegin_;

  std::map<std::string, RoomTemplate> kRoomTemplates_;
  std::set<std::string> kIds_;
  size_t kTemplateCounts_;

  glm::vec2 kEntryPosition_;
  // End of Effectively Constant Fields ================================================================================

public:
  // Getters ===========================================================================================================

  // Geometric Map Characteristics Getters =====================================================
  float RoomWidth() const;
  float RoomHeight() const;

  float GetNSPortalWidth() const;
  float GetEWPortalWidth() const;

  float GetNSPortalBegin() const;
  float GetNSPortalEnd() const;

  float GetEWPortalBegin() const;
  float GetEWPortalEnd() const;

  const glm::vec2& GetEntryPosition() const;
  // End of Geometric Map Characteristics Getters ==============================================


  // Template Characteristics Getters ==========================================================
  size_t RoomTemplateCount() const;
  const std::set<std::string>& GetAvailableIds() const;
  const std::string& RandomId() const;
  // End of Template Characteristics Getters ===================================================

  // End of Getters ====================================================================================================


  // Template Methods ==================================================================================================
  bool ContainsRoomId(const std::string& id) const;

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
  // End of Template Methods ===========================================================================================

  // JSON Loader =======================================================================================================
  friend void from_json(const json& json, RoomFactory& room_factory);
  // End of JSON Loader ================================================================================================
};

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
