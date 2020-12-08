//
// Created by Jack Lee on 2020/11/15.
//

#ifndef ROOMS_APP_H
#define ROOMS_APP_H

#define LN_EPSILON 3.605170186f // -ln(epsilon), for epsilon a close approximation of 0, for example, .01

#include <core/game_engine.h>

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>

#include <string>

namespace room_explorer {

namespace visualizer {

/**
 * Handles ray-casted simple 3D rendering of non-euclidean map.
 * Allows movement through WASD input.
 */
class RoomsExplorerApp : public ci::app::App {
private:
  //! Path of template file, from which the game will load room data from.
  //!  Change path to load from different template!
  const std::string kTemplatePath = "/Users/jack/Cinder/my-projects/final-project-SungjaiLee/resources/tight_map.json";
  // Engine loads from the template
  GameEngine game_engine_;

  // Application Constant Fields =======================================================================================
  //TODO consider moving these constants as json values
  constexpr static const float kScreenWidth_ = 1000; //! HardCoded-Constant
  constexpr static const float kScreenHeight_ = 500; //! HardCoded-Constant

  // into how many columnes should the screen be partitionsed into?
  //  total resolution will be 2*n + 1
  constexpr static const size_t half_resolution = 60; //! HardCoded-Constant
  constexpr static const float total_resolution_ = half_resolution * 2 + 1;

  constexpr static const float strip_width_ = kScreenWidth_ / total_resolution_;

  // range in radius of half field
  constexpr static const float half_visual_field_range_ = 1; //! HardCoded-Constant

  constexpr static const float max_visible_distance_ = 700; //! HardCoded-Constant

  constexpr static const float floor_height_ = 100; //! HardCoded-Constant

  // Angle between each strips, seen from main-direction vector
  constexpr static const float incr_angle_ = 2 * half_visual_field_range_ / total_resolution_;
  // For faster recycling of computation, cosine and sine and sine of angles will be pre-computed at instantiation
  const float rotation_cos_;
  const float rotation_sin_;

  // Angle between rotation of each rotating movement.
  const float movement_angle_;
  // For the same reason as above, these trigonometric functions are pre-computed and recycled.
  const float movement_rotation_cos_;
  const float movement_rotation_sin_;

  // Speed of each forward or backward motion
  const float movement_speed_;

  // End of Application Constant Fields ================================================================================

  float GetBrightness(float distance) const;

  void DrawStrip(float left_index, float strip_width, const Hit& hit) const;

public:
  RoomsExplorerApp();

  void draw() override;
  void update() override;

  void keyDown(ci::app::KeyEvent event) override;


private:
  // Private Variables =================================================================================================
  const float projection_constant_ = 10000;

  // Counts number of updates
  size_t ticks_ = 0;

  // End of Private Variables ==========================================================================================

};

} // namespace visualizer

} // namespace rooms

#endif //ROOMS_APP_H
