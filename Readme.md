
<h2><strong>ABOUT</strong></h2>

<p>A small 2D dungeon crawling prototype in an OpenGL framework.</p>

<p>This is mostly a demonstration of the framework rather than a fully functioning game.</p>

<ul>
	<li>Use WASD to move and Space to attack.</li>
	<li>Shades have 3 health by default, and the player has 5.</li>
	<br>
</ul>

<div align="center"><a href="http://www.youtube.com/watch?v=AlJqBda12tc"><img src="https://drive.google.com/uc?export=view&id=1FUNKGmx2TZT8PgescX1WkVNvwU9uXMHm" width="75%"><img></a></div>

<h2><strong>FEATURES</strong></h2>

<h3>Tile Map</h3>

<ul>
	<li>The level is generated using tilemap data described in an array.</li>
	<li>Sprites like walls, shaded floors, columns and doors are selected automatically based on their adjacent tiles.</li>
	<li>Tiles have characteristics that describe further behavior (such as whether or not they have colliders).</li>
</ul>

<h3>AI</h3>

<ul>
	<li>Uses A* pathfinding and a state machine structure.</li>
	<li>Both the AI and player have health and can hurt each other.</li>
	<li>AI has a Wandering and a Hunting behavior, and Switches between them using its state and its distance to the player.</li>
	<li>AI and player movement use velocity and acceleration.</li>
</ul>

<h3>Collisions</h3>

<ul>
	<li>The prototype features simple circle and rectangle colliders written from scratch.</li>
	<li>The map spawns square colliders where the tilemap calls for them.</li>
	<li>The player, crates and enemies spawn a circle collider on their end.</li>
	<li>There&#39;s a bool in Framework/Source/FWConsts.h that lets you render them onscreen for debugging purposes.</li>
</ul>

<h3>Misc.</h3>

<ul>
	<li>MouseWheel input is read and applied to camera - you can zoom.</li>
	<li>On draw, every Gameobject and the camera is moved slightly so their pixels line up with the rendered window&#39;s pixels, reducing artifacting
	<ul>
		<li>This doesn&#39;t work properly if zoom isn&#39;t at the default value.</li>
	</ul>
	</li>
	<li>We can easily tint sprites using the color variable of their owning gameobject.</li>
</ul>
