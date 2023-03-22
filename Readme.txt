Hey!

The latest changes are on the imgui display when you play the game.
I'll go over everything from the start in here as well.

Controls:
  - WASD to move, Space to attack.
  - Shades have 3 health by default, and the player has 5.
  
Actors:
  - AI and player have health and can hurt each other.
  - AI has a Wander and a Hunt behavior, and Switches between them using its state and its distance to the player.
  - AI can pathfind diagonally. diagonal moves cost 1.4 points (cause sqrt(2)), and there's a const bool that turns this on or off.
  - AI and player movement use velocity and acceleration.

Collision:

  Haven't checked adding an offset between the collider and its parent object. it'll likely break things.
  That being said,
  
  - Circle x Circle: implemented properly as far as i could tell.
  - AARect x AARect: implemented properly as far as i could tell.
  - Circle x Axis-aligned rectangles: Implemented halfway - there's no resolution for cases where both the circle and the rectangle are dynamic.
  - The map spawns square colliders wherever it's marked as "unwalkable".
  - There's TileCollisionEvents, so I could use that for doors and triggers - but sadly I didn't have the time to experiment with that.
  
  - There's a bool in Framework/Source/FWConsts.h that lets you render them onscreen for debug purposes. It's fun.
 
Map:
  - Sprites like straight walls, corners, floors with different shading, doors and columns are selected automatically.
  - Sprite selection and gathering an "isWalkable" array is done once on start and doesn't run every frame.
  - Tilemaps accurately convert world-space to grid-space and vice versa, accounting for offset and scale.
  - The map can spawn dynamic objects, as demonstrated with crates.

Misc. funky stuff in the project:
  - Object indices are ordered by Y value and rendered in that order. That way, objects with a lower Y position are drawn first, so you can go "Behind" objects.
  - MouseWheel input is read and applied to camera - you can zoom.
  - On draw, every Gameobject and the camera is moved slightly so their pixels line up with the window's pixels, reducing artifacting. This doesn't work properly if zoom isn't 1.
  - Colors are multiplied in the frag file so we have tinted sprites.
  
In case I don't see you, Have a wonderful break!