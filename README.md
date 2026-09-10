# Unreal Engine Gameplay Prototype

> **Note:** This is a source-only snapshot of an older Unreal Engine project.
>
> I removed the original `Content/` directory before making the repo public because it contained binary game assets and other project content I don't want to redistribute. That means the project won't run as-is if you clone it.
>
> The original C++ development history is still here.

This was a personal Unreal Engine project I built while learning more about gameplay programming, AI, and systems work in C++.

I ended up spending a lot of time on enemy behavior and navigation, especially for flying enemies. The bit I’m still most fond of is a custom 3D A* pathfinding system that lets AI move through actual volumetric space instead of being limited to ground navigation.

## A few interesting bits

### 3D A* pathfinding

The navigation system builds a three-dimensional grid across X, Y, and Z inside a configurable volume.

Each node stores both its grid position and world position, and collision traces are used to decide whether that part of the grid is traversable.

Pathfinding then runs A* across the grid, including diagonal movement through 3D space. Each node can consider the surrounding 26 neighbors, with different movement costs depending on whether the step moves along one, two, or three axes.

Once a route is found, the path is retraced through parent nodes and returned as an ordered list of waypoints.

I also added debug drawing for both the navigation grid and generated paths, which made it much easier to see what the system was actually doing in-world.

Relevant code:

- `Source/Prototype/Navigation/PNavGrid.cpp`
- `Source/Prototype/Navigation/PNavNode.*`
- `Source/Prototype/Navigation/PWaypointArray.*`

### Flying enemy navigation

The pathfinding system is tied into Unreal's behavior-tree system.

A behavior-tree task finds the active navigation grid, generates a route from the flying enemy to its current target, and stores the resulting path on the blackboard for the AI to use.

Another task then consumes those waypoints and moves the enemy through them.

That let flying enemies navigate around level geometry without depending on Unreal's normal ground-based navmesh.

Relevant code:

- `Source/Prototype/AI/BehaviorTree/Tasks/PGetWaypoints.cpp`
- `Source/Prototype/AI/BehaviorTree/Tasks/PNavigatePath.cpp`

### Strafing and combat positioning

I also experimented with making enemy movement a little less direct than simply moving toward the player.

One behavior-tree task calculates possible strafe positions to either side of the target, adjusts those positions when level geometry gets in the way, and checks line of sight back to the player before picking a destination.

That gave enemies a way to reposition around the player while still trying to preserve a useful firing angle.

Relevant code:

- `Source/Prototype/AI/BehaviorTree/Tasks/PGetStrafeTarget.cpp`

### Room spawning and encounter management

The game world was split into connected rooms, with spawn points assigned to individual room IDs.

The game mode keeps track of the player's current room, spawns enemies in that room and connected neighboring rooms, and can clean up enemies when they fall outside the active area.

Room transitions are represented by objects that know which two rooms they connect, so the game can work out which nearby areas need to be populated without keeping everything active all the time.

Relevant code:

- `Source/Prototype/PrototypeGameMode.cpp`
- `Source/Prototype/Gameplay/PRoomChanger.*`
- `Source/Prototype/Gameplay/PSpawnPoint.*`

### AI and gameplay systems

There are a bunch of other experiments scattered through the project as well, including:

- custom behavior-tree tasks, decorators, and services
- AI perception and target tracking
- flying and ground-based enemies
- weapons and projectiles
- Unreal's Gameplay Ability System
- character attributes and abilities
- enemy health and damage
- player death handling
- HUD and gameplay UI

A lot of the project was me figuring out how Unreal's different gameplay systems fit together and gradually moving more logic out of Blueprints and into C++.

## Why this project exists

I’ve always liked games with interesting enemy movement and combat behavior, and this project became an excuse to dig into some of the systems behind that.

Most of my professional work has been in web applications, so working in Unreal and C++ was a very different kind of problem solving. I especially enjoyed the parts where I had to build something lower-level myself rather than just wire together existing engine behavior.

It’s definitely prototype code, and there are plenty of rough edges, but the navigation and AI experiments were a lot of fun to build and are still the parts I like most.
