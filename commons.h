#define MUSIC_BAR 2000
#define SOUND_BAR 3000

#define ONE_PLAYER_GAME 1
#define TWO_PLAYER_COOP 2
#define TWO_PLAYER_VERSUS 3

#define HOVER_HINT_ON 30
#define HOVER_HINT_OFF 31

#define LAYOUTS 40

#define EXIT 999

#define NOTHING 0

#define REMARK_POS 22

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

#define CUR_FONT(a) ((opt_lang==JAPANESE?3:1) + (a?1:0))

#include "default_mtl.h"
#include "cross_mtl.h"
#include "pyramid_mtl.h"
#include "butterfly_mtl.h"
#include "fortress_mtl.h"
#include "crab_mtl.h"
#include "spider_mtl.h"

#include "ts_old_png.h"
#include "ts_spooky_png.h"
#include "ts_egypt_png.h"
#include "ts_simple_png.h"
#include "ts_default_png.h"
#include "ts_space_png.h"

#include "bk_egypt_jpg.h"
#include "bk_spooky_jpg.h"
#include "bk_space_jpg.h"
#include "gameback_jpg.h"

extern char *curtext[];
extern u8 *tex_tileset;
extern u8 *tex_gameback;
extern u8 *layouts[];

extern bool opt_rumble, opt_hoverhint, opt_widescreen, opt_voices;
extern int opt_sound, opt_music, opt_layout, opt_lang, opt_tileset;
