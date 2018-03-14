Mahjongg Wii 

# Usage

http://wiibrew.org/wiki/MahJongg_Wii

# How to build

* In ArchLinux use yaourt to install some packages.
```bash
yaourt -S blue-libs sox devkitppc-bin grrlib
```

* For 'make run' workable (send developing dol to wii through network)
```bash
make
export WIILOAD=tcp:wii
export WIILOAD=tcp:192.168.5.103  # get wii's IP on homebrew 'about' option
make run
```

# ChangeLog
v0.98 changes (devloping version)
* Build on brand new devkitpro.
* Build with old GRRLIB combined new grrlib's sub-libs(eg. zlib,...)
* Fix bug of default language of Japan wii.(dirty hack)
* Known issues:
  * English & German are the only two workable languages.
  * mod music can't play

v0.97 changes

* build with new devkitpro r18, libogc 1.8.1, libfat 1.0.5
* fix new high score always shown
* minor code cleanups

v0.96 changes

* build with new devkitpro r18, libogc 1.8.0, libfat 1.0.5
* fix array out of bounds (thx to new gcc)
* add printing version number in the main menu

v0.95 changes

* basic highscores for singleplayer and coop
6 new layouts
* show left tiles in game
* undo of last move (with time penalty)
* after a game is finished you can restart with a random layout
* fix crash if you turn of your wii while in menu
* widescreen option is now set when loading the game
* config is saved within the boot.dol folder
* code cleanup
* Known Issues
* no japanese language
* currently only english and german
* sounds are not optimal yet


v0.8 changes 23 Feburary 2009

* Added multiple tilesets!
* Added character voices on 2 player versus mode 
* Re-encoded and removed cracklys from the sound effects
* Added graphic when two player game ends as a draw
* Tidied up and fixed bugs around the placing of tiles / shuffling
* Fixed bug where saved music volume was not being used on startup
* Changed screenshot function to output filenames with date and time instead of just one
* Cleaned up the code that deals with memory allocation of textures in the menus
* Added new menu music + other music tracks for the different tilesets/themes during the game
* Fixed some memory allocation problems that may have caused memory to be held onto when mahjongg is exited and could have caused problems moving between the menu and the game if done numerous times
* Added pressing A button on already selected tile deselects it 
* Added proper support for widescreen including options to turn it on or off
* Changed config loading and saving to include widescreen and tileset selections. Does in a way that is backwards compatible with version 0.7 config files
* Added Norwegian language support
* Changed internationalisation code to fully support utf8 encoding thus giving the use of non-ASCII characters without the need for mapping
* Change to auto detect whether the game has been loaded from a loader or not and exit accordingly (either reset to menu or back to loader)
* Fixed even more bugs

v0.7 changes

* Changed to automatically select the language based on the Wii menu language settings (Nintendo only support Japanese, English, German, French, Spanish, Italian, Dutch) but as Mahjongg Wii supports more languages, I have kept the language selection screen, but moved it to be accessible via the game options screen
* Added three new tile layouts - fortress, crab and spider
* Changed placement algorithm to handle complexity of new tile layouts
* Added a layout selection screen which can be accessed by selecting change layout from options menu
* Changed all the backgrounds from png to jpeg and at same time doubled their resolution so they now look much cleaner
* Added hint functionality to the game (not hover hint) so that if it the icon is pressed a hint selection is highlighted in flashing yellow and 30 seconds are added to the time as a penalty
* Added Wiimote rumble when a tile pair is successfully selected
* Added an option to turn the rumble feature on or off
* Added the saving and loading of configuration data to SD card (mahjongg.cfg on root of SD)
* Added SDHC support
* Added Danish language support
* Wrote language selection paging - now with 9 languages per page with the plus and minus buttons (or on screen equivalents) used to move between them
* Added button B usage as an alternative back button whilst in the menus
* Added callback functions so that power and reset on the Wii now work and power button on the wiimote now turns the Wii off
* Fixed several bugs

v0.6 changes

* Added versus mode!
* Re-factored the tile selection processing and completed all coding around tile selection for both versus and coop modes
* Added option to restart the game when paused or no more moves left
* Added option to start a new game without going back to the menu when current game has finished
* Changed so that if only two tiles are left which are on top of each other the game finishes
* Fixed bug where tile placement algorithm gets stuck in an infinite loop when it cannot place all tiles successfully. Now if this occurs it finishes the shuffle and returns to no more moves mode
* Alternated which pointer is drawn first every frame to not give the advantage to player one in two player versus mode
* Added separate tile selections per player (this affects the co-op mode as well as the versus mode) 
* Added Hungarian, Finish and Swedish languages

v0.5dol changes

* Replaced boot.elf with boot.dol for compatibility with homebrew channel beta 9

v0.5 changes

* Added multi-language support currently supporting English, French, Italian, Dutch, German, Spanish, Catalan Japanese and Portuguese
* Wrote an algorithm to initialise the tiles so that tiles pairs are added in a selectable order 
* Added a language selection menu including waving flags when hovering above them
* Allowed entering and exiting pause in game by pressing the plus button on the wiimote 
* Changed menu selections so that they are only the size of the text for the chosen language

v0.4 changes

* Added multiple tile layouts (four to start with) (re-wrote tile engine to handle this)
* Added shuffle option when no more tiles pairs left
* Added an icon and count of the number of available matching tiles during the game
* Made the non-selectable tiles slightly darker that the selectable ones so easier to see
* Added a gong sound to the start of the game
* Added new true type font for all non static text (tile layout names, time in game, etc)

v0.3 changes

* Added two player co-op mode 
* Added full set of menus as per new graphics courtesy of DayDreamOz
* Added pause to game mode entered and exited by pressing a pause icon 
* I've been re-writing the way the game board works to support multiple tile layouts, as part of this I have also been developing a tile layout editor which I will also release once in a reasonable state. All this should mean that adding some of the new functionality that I want to put in will be a lot easier than before.
* New homebrew channel icon by DayDreamOz
* Cleaned up resources better
* Fixed major bugs that was causing game to dump when moving between the game and the menus
* Fixed graphical glitch on startup

v0.2 changes

* Added selected pair fading routine
* Added better font and aligned the text better 
* Added music
* Fixed accidental selection of piece below ones just removed 
* Was having problems with using modplay and mp3player at the same time so changed to use sndlib and now have both music and sound fx completed 

Controls

Menu

* Move around with the wiimote
* A - Select menu item
* B - Back to previous menu

Game

* Move around with the wiimote

* A - Select a tile / pause, etc.
* B - de-select selected tile
* Plus - pause / un-pause
* Home - quit game for menu

Player 1 always takes control of the selection of menu items, pause, etc.


Play Modes

* One player

  A single player tries to remove all the tile pairs in the shortest possible time.


* Two Player Co-op

  Two players play cooperatively trying to remove all tiles in the shortest possible time. Both players can select tiles independently with each player also having the capability to finish off the others pairing by selection a equivalent tile.


* Two Player Versus

  Two players compete against each other to get the highest score. Points are awarded for each matching tile pair selected, with different tile types being worth different amounts as follows:-

Tile Type			Pair Value
* Bamboo, Characters & Circles 	5
* Winds				10
* Seasons & Flowers 		15
* Dragons 			20

If one player gets multiple pairs in a row without the other player getting any pairs, points are multiplied by the number of pairs in a row (e.g. x2 x3 x4 etc.)

Stealing - players can select tiles already selected by the other player thus disrupting / deselecting the other players choice. 


# Credits
Thank you to all the following:

## Programming 
by JustWoody
v0.95~v0.97 by Rene Peinthor http://rp.oldsch00l.com
v0.98 by Daniel YC Lin http://github.com/ github.com/dlintw/wii-mahjong

## Graphics 
* Graphics by DayDreamOz
* Spooky, Egyptian and Space tilesets and backgrounds by Mr_Nick666

## Music
* Oriental by Allister Brimble
* Sushi by Dr Awesome
* Nebulos by audiomonster
* Childhood by Sandra Chan
* Egypt crap(?) by The freak
* Graveyard by fleshbrain

## SFX
* Versus mode characterisations by Banjo Fett
* Tile click SFX by Partners in Rhyme,
* Tile pair selected SFX by Chris,
* Tile pair mismatch SFX by Public Domain 

## Testing
* Cashman

## Translations

* French by Yod4z, Crayon and CashMan
* Italian by Charlespig
* German by Digdug
* Dutch by Digdug and Bitflusher
* Spanish by Xtract and CashMan
* Catalan by Xtract
* Japanese by HisshouBuraiKen
* Portuguese by Yod4z, Banjo Fett and CashMan
* Hungarian by Ezmegaz
* Finnish by Tubelinus
* Swedish by Tubelinus and noONE
* Danish by Buffstricker and Jedicommando 
* Norwegian by Raschi

## Development Libraries

* The project uses devkitpro, GRRLib, sndlib, libpng, wiiuse, etc.

(Thanks to all the devs on these projects for their excellent tools/libraries)
