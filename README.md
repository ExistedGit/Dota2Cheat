# Dota2Cheat: now with Lua!

![Screenshot_517](https://user-images.githubusercontent.com/66470490/224436497-2e7a5e86-10d8-4d80-94c5-d8eab78b7aef.png)

There weren't many open-source Dota cheats. And so I decided to fix that.
Written in C++20 with love

Some SDK bits were fully or partionally taken from [McDota](https://github.com/LWSS/McDota) by LWSS. It's a very helpful source regarding memory structures

Using [GLFW](https://github.com/glfw/glfw), [GLEW](https://glew.sourceforge.net/) and [ImGui](https://github.com/ocornut/imgui) for the interface, cheers to their wonderful creators!

also using [Minhook](https://github.com/TsudaKageyu/minhook) for bytehooking

also using Google's [Protocol Buffers](https://github.com/protocolbuffers/protobuf) library for net message handling

also using [Lua 5.4](https://www.lua.org/) and [sol2](https://github.com/ThePhD/sol2) for scripting

`assets` folder contents are property of Valve Corporation

## Building
You need to install the protobuf library for it to work. Install [vcpkg](https://vcpkg.io/en/getting-started.html). Navigate to the folder with vcpkg.exe. Open the console in the folder

Here you have two ways:

`.\vcpkg.exe install protobuf:x64-windows`
or
`.\vcpkg.exe install protobuf:x64-windows-static-md`

The first one will install the default version of the lib. You will have to get libprotobuf DLLs from a release in this repo and move them to the folder with dota2.exe

The second one will install the heavier static version which will not require anything.

Once the process completes, go to `installed/x64-windows` or `installed/x64-windows-static-md`, depending on your choice before. Copy libprotobuf.lib and libprotobuf-lite.lib files from /lib to Dota2Cheat/lib. You can also copy the libs with a `d` suffix from debug/lib if you want to compile it in Debug

Open the project in Visual Studio and build as **Release x64**

## Injecting

This cheat can be safely reinjected at any point of the game

Build Dota2Loader and launch its .exe to inject(not from Visual Studio, it messes up the file paths!)

**ABSOLUTELY DETECTABLE BY VAC, FOR USE IN DEMO MODE OR A LOBBY** (Memory Map support WIP)

# Features
To open the cheat menu, press Insert

This list is subject to frequent change as I'm testing new features

* Automation:
  * Uses Magic Wand and Faerie Fire with customizable health tresholds
  * Uses Hand of Midas on nearby big neutrals, ranged & flagbearer lane creeps and catapults
  * Bounty rune & Aegis pickup
  * Tome of Knowledge purchase
  * Redirects spell casts from illusions to the real hero
  * Mana & HP abuse with items like Arcane Boots
  * AutoDodge for projectiles
  
* Information:
  * ~~Visible by Enemy~~ RIP on 21.02.2023, you shall not be forgotten
  * AbilityESP 
  * Draw circles of custom radius around you hero(e. g. to see XP receiving range)
  * Shows point-cast spells(Sunstrike, Torrent, Light Strike Array)
  * Shows target-cast spells(Assassinate, Charge of Darkness)
  * Shows Linken's Sphere effect on heroes
  * Renders any and all possible particles, including in FoW
  * Customizable enemy illusion coloring
  * Shows trajectories of enemy projectiles like Mirana's arrow and non-projectile abilities like Meat Hook
  * Roshan — saves his death time and approximates when he will spawn [WIP]
  
* Convars:
  * `sv_cheats` spoofing
  * customizable `dota_camera_distance` with proper `r_farz` and `fog_enable`(no clipping or blue fog)
  * ~~`dota_use_particle_fow`~~ RIP too

* Utility:
  * Perfect Blink
  * Prevents bad spell casts(such as Black Hole/Chronosphere with no one in its radius)

* Visuals:
  * Weather changing(Ash, Spring etc)
  * River painting

I intend to implement most of the usual cheat functionality

# For Developers

Check out the repo's Wiki, I add useful informations about dota-specific cheat things there

Also has a guide on how to update it
