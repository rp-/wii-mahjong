//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <malloc.h>
//#include <math.h>
//#include <ogcsys.h>
//#include <gccore.h>
#include <wiiuse/wpad.h>

#include "asndlib.h"       // sound library

//#include <fat.h>
//#include <time.h>

#include "GRRLIB/GRRLIB.h"

#include "gamenumbers_png.h"

#include "libpng/pngu/pngu.h"

//#include "gameback_jpg.h"

#include "ts_spooky_png.h"
#include "bk_spooky_jpg.h"

//#include "tileset_png.h"
#include "clock_png.h"
#include "matches_png.h"
#include "playerone_png.h"
#include "playertwo_png.h"
#include "pause_png.h"
#include "paused_png.h"
#include "pauseover_png.h"
#include "hint_png.h"
#include "hintover_png.h"
#include "finished_png.h"
#include "nomorematches_png.h"
#include "winnerone_png.h"
#include "winnertwo_png.h"
#include "draw_png.h"

#include "Click17a_raw.h"
#include "gromb_raw.h"
#include "ptwiiing_raw.h"
#include "gong_raw.h"

#include "a_yokatta_raw.h"
#include "yatta_raw.h"
#include "Haha_raw.h"
#include "Haha2_raw.h"
#include "oh_raw.h"
#include "ohhh_raw.h"
#include "mm_raw.h"
#include "mmmm_raw.h"
#include "aiee_raw.h"
#include "nani_raw.h"
#include "doshiyo_raw.h"
#include "uss_raw.h"

void initGame(int gm);

void setLayout(int laynum);

void initGrid();

void resetToPlaces();

void checkSelectable();

bool isSelectable(int x, int y, int z);

boolean isHigher(char x,char y,char z);

void killGame();

void setupGame();

//void shufflePieces(int shuffles);

void mixPairs(int shuffles);

unsigned GRRLIB_Rand(unsigned long MAX);

void drawGame();

void placeTilePair();

void checkAndSaveGrid();

bool checkWithoutTile1(int tile1, int tile2);

void placeRemainingtiles();

int getTile();

bool checkTile(int tile);

boolean isCovered(int x,int y,int z);

void drawBoard();

void drawTime();

void drawMatches();

void drawScores();

void drawPauseIcon();

void drawHintIcon();

void drawTile(f32 xpos, f32 ypos, unsigned char tile, bool sel, int player);

void drawRichTile(f32 xpos, f32 ypos, unsigned char tile,f32 scale,u8 alpha);

bool gameWiimote(WPADData *wd_one, u32 btns_one, WPADData *wd_two, u32 btns_two);

void selectProcessing(WPADData *wd, int player);

void removeMatched(int player,int s1, int s2);

void finishGame();

bool isSameTile(int s1, int s2);

bool selectTile(f32 x, f32 y,int selnum);

bool selectExactTile(int x, int y, int z, int selnum);

void removeTile(int selnum);

void clearSelected();

bool checkMatch();

int calcScore(int player);

bool goesLeft();

void findHint();

void compressTiles();

char realTileNum(char tileNum);

void setupFade(int n);

int whatGameMode();
