//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room_factory.h>

namespace room_explorer {


// JSON Loaders ===============================================

void from_json(const json& json, RoomFactory& room_factory) {
  room_factory.kRoomWidth = json.at("room_dimension").at("width");
  room_factory.kRoomHeight = json.at("room_dimension").at("height");

  //Use for each items instead of copy to not have to copy id and templates separately
  for (auto& item : json.at("rooms").items()) {
    std::string id = item.key();

    room_factory.ids_.insert(id);
    room_factory.template_rooms_.insert(std::pair<std::string, RoomFactory::RoomTemplate>(id, item.value()));
  }

  room_factory.counts_ = room_factory.ids_.size();
}

void from_json(const json& json, RoomFactory::RoomTemplate& room_template) {
  std::copy(json.at("walls").begin(), json.at("walls").end(),
            std::inserter(room_template.walls_, room_template.walls_.begin()));
}

// end of JSON Loaders


// RoomFactory Member Handler =====================================

float RoomFactory::RoomWidth() const {
  return kRoomWidth;
}

float RoomFactory::RoomHeight() const {
  return kRoomHeight;
}

size_t RoomFactory::RoomTemplateCount() const {
  return counts_;
}

bool RoomFactory::ContainsRoomId(const std::string &id) const {
  return ids_.find(id) != ids_.end();
}

const std::set<std::string>& RoomFactory::GetAvailableIds() const {
  return ids_;
}

const std::string &RoomFactory::RandomId() const {
  auto it = ids_.begin();
  if (counts_ == 0) {
    throw exceptions::NoRoomTemplateException();
  }
  size_t advance_length = std::rand() % counts_;
  std::advance(it, advance_length);
  return *it;
}

Room* RoomFactory::GenerateRoom(const std::string &id) const {
  if (!ContainsRoomId(id)) {
    return nullptr;
  }

  Room* room = new Room();
  room->width_ = kRoomWidth;
  room->height_ = kRoomHeight;

  room->factory = this;

  RoomTemplate room_temp = template_rooms_.at(id);

  //TODO should walls be only reference? copying might take up too much storage
  std::copy( room_temp.walls_.begin(), room_temp.walls_.end(),
             std::inserter(room->walls, room->walls.begin()) );

  return room;
}

Room* RoomFactory::GenerateRandomRoom() const {
  return GenerateRoom(RandomId());
}

//RoomTemplate Methods ============================================

size_t RoomFactory::RoomTemplate::GetWallCount() const {
  return walls_.size();
}

// End of RoomTemplate ===========================


} // namespace room_explorer