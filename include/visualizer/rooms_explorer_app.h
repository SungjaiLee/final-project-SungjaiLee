//
// Created by Jack Lee on 2020/11/15.
//

#ifndef ROOMS_APP_H
#define ROOMS_APP_H

#include <core/current_room.h>

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
  const float kScreenWidth_ = 750;
  const float kScreenHeight_ = 550;

  __unused const std::string room_template_path_;

  // into how many columnes should the screen be partitionsed into?
  //  total resolution will be 2*n + 1
  const size_t half_resolution = 20;

  // range in radius of half field
  const float half_visual_field_range_ = 5;


  CurrentRoom current_room_;

  const float incr_angle_;

  const float rotation_cos_;
  const float rotation_sin_;

  const float total_resolution_;


public:
  RoomsExplorerApp();

  void draw() override;
  void update() override;

};

} // namespace visualizer

} // namespace rooms

#endif //ROOMS_APP_H
