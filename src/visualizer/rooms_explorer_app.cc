//
// Created by Jack Lee on 2020/11/15.
//

#include <visualizer/rooms_explorer_app.h>

using namespace room_explorer::visualizer;

RoomsExplorerApp::RoomsExplorerApp()
    : current_room_("/Users/jack/Cinder/my-projects/final-project-SungjaiLee/resources/room_template.json"),
      incr_angle_(half_visual_field_range_ / half_resolution),
      rotation_cos_(std::cos(incr_angle_)),
      rotation_sin_(std::sin(incr_angle_)),
      total_resolution_(2 * half_resolution + 1),
      movement_rotation_cos_(std::cos(.1f)),
      movement_rotation_sin_(std::sin(.1f)) {

  ci::app::setWindowSize(kScreenWidth_, kScreenHeight_);

}

void RoomsExplorerApp::draw() {
  ci::Color8u background_color(50, 151, 168);
  ci::gl::clear(background_color);


  float wall_bottom = 100;

  ci::Rectf floor{glm::vec2(0, kScreenHeight_),
                   glm::vec2(kScreenWidth_, kScreenHeight_ - wall_bottom)};
  ci::gl::color(ci::ColorA(0, 0, 0));
  ci::gl::drawSolidRect(floor);


  float section_width = kScreenWidth_ / (total_resolution_);

  float max_range = 1500;

  std::vector<HitPackage> packages{current_room_.GetVision(rotation_cos_, rotation_sin_,
                                                           half_resolution,
                                                           max_range)};

  for (size_t i = 0; i < total_resolution_; ++i) {
//    if (i % 2 == 0) {
//      ci::gl::color(ci::Color8u(200, 101, 108));
//    } else {
//      ci::gl::color(ci::Color8u(20, 101, 108));
//    }
//    ci::Rectf rect{{i * section_width, 0},
//                   {(i + 1) * section_width, kScreenHeight_}};
//
//    ci::gl::drawSolidRect(rect);

    HitPackage& package = packages.at(i);

    std::map<float, Hit>::const_iterator it = package.GetHits().end();
    do {
      --it;
      float distance = it->first;
      // todo find the correct height calculation
      float height = 10000 / (distance + 1);
      ci::Rectf wall{{i * section_width, kScreenHeight_ - wall_bottom + (height / 10)},
                     {(i + 1) * section_width, kScreenHeight_ - wall_bottom - height}};

      float shade{GetBrightness(distance, max_range)}; // make its own function

      if (it->second.hit_type_ == kPortal) {
        ci::gl::color(ci::ColorA(100, 0, 200, .2));
//        wall = {{i * section_width, kScreenWidth_},
//             {(i + 1) * section_width, kScreenHeight_ - 100 - height}};
      } else if (it->second.hit_type_ == kWall) {
        ci::ColorA col;
        if (std::fmod(it->second.texture_index_, 50) >= 5) {
          col = ci::ColorA(200 * shade, 200 * shade, 200 * shade);
        } else {
//          col = ci::ColorA(20 * shade, 200 * shade, 100 * shade, .5);
          col = ci::ColorA(0, 200, 0);
        }
        ci::gl::color(col);

      } else if (it->second.hit_type_ == kRoomWall) {
        ci::gl::color(ci::ColorA(200 * shade, 0, 0));
      } else {
        ci::gl::color(ci::ColorA(0, 0, 0));
      }

      ci::gl::drawSolidRect(wall);
    } while (it != package.GetHits().begin());
  }
}
void RoomsExplorerApp::update() {

}

void RoomsExplorerApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_LEFT:
      current_room_.RotateDirection(movement_rotation_cos_, -movement_rotation_sin_);
      break;
    case ci::app::KeyEvent::KEY_RIGHT:
      current_room_.RotateDirection(movement_rotation_cos_, movement_rotation_sin_);
      break;
    case ci::app::KeyEvent::KEY_UP:
      current_room_.MoveForward(4);
      break;
    case ci::app::KeyEvent::KEY_DOWN:
      current_room_.MoveForward(-4);
      break;
  }
}

float RoomsExplorerApp::GetBrightness(float distance, float max_range) const {
//  float proportion{distance / max_range};

  const float half_point_adjuster = .019f;// ln2 / half_point

  return std::exp(-distance * half_point_adjuster);
//  return std::exp(-proportion * half_point_adjuster);
}




