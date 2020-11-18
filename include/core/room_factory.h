//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H

#include <core/wall.h>
#include <core/room.h>

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

 class RoomFactory {


 public:
   struct RoomTemplate {
     std::set<Wall> walls_;
     friend void from_json(const json&, RoomTemplate& );

   public:
     const size_t GetWallCount() const;
   };

 private:
   float kRoomWidth, kRoomHeight;

   std::map<std::string, RoomTemplate> template_rooms_;

   std::set<std::string> ids_;

   size_t counts_;

 public:
   const float RoomWidth() const;
   const float RoomHeight() const;

   size_t AvailableCount() const;
   const std::set<std::string> GetAvailableIds() const;

   bool ContainsRoomId(const std::string& id) const;

   /*
    * nullptr is no id,
    */
   Room* GenerateRoom(const std::string& id) const;

   const std::string& RandomId() const;

   /*
    * nullptr is no id,
    *
    */
   Room* GenerateRandomRoom() const;




   //json parse needs access to private RoomTemplate
   friend void from_json(const json& json, RoomFactory::RoomTemplate& room_template);

   friend void from_json(const json& json, RoomFactory& room_factory);

 };

// void from_json(const json& json, RoomFactory& room_factory);
//
// void from_json(const json& json, RoomFactory::RoomTemplate& room_template);

 } // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
