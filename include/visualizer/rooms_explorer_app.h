//
// Created by Jack Lee on 2020/11/15.
//

#ifndef ROOMS_APP_H
#define ROOMS_APP_H

#include <core/game_engine.h>

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>

namespace room_explorer {

namespace visualizer {

/**
 * Handles ray-casted simple 3D rendering of non-euclidean map.
 * Allows movement through WASD input.
 */
class RoomsExplorerApp : public ci::app::App {
private:
  GameEngine game_engine_;

  constexpr static const float kScreenWidth_ = 1000; //! HardCoded-Constant
  constexpr static const float kScreenHeight_ = 500; //! HardCoded-Constant

  // into how many columnes should the screen be partitionsed into?
  //  total resolution will be 2*n + 1
  constexpr static const size_t half_resolution = 40; //! HardCoded-Constant
  constexpr static const float total_resolution_ = half_resolution * 2 + 1;

  constexpr static const float strip_width_ = kScreenWidth_ / total_resolution_;

  // range in radius of half field
  constexpr static const float half_visual_field_range_ = 1; //! HardCoded-Constant

  constexpr static const float max_visible_distance_ = 2000; //! HardCoded-Constant

  constexpr static const float floor_height_ = 100; //! HardCoded-Constant

  constexpr static const float incr_angle_ = half_visual_field_range_ / half_resolution;

  // Angle between each strips, seen from main-direction vector
  const float rotation_cos_;
  const float rotation_sin_;

  // Angle between rotation of each rotating movement.
  const float movement_angle_;
  const float movement_rotation_cos_;
  const float movement_rotation_sin_;

  // Speed of each forward or backward motion
  const float movement_speed_;

  float GetBrightness(float distance) const;

  void DrawStrip(float left_index, float strip_width, const Hit& hit) const;

public:
  RoomsExplorerApp();

  void draw() override;
  void update() override;

  void keyDown(ci::app::KeyEvent event) override;


private:
  const float projection_constant_ = 10000;
};

} // namespace visualizer

} // namespace rooms

#endif //ROOMS_APP_H
