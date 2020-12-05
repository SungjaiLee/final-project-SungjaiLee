//
// Created by Jack Lee on 2020/11/15.
//

#include <visualizer/rooms_explorer_app.h>

using room_explorer::visualizer::RoomsExplorerApp;

void prepareSettings(RoomsExplorerApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(RoomsExplorerApp, ci::app::RendererGl, prepareSettings);