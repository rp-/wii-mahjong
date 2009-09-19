#include "GRRLIB/GRRLIB.h"
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <string.h>

#include "letterswhite_png.h"
#include "letters_png.h"

#include "japanwhite_png.h"
#include "japan_png.h"

#include "latin_map.h"
#include "japanese_map.h"

#include "english_lng.h"
#include "french_lng.h"
#include "italian_lng.h"
#include "dutch_lng.h"
#include "german_lng.h"
#include "spanish_lng.h"
#include "catalan_lng.h"
#include "japanese_lng.h"
#include "portuguese_lng.h"
#include "hungarian_lng.h"
#include "finnish_lng.h"
#include "swedish_lng.h"
#include "danish_lng.h"
#include "norwegian_lng.h"

#include "libpng/pngu/pngu.h"

#include "asndlib.h"       // sound library

#include "Click17a_raw.h"
#include "gromb_raw.h"

// main menu graphics
#include "mainback_jpg.h"
#include "mainfore_png.h"
#include "shanghi_png.h"

// play menu graphics
#include "playback_jpg.h"
#include "shade_png.h"
#include "playfore_png.h"

// options menu graphics
#include "optionsback_jpg.h"
#include "optionsfore_png.h"

// sound menu graphics
#include "soundback_jpg.h"
#include "unselectedball_png.h"
#include "selectedball_png.h"
#include "soundfore_png.h"

// language menu graphics
#include "flags_png.h"
#include "languages_png.h"
#include "HSminusButton_png.h"
#include "HSplusButton_png.h"

// layout menu graphics
#include "layoutback_png.h"
#include "smalltile_png.h"

// tileset menu graphics
#include "border_png.h"


void initMenu();

void setLanguage();

void killMenu();

void killMenuOther();

void killMenuLanguages();

void parseMenu(char *str, u32 str_len);

void parseLine(char *str, int menu);

void drawMenu(WPADData *wd);

void drawLayout(int n);

int menuWiimote(WPADData *wd, u32 wpaddown);

void checkSelected(WPADData *wd);

void initMainMenu();

void killMainMenu();

void initPlayMenu();

void killPlayMenu();

void initSoundMenu();

void killSoundMenu();

void initOptionMenu();

void initLangMenu();

void killLangMenu();

void initLayoutMenu();

void killLayoutMenu();

void initTilesetMenu();

void killTilesetMenu();

void playWrong();

void playClick();
