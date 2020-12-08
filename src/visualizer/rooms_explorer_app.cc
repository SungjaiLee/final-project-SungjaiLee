//
// Created by Jack Lee on 2020/11/15.
//

#include <visualizer/rooms_explorer_app.h>

using namespace room_explorer::visualizer;

RoomsExplorerApp::RoomsExplorerApp()
    : game_engine_("/Users/jack/Cinder/my-projects/final-project-SungjaiLee/resources/small_maze.json"),
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


  ci::Rectf floor{glm::vec2(0, kScreenHeight_),
                   glm::vec2(kScreenWidth_, kScreenHeight_ - floor_height_)};
  ci::gl::color(ci::ColorA(0, 0, 0));
  ci::gl::drawSolidRect(floor);


  float section_width = kScreenWidth_ / (total_resolution_);

//  float max_range = 1500;

  std::vector<HitPackage> packages{game_engine_.GetVision(rotation_cos_, rotation_sin_,
                                                           half_resolution,
                                                           max_visible_distance_)};

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
      DrawStrip(i, section_width, it->second);
    } while (it != package.GetHits().begin());
  }
}
void RoomsExplorerApp::update() {

}

void RoomsExplorerApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_LEFT:
      game_engine_.RotateDirection(movement_rotation_cos_, -movement_rotation_sin_);
      break;
    case ci::app::KeyEvent::KEY_RIGHT:
      game_engine_.RotateDirection(movement_rotation_cos_, movement_rotation_sin_);
      break;
    case ci::app::KeyEvent::KEY_UP:
      game_engine_.MoveForward(10);
      break;
    case ci::app::KeyEvent::KEY_DOWN:
      game_engine_.MoveForward(-10);
      break;
  }
}

float RoomsExplorerApp::GetBrightness(float distance) const {
//  float proportion{distance / max_range};

  const float half_point_adjuster = .0005f;// ln2 / half_point

  float brightness{std::exp(-distance * half_point_adjuster)};
  if (brightness < 0) {
    return 0;
  }
  if (brightness > 1) {
    return 1;
  }
  return brightness;
//  return std::exp(-proportion * half_point_adjuster);
}

void RoomsExplorerApp::DrawStrip(float left_index, float strip_width, const Hit& hit) const {
  float room_height{projection_constant_ / hit.hit_distance_};

  float lower_height{kScreenHeight_ - floor_height_ + room_height / 8};
  float upper_height{kScreenHeight_ - floor_height_ - room_height};

  switch (hit.hit_type_) {

    case kRoomWall:
      {
        ci::Rectf wall{{left_index * strip_width,       lower_height},
                       {(left_index + 1) * strip_width, upper_height}};
        float shade{GetBrightness(hit.hit_distance_)};
        ci::ColorA col{1 * shade, .2f * shade, .3f * shade};

        ci::gl::color(col);
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kWall:
      {
        float sub_index{static_cast<float>(std::fmod(hit.texture_index_, 50))};

        float lower_window;
        float upper_window;
        if (sub_index >= 30) {
          //window will begin at 60% to 90%
          // since wall height is 9 * distance / 8,  lower and upper window is at
          lower_window = {28 * room_height / 40};
          upper_window = {81 * room_height / 80};
        } else {
          //window will begin at 5% to 90%
          lower_window = {9 * room_height / 160};
          upper_window = {81 * room_height / 80};
        }

        float shade{GetBrightness(hit.hit_distance_)};
        // wall col
        ci::ColorA col{1 * shade, 1 * shade, 1 * shade};
        ci::gl::color(col);
        // lower section
        ci::Rectf wall {{left_index * strip_width,       lower_height},
                        {(left_index + 1) * strip_width, lower_height - lower_window}};
        ci::gl::drawSolidRect(wall);
        // upper
        wall = {{left_index * strip_width,      lower_height - upper_window},
               {(left_index + 1) * strip_width, upper_height}};
        ci::gl::drawSolidRect(wall);

        // window col
        col = {0, 1 * shade, 0, 1.2f - shade};
        ci::gl::color(col);
        wall = {{left_index * strip_width,       lower_height - lower_window},
                {(left_index + 1) * strip_width, lower_height - upper_window}};
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kPortal:
      {
        // find a more elegent design
//        const float portal_repetition{50};
//        float sub_index{static_cast<float>(std::fmod(hit.texture_index_, portal_repetition))};
//        sub_index /= portal_repetition;
//        sub_index -= .5f;
//        sub_index /= 3;
//        float shade{sub_index * sub_index};

        ci::ColorA col{.8f, .2f, .9f, .3f};
        ci::gl::color(col);
        ci::Rectf wall = {{left_index * strip_width,       lower_height},
                {(left_index + 1) * strip_width, upper_height}};
        ci::gl::drawSolidRect(wall);
      }
      break;

    case kVoid:
    case kInvalid:
      // Draw nothing?
      break;
  }
}




