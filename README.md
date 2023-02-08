# Dota2Cheat: now with ImGui!

![Screenshot](https://user-images.githubusercontent.com/66470490/217580749-87723f6b-6fa4-4b0d-998a-1c9400c06302.png)

SDK folder code is from another github repo, McDota by LWSS. It's a very helpful source regarding memory structures

Using GLFW, GLEW and ImGui for the interface, cheers to their wonderful creators!
also using Minhook for bytehooking
## Building
Open the project in Visual Studio and build as **Release x64**
## Injecting
I recommend using [Extreme Injector v3.7.2](https://www.unknowncheats.me/forum/downloads.php?do=file&id=21570) with either Thread Hijacking or the default option

# Features
To open the cheat menu, press Insert

This list is subject to frequent change as I'm testing new features

* Automation:
  * Magic Wand and Faerie Fire usage with customizable health tresholds
  * Hand of Midas usage on nearby big neutrals, ranged & flagbearer lane creeps and catapults
  * Bounty rune pickup
  * Tome of Knowledge purchase
  * Redirects spell casts from illusions to the real hero
  * Mana & HP abuse with items like Arcane Boots

* Information:
  * Visible by Enemy — shows a Skyrim-inspired HIDDEN/DETECTED text and/or a particle under your hero
  * Roshan — saves his death time and approximates when he will spawn [WIP]
  * Draw circles of custom radius(e. g. to see XP receiving range)
  * Highlights enemy sun strikes as if they were yours

* Convars:
  * `sv_cheats` spoofing
  * customizable `dota_camera_distance` with proper `r_farz` and `fog_enable`(no clipping or blue fog)
  * `dota_use_particle_fow` — shows more particles in FoW, including teleportation effects

* Utility:
  * Bypasses Blink Dagger's overshoot which normally makes you only blink for 1000 units if clicked out of its casting range

* Visuals:
  * Weather changing(Ash, Spring etc)
  * Customizable illusion coloring

I intend to implement most of the usual cheat functionality
