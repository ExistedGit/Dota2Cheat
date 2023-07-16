# Dota2Cheat: now crash-free!

![D2C image v3](https://github.com/ExistedGit/Dota2Cheat/assets/66470490/2b5a66ba-c6fe-4f27-9009-914135948216)

There weren't many open-source Dota cheats. And so I decided to fix that.
Written in C++20 with love

Some SDK bits were fully or partionally taken from [McDota](https://github.com/LWSS/McDota) by LWSS. It's a very helpful source regarding memory structures

Using [ImGui](https://github.com/ocornut/imgui) with DirectX 11 for the interface

also using [Minhook](https://github.com/TsudaKageyu/minhook) for bytehooking

also using Google's [Protocol Buffers](https://github.com/protocolbuffers/protobuf) library for net message handling

also using DarthTon's [BlackBone](https://github.com/DarthTon/Blackbone) for injection

Cheers to their wonderful creators!

`assets` folder contents are property of Valve Corporation

# Features
To open the cheat menu, press Insert

Dota2Cheat follows the legit paradigm, so you probably won't see AutoStop, AutoLastHit or AutoSteal here. Not only because I don't have time for complicated multipurpose scripts for each hero, but also because they can often be spotted without even replaying the game. It's also my personal opinion that if you can't press two buttons in a sequence, then perhaps Dota is not for you.

This list is no longer subject to frequent change as I'm only refining existing features. I have little interest in just reusing instruments I already have. I'd much rather have a good cheat with a few truly needed features than a detectable, crashing, lagging monstrosity.

* Automation:
  * AutoAccept:
    * Customizable delay
  * Uses Magic Wand and Faerie Fire with customizable health tresholds
  * AutoMidas with customizable min XP reward
  * Bounty rune & Aegis pickup
  * Redirects spell casts from illusions to the real hero
  * Mana & HP abuse with items like Arcane Boots
  * AutoDodge for projectiles
  * ~~Tome of Knowledge purchase~~ RIP on 21.04.2023. Goodbye, sweet prince.
  
* Information:
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
  * ~~Visible by Enemy~~ RIP on 21.02.2023, you shall not be forgotten
  
* Convars:
  * Using ConVar spoofing to counter serverside detection mechanisms 
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
  * Tree changer(can revert to default trees without reconnecting)
  * Allows playing with VPK mods
  
* Work in progress:
  * Roshan timer
  
I intend to get most of D2C's functionality to work in the most proper way possible. Y'know, to rebuild functions and remove signatures, to use less detectable and/or more comfortable methods. That kinda stuff.

## Building
You need to install the protobuf library for it to work. Install [vcpkg](https://vcpkg.io/en/getting-started.html). Navigate to the folder with vcpkg.exe. Open the console in the folder and enter the following command:

`.\vcpkg.exe install protobuf:x64-windows-static-md`

This will install the heavier static version of the library. 

Once the process completes, go to `installed/x64-windows-static-md`. Copy libprotobuf.lib and libprotobuf-lite.lib files from /lib to Dota2Cheat/lib. You can also copy the libs with a `d` suffix from debug/lib if you want to compile it in Debug

You also need to install the [June 2010 DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

Open the project in Visual Studio and build both Dota2Cheat and Dota2Loader as **Release x64**

**Note**: If you got an error compiling `Dota2Loader` and `BlackBone` you must add these https://github.com/paulhobbel/Blackbone/tree/master/DIA/x64 files to the `Dota2Loader/DIA/x64` folder. The `msdia140.dll` and `symsrv.dll` DLL's are important files for injecting, especially on `Windows 11`.

## Injecting
This cheat can be safely reinjected at any point of the game

Build Dota2Loader and launch it, it will move the necessary resources around and then inject

**I KNOW IT DOESN'T WORK ON WIN11, STOP MESSAGING ME ABOUT IT. TRY TO [USE](https://github.com/ExistedGit/Dota2Cheat/issues/124) SOME [OTHER](https://github.com/ExistedGit/Dota2Cheat/issues/120) INJECTOR THAT WORKS ON WIN11(JUST DON'T FORGET TO STILL LAUNCH IT, TO MOVE ASSETS)**

**Use at own risk. Though I use all available methods to lower the detection chance, there are no guarantees.** (also consider not using non-legit features that you can get reported/flagged for)

## Troubleshooting

If you encounter a crash and want to inform me of it, do the following:

### Debugging

This method is for errors that are easily reproducible. You're meant to be playing either in the demo mode or a lobby(not on a VAC-protected server!)
Build both the cheat and loader in Debug. Launch dota 2, then In Visual Studio Ctrl + Alt + P and select dota2.exe.
Now that you're debugging the process, inject the cheat and trigger the error. VS will show you where the exception occurs.

### Testing

Bugs don't always happen when you need them to. They strike viciously, when you don't expect it. If you have what it takes to be a tester, here are your instruments:

* [x64dbg](https://x64dbg.com/) — debugger
* [ScyllaHide](https://github.com/x64dbg/ScyllaHide/releases/tag/v1.4) — plugin for x64dbg that allows you to hide the debugger ring3 programs(VAC is one) 
* [Use](https://github.com/zyhp/vac3_inhibitor) a [vac bypass](https://github.com/danielkrupinski/VAC-Bypass) so that VAC doesn't see your LoadLibrary trickery
* An alt account, since disabling VAC only means you won't be banned. No information sent by VAC is suspicious too, you'll be placed in a player pool with cheaters(as I noticed myself)
* Dota2Cheat & Dota2Loader compiled in the Testing configuration such that they function like Release but contain debug information

### What to do when an error appears

Upon an exception, examine the file and lines it happened on. In the Call Stack tab you'll be able to trace the call to the function(in case the exception place doesn't provide enough information). Screenshot all of this and then you can make an Issue here with the data.

# For Developers

If you want to tinker with the sources, compile both the cheat and loader as Debug x64(it will use LoadLibrary to allow for debugging and the overlay will not cover the whole screen)

Check out the repo's Wiki, I add useful informations about dota-specific cheat things there

Also has a guide on how to update it
