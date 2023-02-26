# Dota2Cheat: now with Lua!

![Screenshot](https://user-images.githubusercontent.com/66470490/220915529-598f1e87-3047-4dc2-928b-9901808e321f.png)

SDK folder code is from another github repo, [McDota](https://github.com/LWSS/McDota) by LWSS. It's a very helpful source regarding memory structures

Using [GLFW](https://github.com/glfw/glfw), [GLEW](https://glew.sourceforge.net/) and [ImGui](https://github.com/ocornut/imgui) for the interface, cheers to their wonderful creators!

also using [Minhook](https://github.com/TsudaKageyu/minhook) for bytehooking

also using Google's [Protocol Buffers](https://github.com/protocolbuffers/protobuf) library for net message handling

also using [Lua 5.4](https://www.lua.org/) and [sol2](https://github.com/ThePhD/sol2) for scripting

## Building
Open the project in Visual Studio and build as **Release x64**

## Injecting
You need to have libprotobuf DLLs in your game folder, get them from a release in this repo or install protobuf via vcpkg and get them fron the vcpkg/installed/x64-windows/bin folder

I recommend using [Extreme Injector v3.7.2](https://www.unknowncheats.me/forum/downloads.php?do=file&id=21570) with either Thread Hijacking or the default option

It can apparently suddenly stop working at all(the .exe won't launch) so you can compile the Dota2Loader project from this repo

**ABSOLUTELY DETECTABLE BY VAC, FOR USE IN DEMO MODE OR A LOBBY**

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

* Information:
  * ~~Visible by Enemy~~ RIP on 21.02.2023, you shall not be forgotten
  * Roshan — saves his death time and approximates when he will spawn [WIP]
  * Draw circles of custom radius around you hero(e. g. to see XP receiving range)
  * Shows point-cast spells(Sunstrike, Torrent, Light Strike Array)
  * Shows target-cast spells(Assassinate, Charge of Darkness)
  * Shows Linken's Sphere effect on heroes
  * Renders any and all possible particles, including in FoW
  * Customizable enemy illusion coloring
 
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

* A Lua API for scripting, currently in development, documentation will appear at some point

I intend to implement most of the usual cheat functionality
