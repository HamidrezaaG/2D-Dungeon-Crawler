<p><strong>ABOUT</strong></p>

A small 2D dungeon crawling prototype in an OpenGL framework.
This is mostly a demonstration of the framework rather than a functioning game.

Controls:
  - WASD to move, Space to attack.
  - Shades have 3 health by default, and the player has 5.

<p><strong>FEATURES</strong></p>

Tile Map:
  - The level is generated using tilemap data described in an array.
  - Sprites like walls, floors with different shading, columns and doors are automatically selected based on what their adjacent tiles are.
  - Tiles have characteristics that describe further behavior (such as whether or not they have colliders).
  
AI:
  - Uses A* pathfinding and a state machine structure.
  - Both the AI and player have health and can hurt each other.
  - AI has a Wandering and a Hunting behavior, and Switches between them using its state and its distance to the player.
  - AI and player movement use velocity and acceleration.

Collisions:
  The prototype features simple circle and rectangle colliders written from scratch.
  - The map spawns square colliders where the tilemap calls for them.
  - The player, crates and enemies spawn a circle collider on their end.
  - There's a bool in Framework/Source/FWConsts.h that lets you render them onscreen for debugging purposes.

Misc. funky stuff in the project:
  - Object indices are ordered by Y value and rendered in that order. That way, objects with a lower Y position are drawn first, so you can go "Behind" objects.
  - MouseWheel input is read and applied to camera - you can zoom.
  - On draw, every Gameobject and the camera is moved slightly so their pixels line up with the rendered window's pixels, reducing artifacting.
    - (This doesn't work properly if zoom isn't at the default value.)
  - We can easily tint sprites using the color variable of their owning gameobject.
  
