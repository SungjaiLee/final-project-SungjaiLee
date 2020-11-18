//
// Created by Jack Lee on 2020/11/16.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H

#include <core/wall.h>

#include <nlohmann/json.hpp>

#include <string>
#include <fstream>

using json = nlohmann::json;

/**
 * Responsibe for generating new rooms.
 * Knows all the room templates from which rooms can be generatde from
 */
 namespace room_explorer {

 class RoomFactory {

 private:
   float kRoomWidth, kRoomHeight;

   struct RoomTemplate {
     std::set<Wall> walls_;
     int id_;
     friend void from_json(const json&, RoomTemplate& );
   };

   std::vector<RoomTemplate> rooms_;

 public:

   //json parse needs access to private RoomTemplate
   friend void from_json(const json& json, RoomFactory::RoomTemplate& room_template);

   friend void from_json(const json& json, RoomFactory& room_factory);

 };

// void from_json(const json& json, RoomFactory& room_factory);
//
// void from_json(const json& json, RoomFactory::RoomTemplate& room_template);

 } // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_FACTORY_H
