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
      movement_rotation_cos_(std::cos(.01f)),
      movement_rotation_sin_(std::sin(.01f)) {

  ci::app::setWindowSize(kScreenWidth_, kScreenHeight_);

}

void RoomsExplorerApp::draw() {
  ci::Color8u background_color(50, 151, 168);
  ci::gl::clear(background_color);


  float section_width = kScreenWidth_ / (total_resolution_);

  std::vector<HitPackage> packages{current_room_.GetVision(rotation_cos_, rotation_sin_,
                                                           half_resolution,
                                                           900)};

  for (size_t i = 0; i < total_resolution_; ++i) {
    if (i % 2 == 0) {
      ci::gl::color(ci::Color8u(200, 101, 108));
    } else {
      ci::gl::color(ci::Color8u(20, 101, 108));
    }
    ci::Rectf rect{{i * section_width, 0},
                   {(i + 1) * section_width, kScreenHeight_}};

    ci::gl::drawSolidRect(rect);

    HitPackage& package = packages.at(i);
    auto it = package.GetHits().begin();

    float distance = it->first;
    // todo find the correct height calculation
    float height = 10000 / (distance + 1);
    ci::Rectf wall{{i * section_width, kScreenHeight_ - 100},
                   {(i + 1) * section_width, kScreenHeight_ - 100 - height}};


    ci::gl::color(ci::Color8u(200, 200, 200));
    if (it->second.hit_type_ == kPortal) {
      ci::gl::color(ci::Color8u(100, 20, 100));
    }
    ci::gl::drawSolidRect(wall);

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
      current_room_.MoveForward(1);
      break;
    case ci::app::KeyEvent::KEY_DOWN:
      current_room_.MoveForward(-1);
      break;
  }
}




