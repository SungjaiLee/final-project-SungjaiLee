//
// Created by Jack Lee on 2020/11/15.
//

#ifndef ROOMS_APP_H
#define ROOMS_APP_H

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace rooms {

namespace visualizer {

/**
 * Handles ray-casted simple 3D rendering of non-euclidean map.
 * Allows movement through WASD input.
 */
class RoomsExplorerApp : public ci::app::App {

};

} // namespace visualizer

} // namespace rooms

#endif //ROOMS_APP_H
