//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room_factory.h>

namespace room_explorer {




void from_json(const json& json, RoomFactory& room_factory) {
  room_factory.kRoomWidth = json.at("room_dimension").at("width");
  room_factory.kRoomHeight = json.at("room_dimension").at("height");

//  room_factory.template_rooms_ = json.at("rooms").get<std::map<std::string, RoomFactory::RoomTemplate>>();
  //USE .items for both keya dn value

  for (auto& item : json.at("rooms").items()) {
    std::string id = item.key();

    room_factory.ids_.insert(id);
    room_factory.template_rooms_.insert(std::pair<std::string, RoomFactory::RoomTemplate>(id, std::move(item.value())));
  }

}


void from_json(const json& json, RoomFactory::RoomTemplate& room_template) {
  std::copy(json.at("walls").begin(), json.at("walls").end(),
            std::inserter(room_template.walls_, room_template.walls_.begin()));
}

size_t RoomFactory::AvailableCount() const {
  return template_rooms_.size();
}

const float RoomFactory::RoomWidth() const {
  return kRoomWidth;
}

const float RoomFactory::RoomHeight() const {
  return kRoomHeight;
}

const std::set<std::string> RoomFactory::GetAvailableIds() const {
  return ids_;
}

const size_t RoomFactory::RoomTemplate::GetWallCount() const {
  return walls_.size();
}


} // namespace room_explorer