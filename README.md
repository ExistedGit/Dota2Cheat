# Dota2Cheat: now with Dota Plus!

![cheat preview 27 03 2023](https://user-images.githubusercontent.com/66470490/228053121-ed8cbc2b-8f9f-4db7-914c-81db2dad518d.png)

There weren't many open-source Dota cheats. And so I decided to fix that.
Written in C++20 with love

Some SDK bits were fully or partionally taken from [McDota](https://github.com/LWSS/McDota) by LWSS. It's a very helpful source regarding memory structures

Using [GLFW](https://github.com/glfw/glfw), [GLEW](https://glew.sourceforge.net/) and [ImGui](https://github.com/ocornut/imgui) for the interface, cheers to their wonderful creators!

also using [Minhook](https://github.com/TsudaKageyu/minhook) for bytehooking

also using Google's [Protocol Buffers](https://github.com/protocolbuffers/protobuf) library for net message handling

also using [Lua 5.4](https://www.lua.org/) and [sol2](https://github.com/ThePhD/sol2) for scripting

also using DarthTon's [BlackBone](https://github.com/DarthTon/Blackbone) for injection

also using [libcurl](https://github.com/curl/curl) for network requests

`assets` folder contents are property of Valve Corporation

## Building
You need to install the protobuf library for it to work. Install [vcpkg](https://vcpkg.io/en/getting-started.html). Navigate to the folder with vcpkg.exe. Open the console in the folder and enter the following command:

`.\vcpkg.exe install protobuf:x64-windows-static-md`

This will install the heavier static version of the library. 

Once the process completes, go to `installed/x64-windows-static-md`. Copy libprotobuf.lib and libprotobuf-lite.lib files from /lib to Dota2Cheat/lib. You can also copy the libs with a `d` suffix from debug/lib if you want to compile it in Debug

Open the project in Visual Studio and build both Dota2Cheat and Dota2Loader as **Release x64**

## Injecting

This cheat can be safely reinjected at any point of the game

Build Dota2Loader and launch it, it will move the necessary resources around and then inject

**Use at own risk. Though I use methods that make detection chances as low as possible, there are no guarantees. [Use](https://github.com/zyhp/vac3_inhibitor) a [vac bypass](https://github.com/danielkrupinski/VAC-Bypass)** (also consider not using non-legit features that you can get reported for)

## Troubleshooting

If you encounter a crash and want to inform me of it, do the following:

Build both the cheat and loader in Debug. Launch dota 2, then In Visual Studio Ctrl + Alt + P and select dota2.exe.
Now that you're debugging the process, inject the cheat. VS will show you where the exception occurs.
Screenshot both what place it crashed in and the Call Stack(it's one of the bottom menus).
You can also screenshot exact places the call stack entries take you to(double-click them).

And thus we can defeat bugs together!

# Features
To open the cheat menu, press Insert

This list is subject to frequent change as I'm testing new features

* Automation:
  * AutoAccept:
    * Customizable delay
    * Optional notifications sent straight to your Telegram(start [our bot](https://t.me/dotacheatnotifybot), get [your user ID](https://t.me/getmyid_bot) and plug it into the input field!)
  * Uses Magic Wand and Faerie Fire with customizable health tresholds
  * AutoMidas with customizable min XP reward
  * Bounty rune & Aegis pickup
  * ~~Tome of Knowledge purchase~~ RIP on 21.04.2023. Goodbye, sweet prince.
  * Redirects spell casts from illusions to the real hero
  * Mana & HP abuse with items like Arcane Boots
  * AutoDodge for projectiles
  
* Information:
  * ~~Visible by Enemy~~ RIP on 21.02.2023, you shall not be forgotten
  * Maphack:
    * Teleport Tracker — draws enemy TPs on the map
    * Particle MapHack — shows heroes in FoW when they create particles(if possible)
  * AbilityESP & ItemESP
  * Bars:
    * Manabars
    * HP amount displayed on healthbar
  * Indicators
    * Speed indicator — shows whether you are faster or slower than the enemy
    * Kill indicator — shows if you can kill the enemy with a nuke(if your hero has one). If you can't, shows how much more health there is than the treshold.
  * Draw circles of custom radius around you hero(e. g. to see XP receiving range)
  * Shows point-cast spells(Sunstrike, Torrent, Light Strike Array)
  * Modifier Revealer:
    * Shows target-cast spells(Assassinate, Charge of Darkness)
    * Shows Linken's Sphere on everyone
    * Shows True Sight on allies and wards
  * Renders any and all possible particles, including in FoW
  * Customizable enemy illusion coloring
  * Shows trajectories of enemy projectiles like Mirana's arrow and non-projectile abilities like Meat Hook
  
* Convars:
  * `sv_cheats` spoofing
  * customizable `dota_camera_distance` with proper `r_farz` and `fog_enable`(no clipping or blue fog)
  * ~~`dota_use_particle_fow`~~ RIP too

* Utility:
  * Perfect Blink
  * Prevents bad spell casts(such as Black Hole/Chronosphere with no one in its radius)

* Changer:
  * Weather changing(Ash, Spring etc)
  * River painting
  * Unlocks emoticons
  * Dota Plus Unlocker
  
* Work in progress:
  * Roshan timer
  
I intend to implement most of the usual cheat functionality

# For Developers

If you want to tinker with the sources, compile both the cheat and loader as Debug x64(it will use LoadLibrary to allow for debugging and the overlay will not cover the whole screen)

Check out the repo's Wiki, I add useful informations about dota-specific cheat things there

Also has a guide on how to update it
