#Non-Euclidean Procedurally-Generated Wolfenstein Clone

Despite the fancy name, the project idea is very simple: 
make a very basic Wolfenstein/Doom clone who's map is map is generated as player enters each room. 
To make this generation process easier, each room only need to link to another, without having to consider 
adjacent rooms intersecting, giving the game a little bit of non-euclidean feel. 

##Map Generation

Map generation is done as player enters a room. Room will be square and with up to 4 gate/door. 
Each door leads to another room. Which is similar to the current room but different or even same design.  
These doors are always open adn player can see through this door.  
However, the rooms are not linked together.
This means going up then left will land you in a different room than going left and up.
This makes the map non-euclidean. 

This characteristic of map generation also means a game's map is not a singular 2d image but rather
a room object linked to 4 more rooms. 
The connected rooms are only determined once the player steps into the current room.
The room type can be selected from existing room templates. 

##Ray Casting

Visual component of this project will be handled using simple ray casting.  
Player will shoot number of rays which extend through the map until it hits an object.  
The vertical strip on the screen will be determined using the distance the ray took.  
This limits the quality of the graphics to look like extremely rudimentary Wolfenstein 3D clone with
even less graphics, bit that is the best I can do. Sorry. I don't know how to use unity :'(  
Ray will continue until it either hits wall or go too far out. 
Even if the ray hits an entity such as a enemy, it will simply collect that data and continue until it hits a wall. 
This is done because entity may not take up the whole vertical strip
 and you may be able to look over the entity at the wall.

######Helpful Links
[Coding Train's Raycasting](https://www.youtube.com/watch?v=vYgIKn7iDH8)  
[Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)

####Non Euclidean Ray Casting
The problem is with the Non-Euclidean nature of my maps.  
Because my maps are not simple geometric shapes, you cannot shoot a simple ray.  
The ray might go through the door and into the adjacent room, which is not straight-forward because they will
 only be linked by data, but map.  
This means that if a ray is cast into a door, we will have to request the connected room to shoot another ray
from the same location of the door at the same angle and return the targets.  
If a door is not yet linked, it will automatically generate once the ray requests for data through that door.  
This allows the game to maintain that weird Non-Euclidean feel. 
 
This ray casting through so many doors and elements may make the game bit slow, 
 but by allowing limited range and using c++ instead of slower java, I'm hoping it is at least playable. 

######Hepful Links
[Non-Euclidean Example](https://www.youtube.com/watch?v=kEB11PQ9Eo8)

##Combat and Entity

Some rooms may spawn entity within them. This again will eb determined by the room types.  
Player can shoot certain kind of fireball or something to attempt to damage and kill these entities.  
Killing them will increase player's score. 

####Enemy Types

#####Shooter
Shooter will shoot small fireballs just as player does. Getting hit will decrease player's health. 

#####Rusher
Rushers will run towards player. Making contact will immediately kill the player. 

The ideas for enemies are not yet flashed out.  
Knifing mechanics may be considered in addition for a stealth kill.

Due to graphics limitations, enemies may look like boxes and circles.  
I don't know how to make fancy sprites to appeare like in Wolfenstein :'(

##Program Structure

Game holds player  
Game holds current room 

Game reuqets to move player   
Game checks if player can move in the room  
-If can, then move  
Check if player exited a door  
-If room changed, get the new room,  
-Set curr room to this new room  
-Move player's position to the new position in this new room  

Entity handling: very complicated  
-Entity has few types:  
--Bullet type  
---Just move in the give direction until out of range or wall hit  
----If bullet more than 2 rooms away from player, delete  
--Item  
---Do nothing until player touches them?  
---Every move, check if curren't room's item is touched by player  
--Enemy  
---Holds data of what room they are currently in  
----If player is more than 2 rooms away, do nothing  
----Don't delete but do nothing  
---If player within 2 rooms, try to move towards them?  
----Movement might be bit difficult  
-----Maybe have a simplified a* algorithm, by dividing up the room into big grids  
-----Make them walk to the door leading to player?  

##Timeline
###Week 1: Ray Casting and Map Generation
* Begin exploring Ray-Cast rendering in a single room.
  * Explore Ray Casting in a Euclidean single regional map
  * Play around with range and efficiency
  * Find good number of rays to cast
  * Play arond with perspective and heights
  * Figure out 
   
* Come up with simple map generation. 
  * Use heavily modified linked list for this non-euclidean map
  * Come up with few prototype rooms and see how to randomly generate and link rooms
    * Test room generation upon Player entering and ray entering
   
###Week 2: Non-Euclidean Ray Casting Render
*Non Euclidean Rendering
  * Begin implementing Ray Casting onto the Non-Euclidean Map
  * Make sure Rays transfer proper data through

* Game Structure
  * Add player mobility
    * Forward and Backward Movement
    * Rotation (Using left right?) (Use arrows?)
  * Test room transfer

* Enemy Rendering and Spawning
  * Allow some room types to come with enemy
  * Begin looking into enemy motion

###Week 3: Player Mechanics and Enemy Mobility
* Add Bullet motion
  * Allow player to fire bullet which will also be rendered by the rays

* Add health and Ammo

* Add title Screen and Menu?
  * Allow game to be paused?


###Stretch Goals
* Textured Walls
* Enemy and Combat Mechanics may be considered Stretch Goals too
* Implementing more sophisticated path finding algorithm to enemy. 