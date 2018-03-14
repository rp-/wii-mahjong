#ifndef _commons_h_
#define _commons_h_

#define VERSION_STRING "V098"

#define MUSIC_BAR 2000
#define SOUND_BAR 3000

#define ONE_PLAYER_GAME 1
#define TWO_PLAYER_COOP 2
#define TWO_PLAYER_VERSUS 3

#define HOVER_HINT_ON 30
#define HOVER_HINT_OFF 31

#define LAYOUTS 12
#define FILE_HIGHSCORE "high.sco"

#define EXIT 999

#define NOTHING 0

#define ENGLISH 0
#define FRENCH 1
#define ITALIAN 2
#define HUNGARIAN 3
#define DUTCH 4
#define GERMAN 5
#define SPANISH 6
#define FINNISH 7
#define CATALAN 8
#define JAPANESE 9
#define PORTUGUESE 10
#define SWEDISH 11
#define DANISH 12
#define NORWEGIAN 13

#define MAX_WIDTH 30
#define MAX_HEIGHT 16
#define MAX_LAYERS 5
#define MAX_TILES 144

#define DEFAULT 0
#define SIMPLE 1
#define OLD 2
#define SPOOKY 3
#define EGYPTIAN 4
#define SPACE 5

#include "languages.h"
#include "mahjongglayouts.h"

#include "gfx/ts_old_png.h"
#include "gfx/ts_spooky_png.h"
#include "gfx/ts_egypt_png.h"
#include "gfx/ts_simple_png.h"
#include "gfx/ts_default_png.h"
#include "gfx/ts_space_png.h"

#include "gfx/bk_egypt_jpg.h"
#include "gfx/bk_spooky_jpg.h"
#include "gfx/bk_space_jpg.h"
#include "gfx/gameback_jpg.h"

#define CONTAINS( vx, vy, x, y, width, height) ((vx) > (x) && (vx) < (x + width) && (vy) > (y) && (vy) < (y + height))
#define CUR_FONT(a) ((opt_lang==JAPANESE?3:1) + (a?1:0))

extern char** curtext;
extern u8 *tex_tileset;
extern u8 *tex_gameback;
extern u8 *layouts[];
extern unsigned long g_scores[LAYOUTS * 2];

extern bool opt_rumble, opt_hoverhint, opt_widescreen, opt_voices;
extern int opt_sound, opt_music, opt_layout, opt_lang, opt_tileset;
#endif
