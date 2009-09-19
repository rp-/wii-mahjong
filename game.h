#ifndef _game_h_
#define _game_h_

#include <wiiuse/wpad.h>

void initGame(int gm);

void setLayout(int laynum);

void initGrid();

void resetToPlaces();

void checkSelectable();

bool isSelectable(int x, int y, int z);

bool isHigher(char x,char y,char z);

void killGame();

void setupGame();

//void shufflePieces(int shuffles);

void mixPairs(int shuffles);

void drawGame();

void placeTilePair();

void checkAndSaveGrid();

bool checkWithoutTile1(int tile1, int tile2);

void placeRemainingtiles();

int getTile();

bool checkTile(int tile);

bool isCovered(int x,int y,int z);

void drawBoard();

void drawTime();

void drawMatches();

void drawScores();

void drawTilesLeft();

void drawPauseIcon();

void drawHintIcon();

void drawUndoIcon();

void undo();

void storeUndo( int sel1, int sel2);

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
#endif
