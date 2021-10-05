//
// Created by Jack Lee on 2020/11/18.
//

#include <core/room_factory.h>

namespace room_explorer {

// JSON Loaders ========================================================================================================
void from_json(const json& json, RoomFactory& room_factory) {
  room_factory.kRoomWidth_ = json.at("room_dimension").at("width");
  room_factory.kRoomHeight_ = json.at("room_dimension").at("height");

  // Initialize position only if provided. Default to center of the room
  if (json.contains("entry_x")) {
    room_factory.kEntryPosition_.x = json.at("entry_x");
  } else {
    room_factory.kEntryPosition_.x = room_factory.kRoomWidth_ / 2;
  }
  if (json.contains("entry_y")) {
    room_factory.kEntryPosition_.y = json.at("entry_y");
  } else {
    room_factory.kEntryPosition_.y = room_factory.kRoomHeight_ / 2;
  }

  room_factory.kNSDoorWidth_ = json.at("room_dimension").at("ns_door_width");
  room_factory.kEWDoorWidth_ = json.at("room_dimension").at("ew_door_width");

  room_factory.kNSDoorBegin_ = (room_factory.kRoomWidth_ - room_factory.kNSDoorWidth_) / 2;
  room_factory.kEWDoorBegin_ = (room_factory.kRoomHeight_ - room_factory.kEWDoorWidth_) / 2;

  //Use for each items instead of copy to not have to copy id and templates separately
  for (auto& item : json.at("rooms").items()) {
    const std::string& id = item.key();

    room_factory.kIds_.insert(id);
    room_factory.kRoomTemplates_.insert(std::pair<std::string, RoomFactory::RoomTemplate>(id, item.value()));
  }

  room_factory.kTemplateCounts_ = room_factory.kIds_.size();
}

void from_json(const json& json, RoomFactory::RoomTemplate& room_template) {
  std::copy(json.at("walls").begin(), json.at("walls").end(),
            std::inserter(room_template.walls_, room_template.walls_.begin()));
}
// End of JSON Loaders =================================================================================================


// Room Template Getters ===============================================================================================
size_t RoomFactory::RoomTemplate::GetWallCount() const {
  return walls_.size();
}
// End of Room Template Getters ========================================================================================

// Room Factory Getters ================================================================================================
// Geometric Getters =================================================================
float RoomFactory::RoomWidth() const {
  return kRoomWidth_;
}
float RoomFactory::RoomHeight() const {
  return kRoomHeight_;
}

float RoomFactory::GetNSPortalWidth() const {
  return kNSDoorWidth_;
}
float RoomFactory::GetEWPortalWidth() const {
  return kEWDoorWidth_;
}

float RoomFactory::GetNSPortalBegin() const {
  return kNSDoorBegin_;
}
float RoomFactory::GetEWPortalBegin() const {
  return kEWDoorBegin_;
}

float RoomFactory::GetNSPortalEnd() const {
  return kNSDoorBegin_ + kNSDoorWidth_;
}
float RoomFactory::GetEWPortalEnd() const {
  return kEWDoorBegin_ + kEWDoorWidth_;
}

const glm::vec2& RoomFactory::GetEntryPosition() const {
  return kEntryPosition_;
}
// End of Geometric Getters ==========================================================

// Template Characteristics Getters ==================================================
size_t RoomFactory::RoomTemplateCount() const {
  return kTemplateCounts_;
}

const std::string &RoomFactory::RandomId() const {
  auto it = kIds_.begin();
  if (kTemplateCounts_ == 0) {
    throw exceptions::NoRoomTemplateException();
  }
  size_t advance_length = std::rand() % kTemplateCounts_;
  std::advance(it, advance_length);
  return *it;
}

const std::set<std::string>& RoomFactory::GetAvailableIds() const {
  return kIds_;
}
// End of Template Characteristics Getters ===========================================

// End of Room Factory Getters =========================================================================================

// Room Factory Generation Methods =====================================================================================
bool RoomFactory::ContainsRoomId(const std::string &id) const {
  return kIds_.find(id) != kIds_.end();
}

Room* RoomFactory::GenerateRoom(const std::string &id) const {
  if (!ContainsRoomId(id)) {
    return nullptr;
  }

  Room* room = new Room();
  // Every room has to hold reference to factory from which it and its adjacent rooms are generated
  room->factory = this;

  const RoomTemplate& room_temp = kRoomTemplates_.at(id);
  // Link straight to source. Reduces space complexity, which may be a source of slowness.
  // Due to non-euclidean physics, small repeating room unit will define infinite space.
  room->walls_ = &room_temp.walls_;

  return room;
}

Room* RoomFactory::GenerateRandomRoom() const {
  return GenerateRoom(RandomId());
}
// End of Room Factory Generation Methods ==============================================================================

} // namespace room_explorer