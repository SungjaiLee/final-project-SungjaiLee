#Multi-Intersection Ray-Casting on a Procedurally Generated Non-Euclidean Map
#### Wolfenstein 3D Clone with Transparent Walls and Weirdly Connected Rooms

This project utilizes simple technique of 3D rendering of a 2D map known as Ray-Casting.
 Instead of rendering a single map, each individual 'room' only make in portion of the true infinite map.
 Ray will be shot from a single room, then expand out to the adjacent room if portal is intersected. 
 
Through such rendering technique, a map does not need to be preloaded or even built.
 This allows easy implementation of both procedural generation and also non-euclidean map. 
 Rooms adjacent rooms are only connected on load, and only directly adjacent rooms are connected.
 Meaning, that room at Up+Left is not the same room at Left+Up. (Look for this feature when exploring the rooms!)
 
##Build
Application relies on Cinder for rendering and gl library. The json-reading nlohmann library is included in include.

Place the apps, include, resources, and src into a project under Cinder file and run the cinder app.

### resources
The resources folder contain the json for room-templates from which the game will be loaded from.
You may write your own map templates and load run it through the rest of application. 
To change the kTemplatePath in include/visualizer/rooms_explorer_app.h header file.
Use absolute path.

##Movements
Use arrow keys to move around in the explorer app.
 
Up and down will move forwards and backwards.

Left and right will rotate the point of view. 

##Goal
There is not defined gaol in this game yet.

Try to walk around and observe the weird geometry of this non-euclidean world!
Try to make your own maps and walk around it. 


    