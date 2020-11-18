//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room_factory.h>

namespace room_explorer {




//void from_json(const json& json, RoomFactory& room_factory) {
//  room_factory.kRoomWidth = json.at("room_dimension").at("width");
//  room_factory.kRoomHeight = json.at("room_dimension").at("height");
//
//  room_factory.rooms_.resize(json.at("rooms").size());
//  std::copy(json.at("rooms").begin(), json.at("roos").end(), room_factory.rooms_.begin());
//}
//
//
void from_json(const json& json, RoomFactory::RoomTemplate& room_template) {
  std::copy(json.at("walls").begin(), json.at("walls").end(),
            std::inserter(room_template.walls_, room_template.walls_.begin()));
}

const size_t RoomFactory::RoomTemplate::GetWallCount() const {
  return walls_.size();
}


} // namespace room_explorer