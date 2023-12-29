# Dota2Cheat 7.35b: now with VPK decoding! 

![v4 image](https://github.com/ExistedGit/Dota2Cheat/assets/66470490/88dbfa94-0ba3-4d53-b355-8cb18de98d71)

There weren't many open-source Dota cheats. And so I decided to fix that. Written in C++20 with love.

Some SDK bits were fully or partionally taken from [McDota](https://github.com/LWSS/McDota) by LWSS, which D2C is inspired by.

Using [ImGui](https://github.com/ocornut/imgui) with DirectX 11 for the interface

also using [Minhook](https://github.com/TsudaKageyu/minhook) for them trampolines

also using Google's [Protocol Buffers](https://github.com/protocolbuffers/protobuf) for net message handling

Cheers to their wonderful creators!

# Features
To open the cheat menu, press Insert

WARNING! Legit features ahead, no scripts that play the game for you!

* Automation:
  * AutoAccept:
    * Customizable delay
  * AutoHeal with customizable health tresholds
  * AutoMidas with customizable min XP reward
  * Bounty rune & Aegis snatcher
  * CastRedirection™(always casts on real hero)
  * Mana & HP abuse
  * AutoDodge for projectiles
  * ~~Tome of Knowledge purchase~~ RIP on 21.04.2023. Goodbye, sweet prince.
  
* Information:
  * Maphack:
    * Teleport Tracker — draws enemy TPs on the map
    * Particle MapHack — shows particles in FoW and identifies their source()
  * AbilityESP™(abilities & items)
  * Bars:
    * Manabars
    * HP amount displayed on healthbar
  * Indicators
    * Speed indicator — shows whether you are faster or slower than the enemy
    * Kill indicator — shows if you can kill the enemy with a nuke(if your hero has one). If you can't, shows how much more health there is than the treshold.
  * Draw circles of custom radius around your hero(e. g. to see XP receiving range)
  * Shows point-cast spells(Sunstrike, Torrent, Light Strike Array)
  * Modifier Revealer:
    * Shows target-cast spells(Assassinate, Charge of Darkness)
    * Shows Linken's Sphere on everyone
    * Shows True Sight on allies and wards
  * Customizable enemy illusion coloring
  * Shows trajectories of enemy projectiles like Mirana's arrow and non-projectile abilities like Meat Hook
  * ~~Visible by Enemy~~ RIP on 21.02.2023, you shall not be forgotten
  
* Convars:
  * Using ConVar spoofing to counter serverside detection mechanisms 
  * customizable `dota_camera_distance` with proper `r_farz` and `fog_enable`(no clipping or blue fog)
  * ~~`dota_use_particle_fow`~~ RIP too

* Utility:
  * Perfect Blink
  * BadCastPrevention™(prevents bad BHs and RPs and chronospheres)

* Changer:
  * Weather changing(Ash, Spring etc)
  * River painting
  * Unlocks emoticons
  * Dota Plus Unlocker
  * TreeChanger™
  * Allows playing with VPK mods
  
* Work in progress:
  * Roshan timer

## Building
You need to install the protobuf library for it to work. Install [vcpkg](https://vcpkg.io/en/getting-started.html). Navigate to the folder with vcpkg.exe. Open the console in the folder and enter the following command:

`.\vcpkg.exe install protobuf:x64-windows-static`

This will install the heavier static version of the library. 

Once the process completes, go to `installed/x64-windows-static`. Copy libprotobuf.lib and libprotobuf-lite.lib files from /lib to Dota2Cheat/lib. You can also copy the libs with a `d` suffix from debug/lib if you want to compile it in Debug

You also need to install the [June 2010 DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

Open the project in Visual Studio and build Dota2Cheat as **Release x64**

## Injecting
This cheat can be safely reinjected at any point of the game

Build & run D2CUpdater to move/update all relevant resources to their destination in C:\Users\%user%\Documents\Dota2Cheat, then use any injector to do the job.

**Use at own risk** (also consider not using non-legit features that you can get reported/flagged for)

## Troubleshooting

If you encounter an error and want to coherently report it:

### Debugging

This method is for errors that are easily reproducible. You're meant to be playing either in the demo mode or a lobby(not on a VAC-protected server!)
Build both the cheat and loader in Debug. Launch dota 2, then In Visual Studio Ctrl + Alt + P and select dota2.exe.
Now that you're debugging the process, inject the cheat and trigger the error. VS will show you where the exception occurs.

### What to do when an error appears

Upon an exception, examine the file and lines it happened on. In the Call Stack tab you'll be able to trace the call to the function(in case the exception place doesn't provide enough information). Screenshot all of this and then you can make an Issue here with the data.

# For Developers

Check out the repo's Wiki, I add useful pieces of information about dota-specific cheat things there

Also has a guide on how to update it

AND I also consider translating my YouGame tutorials to a human language for the sake of preservation
