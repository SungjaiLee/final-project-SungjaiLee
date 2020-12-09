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
#include <set>

namespace room_explorer {

namespace visualizer {

/**
 * Handles ray-casted simple 3D rendering of non-euclidean map.
 * Allows movement through WASD input.
 */
class RoomsExplorerApp : public ci::app::App {
private:
  //! To load different settings and different map templates, alter the Config paths!
  // External JSON Config Paths ========================================================================================
  const std::string kTemplatePath =
      "/Users/jack/Cinder/my-projects/final-project-SungjaiLee/resources/room_templates/tight_map.json";

  const std::string kMetaPath =
      "/Users/jack/Cinder/my-projects/final-project-SungjaiLee/resources/meta/standard_meta.json";
  // End of External JSON Config Paths =================================================================================

  GameEngine game_engine_;

  // Application Effective Constant Fields =============================================================================
  float kScreenWidth_;
  float kScreenHeight_;

  // Rendering Resolution Constants ===================================================
  size_t kHalfResolution_; // Number of strips occupying half of screen
  size_t kTotalResolution_; // Number if strips occupying full screen. Middle 1 + 2 * half_resolution.

  float kStripWidth_;

  float kResolutionAngle_; // Angle between rays for each individual strips
  float kResolutionCosine_; // Trigonometrics pre-calculated for known consistent angles between rays.
  float kResolutionSine_;
  // End of Rendering Resolution Constants ============================================


  // Visibility Constants =============================================================
  float kHalfVisualFieldRange_;

  float kVisibleDistance_;
  // End of Visibility Constants ======================================================


  // Player Motion Variable ============================================================
  float kMovementRotationAngle_;
  float kMovementRotationCosine_;
  float kMovementRotationSine_;

  float kMovementSpeed_;
  // End of Player Motion Variable =====================================================


  // Cosmetic Constants ===============================================================
  float kFloorHeight_; // Horizontal level of the floor on screen

  float kProjectionPlaneDistanceCoefficient_; // Concerns height of walls hit by the ray.
                              // Further the distance, the larger each walls will seem.

  float kBrightnessDepthAdjuster;

  // Colors ==========================================================
  ci::ColorA kCeilingColor;
  ci::ColorA kFloorColor;

  ci::ColorA kWallSolidColor;
  ci::ColorA kWallWindowColor;

  ci::ColorA kPortalPureColor;

  ci::ColorA kRoomWallColor;
  // End of Colors ===================================================

  // Cosmetic Constants ===============================================================

  // End of Application Constant Fields ================================================================================


  // State Handler Variables ===========================================================================================
  size_t ticks_; // Keeps track of number of updates performed

  std::set<int> held_keys_; // Keeps track of all the keys pressed simultaneously
  // End of State Handler Variables ====================================================================================


  // Private Render Helper Methods =====================================================================================
  /**
   * Returns brightness associated with a distance.
   * This will depend on the max visible distance.
   * @param distance Distance of hit.
   * @return 1 for fully bright. Lower values for dimmer.
   *            Value will be clamped between 0 and 1.
   */
  float GetBrightness(float distance) const;

  /**
   * Handles rendering of a single component on a single strip.
   * Wall, Room-wall, or a Portal will be graphically rendered on top of existing frame.
   *    This allows transparent glass-look if transparent portal is drawn on top of another element.
   * Void and invalid hits are not rendered at all.
   * @param left_index Index of the strip.
   * @param hit Element intersected by the ray.
   */
  void DrawStrip(float left_index, const Hit& hit) const;
  // End of Private Render Helper Methods ==============================================================================

public:
  RoomsExplorerApp();

  void draw() override;
  void update() override;

  void keyDown(ci::app::KeyEvent event) override;
  void keyUp(ci::app::KeyEvent event) override;
};

} // namespace visualizer

} // namespace rooms

#endif //ROOMS_APP_H
