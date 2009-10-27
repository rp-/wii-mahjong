#ifndef _game_h_
#define _game_h_

#include <wiiuse/wpad.h>

void initGame(int gm);

void killGame();

void drawGame();

bool gameWiimote(WPADData *wd_one, u32 btns_one, WPADData *wd_two, u32 btns_two);

int whatGameMode();

#endif
