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
      total_resolution_(2 * half_resolution + 1) {

  ci::app::setWindowSize(kScreenWidth_, kScreenHeight_);

}

void RoomsExplorerApp::draw() {
  ci::Color8u background_color(50, 151, 168);
  ci::gl::clear(background_color);


  float section_width = kScreenWidth_ / (total_resolution_);

  std::vector<HitPackage> packages{current_room_.GetVision(rotation_cos_, rotation_sin_,
                                                           half_resolution,
                                                           200)};

  for (size_t i = 0; i < total_resolution_; ++i) {
    if (i % 2 == 0) {
      ci::gl::color(ci::Color8u(200, 101, 108));
    } else {
      ci::gl::color(ci::Color8u(20, 101, 108));
    }
    ci::Rectf rect{{i * section_width, 0},
                   {(i + 1) * section_width, kScreenHeight_}};

    ci::gl::drawSolidRect(rect);
  }
}
void RoomsExplorerApp::update() {

}




