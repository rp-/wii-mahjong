/*===========================================
		Mahjongg Solitaire Wii
		Code     : Justin Wood

		Game class
 ============================================*/
#include "game.h"
#include "commons.h"

#include "GRRLIB/GRRLIB.h"
#include "asndlib.h"       // sound library

#include "gfx/gamenumbers_png.h"

//#include "gameback_jpg.h"

#include "gfx/bk_spooky_jpg.h"

//#include "tileset_png.h"
#include "gfx/clock_png.h"
#include "gfx/matches_png.h"
#include "gfx/playerone_png.h"
#include "gfx/playertwo_png.h"
#include "gfx/pause_png.h"
#include "gfx/paused_png.h"
#include "gfx/pauseover_png.h"
#include "gfx/hint_png.h"
#include "gfx/hintover_png.h"
#include "gfx/undo_png.h"
#include "gfx/undoover_png.h"
#include "gfx/finished_png.h"
#include "gfx/nomorematches_png.h"
#include "gfx/winnerone_png.h"
#include "gfx/winnertwo_png.h"
#include "gfx/draw_png.h"

#include "sound/Click17a_raw.h"
#include "sound/gromb_raw.h"
#include "sound/ptwiiing_raw.h"
#include "sound/gong_raw.h"

#include "sound/a_yokatta_raw.h"
#include "sound/yatta_raw.h"
#include "sound/Haha_raw.h"
#include "sound/Haha2_raw.h"
#include "sound/oh_raw.h"
#include "sound/ohhh_raw.h"
#include "sound/mm_raw.h"
#include "sound/mmmm_raw.h"
#include "sound/aiee_raw.h"
#include "sound/nani_raw.h"
#include "sound/doshiyo_raw.h"
#include "sound/uss_raw.h"

#define BLANK 150
#define PLACE 151

#define TILEHEIGHT 54
#define TILEWIDTH 38
#define TILESIDE 5
#define TILEBOTTOM 4
#define STARTX 12
#define STARTY 22
#define SPECIALY STARTY+TILEHEIGHT/2+3*TILEHEIGHT
#define TOPX STARTX+TILEWIDTH/2+6*TILEWIDTH-(3*TILESIDE+2)
#define RIGHT1X 1+STARTX+13*TILEWIDTH
#define RIGHT2X 1+STARTX+14*TILEWIDTH
#define TOPY SPECIALY-3*TILEBOTTOM
#define TILEX(x,z) STARTX+x*(TILEWIDTH/2)-(z*TILESIDE)
#define TILEY(y,z) STARTY+y*(TILEHEIGHT/2)-(z*TILEBOTTOM)

#define SEL_NONE 0
#define SEL_TILE 5

#define GAME_PLAY 0
#define GAME_PAUSED 1
#define GAME_SHUFFLE 2
#define GAME_AUTOSHUFFLE 3
#define GAME_FINISHED 4
#define GAME_NOMORE 5
#define GAME_BOARDINIT 6

static u8 *tex_tiles, *tex_numbers, *tex_clock, *tex_matches, *tex_pause, *tex_pauseover, *tex_paused, *tex_finished;
static u8 *tex_hint, *tex_hintover, *tex_playerone, *tex_playertwo, *tex_winnerone, *tex_winnertwo, *tex_draw, *tex_nomorematches;
static u8 *tex_undo, *tex_undoover;
u8* tex_gameback;

static bool selectable[MAX_TILES];

// grid to know where you can place tiles
static unsigned char grid[MAX_WIDTH][MAX_HEIGHT][MAX_LAYERS];

static unsigned char bestGuess[MAX_TILES];

// array of tiles
static unsigned char tiles[MAX_TILES];

static int game_state=GAME_PLAY;
static int tot=0,dif=2,score[2],shuffleretries,totals[2]={0,0};

static u8 *mtl=(u8 *)default_mtl;

static time_t startTime,endTime,pauseTime;
static unsigned char tilesLeft,tilesToPlace,bestGuessNum=MAX_TILES;

typedef struct selection {
	char type;
	unsigned char tile;
	int x,y,z;
} selection;

static selection undoTiles[2];

static selection sel[6] = {{SEL_NONE,BLANK,-1,-1,-1},{SEL_NONE,BLANK,-1,-1,-1},
					{SEL_NONE,BLANK,-1,-1,-1},{SEL_NONE,BLANK,-1,-1,-1},
					{SEL_NONE,BLANK,-1,-1,-1},{SEL_NONE,BLANK,-1,-1,-1}};

static const u32 playercol[3] = {0x00001111,0x00110011,0x00000011};

static const unsigned char numwidths[] = {10,15,16,17,15,16,16,15,16,16,10,11,6};

typedef struct fadestruct {
	u8 alpha;
	f32 scale;
	f32 x,y;
	unsigned char tile;
	bool fading;
} fadestruct;

static fadestruct fade[4] = {{0,0,0,0,0,false},{0,0,0,0,0,false},{0,0,0,0,0,false},{0,0,0,0,0,false}};

static bool pauseover=false,menuover=false,shuffleover=false,hintover=false;
static bool restartover=false,newgameover=false,undoover=false, randomgameover = false;
static int gamemode,matches;
static int imgx=0,imgacc=0;

static unsigned char openTiles[36];

static int pausepos[4] = {578,408,56,44};
static int hintpos[4] = {578,353,56,44};
static const int undopos[4] = {578,298,56,44};
static int menupos[4] = {499,398,108,56};
static int shufflepos[4] = {463,328,168,48};
static int restartpos[4] = {40,398,168,48};
static int newgamepos[4] = {40,398,168,48};
static int randomgamepos[4] = {40, 348, 168, 48};

static int multi[2] = {0,0};

void initGame(int gm) {
	gamemode=gm;

	setLayout(opt_layout);

	// load the textures from the .h tilesets
	switch(opt_tileset) {
		case DEFAULT :
			tex_gameback=GRRLIB_LoadJPG(gameback_jpg, gameback_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_default_png);
			break;
		case SIMPLE :
			tex_gameback=GRRLIB_LoadJPG(gameback_jpg, gameback_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_simple_png);
			break;
		case OLD :
			tex_gameback=GRRLIB_LoadJPG(gameback_jpg, gameback_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_old_png);
			break;
		case SPOOKY :
			tex_gameback=GRRLIB_LoadJPG(bk_spooky_jpg, bk_spooky_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_spooky_png);
			break;
		case EGYPTIAN :
			tex_gameback=GRRLIB_LoadJPG(bk_egypt_jpg, bk_egypt_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_egypt_png);
			break;
		case SPACE :
			tex_gameback=GRRLIB_LoadJPG(bk_space_jpg, bk_space_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_space_png);
			break;
		default :
			tex_gameback=GRRLIB_LoadJPG(gameback_jpg, gameback_jpg_size);
			tex_tiles=GRRLIB_LoadTexture(ts_default_png);
			break;
	}

	tex_clock=GRRLIB_LoadTexture(clock_png);
	tex_matches=GRRLIB_LoadTexture(matches_png);
	tex_playerone=GRRLIB_LoadTexture(playerone_png);
	tex_playertwo=GRRLIB_LoadTexture(playertwo_png);
	tex_hint=GRRLIB_LoadTexture(hint_png);
	tex_hintover=GRRLIB_LoadTexture(hintover_png);
	tex_pause=GRRLIB_LoadTexture(pause_png);
	tex_paused=GRRLIB_LoadTexture(paused_png);
	tex_pauseover=GRRLIB_LoadTexture(pauseover_png);
	tex_undo=GRRLIB_LoadTexture(undo_png);
	tex_undoover=GRRLIB_LoadTexture(undoover_png);
	tex_finished=GRRLIB_LoadTexture(finished_png);
	tex_nomorematches=GRRLIB_LoadTexture(nomorematches_png);
	tex_winnerone=GRRLIB_LoadTexture(winnerone_png);
	tex_winnertwo=GRRLIB_LoadTexture(winnertwo_png);
	tex_draw=GRRLIB_LoadTexture(draw_png);

	tex_numbers=GRRLIB_LoadTexture(gamenumbers_png);
	GRRLIB_SetFont(tex_numbers,20, 24, "1234567890:x-", 13, 1, numwidths, 0);

	int strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_MENU]);
	menupos[0]=605-strsize;
	menupos[2]=strsize;

	strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_SHUFFLE]);
	shufflepos[0]=605-strsize;
	shufflepos[2]=strsize;

	restartpos[2]=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_RESTART]);
	newgamepos[2]=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_NEW]);
	randomgamepos[2]=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_RANDOMLAYOUT]);

	setupGame();
}

void setLayout(int layoutNum) {
	mtl = (u8 *)layouts[layoutNum];
}

void initGrid() {
	int l,x,y,z,pos=0;

	// clear the grid
	for(z=0;z<MAX_LAYERS;z++)
		for(x=0;x<MAX_WIDTH;x++)
			for(y=0;y<MAX_HEIGHT;y++)
				grid[x][y][z]=BLANK;

	// put placeholders on the grid for the tiles
	for(l=0;l<MAX_TILES;++l) {
		x=mtl[pos++];
		y=mtl[pos++];
		z=mtl[pos++];
		grid[x][y][z]=PLACE;
	}
}

void resetToPlaces() {
	int l,x,y,z;
	tilesToPlace=0;
	for(l=0;l<MAX_TILES;l++) {
		x=mtl[l*3];
		y=mtl[l*3+1];
		z=mtl[l*3+2];
		if(grid[x][y][z]!=BLANK) {
			grid[x][y][z]=PLACE;
			tilesToPlace++;
		}
	}
}

void checkSelectable() {
	int x,y,z;
	for(x=0;x<MAX_TILES;++x) selectable[x]=false;

	for(z=MAX_LAYERS-1;z>=0;--z) {
		for(y=0;y<MAX_HEIGHT;++y) {
			for(x=0;x<MAX_WIDTH;++x) {
				if(grid[x][y][z]!=BLANK) {
					selectable[grid[x][y][z]]=isSelectable(x,y,z);
				}
			}
		}
	}
}

bool isSelectable(int x, int y, int z) {
	// first check if there are any overlapping from the row above
	if(z<(MAX_LAYERS-1)) {
		if(isHigher(x,y,z+1)) {
			return false;
		}
	}

	int tot;

	// check left hand side
	if(x-2>=0) {
		tot=0;
		if(y-1>=0) {
			tot+=grid[x-2][y-1][z]==BLANK?0:1;
		}
		tot+=grid[x-2][y][z]==BLANK?0:1;
		tot+=grid[x-2][y+1][z]==BLANK?0:1;
		if(tot==0) return true;
	}
	else {
		return true;
	}

	// check right hand side
	if(x+2<MAX_WIDTH) {
		tot=0;
		if(y-1>=0) {
			tot+=grid[x+2][y-1][z]==BLANK?0:1;
		}
		tot+=grid[x+2][y][z]==BLANK?0:1;
		tot+=grid[x+2][y+1][z]==BLANK?0:1;
		if(tot==0) return true;
	}
	else {
		return true;
	}

	return false;
}

bool isHigher(char x,char y,char z) {
	int r,c,tot=0;
	int x1=x-1,x2=x+2,y1=y-1,y2=y+2;
	if(x1<0) x1=0;
	if(x2>=MAX_WIDTH) x2=MAX_WIDTH-1;
	if(y1<0) y1=0;
	if(y2>=MAX_HEIGHT) y2=MAX_HEIGHT-1;
	for(r=x1;r<x2;r++)
		for(c=y1;c<y2;c++)
			tot+=grid[r][c][(int)z]==BLANK?0:1;
	return tot>0;
}

void killGame() {
	if(tex_numbers) free(tex_numbers);

	if(tex_draw) free(tex_draw);
	if(tex_winnertwo) free(tex_winnertwo);
	if(tex_winnerone) free(tex_winnerone);
	if(tex_nomorematches) free(tex_nomorematches);
	if(tex_finished) free(tex_finished);
	if(tex_pauseover) free(tex_pauseover);
	if(tex_paused) free(tex_paused);
	if(tex_pause) free(tex_pause);
	if(tex_hintover) free(tex_hintover);
	if(tex_hint) free(tex_hint);
	if(tex_undoover) free(tex_undoover);
	if(tex_undo) free(tex_undo);
	if(tex_playertwo) free(tex_playertwo);
	if(tex_playerone) free(tex_playerone);
	if(tex_matches) free(tex_matches);
	if(tex_clock) free(tex_clock);
	if(tex_tiles) free(tex_tiles);

	if(tex_gameback) free(tex_gameback);
}

void setupGame() {
	srand(time(NULL));

	// This sets up the array of tiles so that they correspond to the number in the graphical tile set
	int z=0,x,y;
	for(x=0;x<34;x++) {
		for(y=0;y<4;y++) {
			tiles[z++]=x;
		}
	}

	for(x=34;x<42;x++) {
		tiles[z++]=x;
	}

	tot=0;
	dif=2;

	score[0]=0;
	score[1]=0;

	undoTiles[0].x = -1; // disable undo from start

	for(x=0;x<4;x++) {
		clearSelected(x);
		fade[x].fading=false;
	}

	clearSelected(4);
	clearSelected(5);

	multi[0]=0;
	multi[1]=0;

	shuffleretries=0;

	initGrid();

	tilesLeft=tilesToPlace=MAX_TILES;

	mixPairs(tilesLeft*6);

	shuffleretries=0;
	bestGuessNum=MAX_TILES;

	while(tilesToPlace>0) {
		placeTilePair();
	}

	checkSelectable();
	goesLeft();

	startTime = time(NULL);

	game_state=GAME_PLAY;

	SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 11025, 0,&gong_raw, gong_raw_size, opt_sound, 0, NULL);
	SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 11025, 40,&gong_raw, gong_raw_size, 0, opt_sound, NULL);
}

void mixPairs(int shuffles) {
	int x,t1,t2;
	unsigned char dummy;
	for(x=0;x<shuffles;x++) {
		t1=((rand()%tilesLeft)/2)*2;
		while(tiles[t1]==BLANK) {
			t1=((rand()%tilesLeft)/2)*2;
		}

		t2=((rand()%tilesLeft)/2)*2;
		while(tiles[t2]==BLANK || t1==t2) {
			t2=((rand()%tilesLeft)/2)*2;
		}

		dummy=tiles[t1];
		tiles[t1]=tiles[t2];
		tiles[t2]=dummy;

		dummy=tiles[t1+1];
		tiles[t1+1]=tiles[t2+1];
		tiles[t2+1]=dummy;
	}
}

void drawGame(){
	static int tot=0,dif=1;
	int strlen,x;

	GRRLIB_DrawImg(0, 0, 640, 480, tex_gameback, 0, 1, 1, 255);

	drawBoard();

	if(gamemode==TWO_PLAYER_VERSUS) {
		drawScores();
	}
	else {
		drawTime();
		drawMatches();
		drawTilesLeft();
	}

	for(x=0;x<4;x++) {
		if(fade[x].fading) {
			drawRichTile(fade[x].x,fade[x].y,fade[x].tile,fade[x].scale,fade[x].alpha);
			if(fade[x].alpha-8<0) fade[x].fading=false;
			if(x%2==1 && multi[x/2]>1) GRRLIB_GPrintf((fade[x].x+fade[x-1].x)/2,(fade[x].y+fade[x-1].y)/2,0x00FFFFFF-(0x01000000*fade[x].alpha),fade[x].scale,fade[x].scale,ALIGN_CENTRE,2,"X%d",multi[x/2]);
			fade[x].alpha-=8;
			fade[x].scale+=0.2;
			if(fade[x].alpha==191) {
				if(opt_rumble) WPAD_Rumble(x/2==0?WPAD_CHAN_0:WPAD_CHAN_1, 0);
			}
		}
	}

	switch(game_state) {
		case GAME_PLAY :
			drawPauseIcon();
			if(gamemode!=TWO_PLAYER_VERSUS) {
				drawHintIcon();
				drawUndoIcon();
			}

			break;
		case GAME_PAUSED :
			GRRLIB_FillScreen(0xAA000000);
			GRRLIB_DrawImg(198,134,228,196,tex_paused, 0, 1, 1, 255);
			strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[LNG_GAME_PAUSED]);
			if(strlen>192) {
				f32 zoom = 192.0/(float)strlen;;
				GRRLIB_GPrintf(198+26,274,0xFFEEEEBB,zoom,1, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_GAME_PAUSED]);
			}
			else {
				GRRLIB_GPrintf(198+22+200/2,274,0xFFEEEEBB,1,1, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_PAUSED]);
			}

			GRRLIB_GPrintf(restartpos[0], restartpos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(restartover),curtext[LNG_GAME_RESTART]);

			drawPauseIcon();
			break;
		case GAME_SHUFFLE :
			if(tot++==dif) {
				dif+=10;
				if(tot>90 && tilesToPlace<=0) {
					game_state=GAME_PLAY;
					checkSelectable();
					goesLeft();
					dif=2;
					tot=0;
					for(x=0;x<4;x++) {
						clearSelected(x);
					}
				}
			}

			if(tilesToPlace>0)
				placeTilePair();

			if(tot*2.5<=255) {
				GRRLIB_GPrintf(320, 220,0xFFFFFFFF- (0x01000000*(tot*2.5)),1+((float)tot)/16,1+((float)tot)/32, ALIGN_CENTRE,CUR_FONT(true),curtext[LNG_GAME_SHUFFLE]);
			}

			break;
		case GAME_FINISHED :
			GRRLIB_FillScreen(0xAA000000);

			if(gamemode==TWO_PLAYER_VERSUS) {
				if(score[0]>score[1]) {
					GRRLIB_DrawImg(185+imgx,103,260,220,tex_winnerone, 0, 1, 1, 255);
					GRRLIB_GPrintf(181+imgx,232,0xFF9E9E6B,0.6,0.68, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_GAME_WINNER]);
					GRRLIB_GPrintf(188+6+119+imgx,270,0xFFC02600,1,1, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_PLAYER1]);
				}
				else
				if(score[1]>score[0]) {
					GRRLIB_DrawImg(185+imgx,103,260,220,tex_winnertwo, 0, 1, 1, 255);
					GRRLIB_GPrintf(181+imgx,232,0xFF9E9E6B,0.6,0.68, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_GAME_WINNER]);
					GRRLIB_GPrintf(188+6+119+imgx,270,0xFF8DC033,1,1, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_PLAYER2]);
				}
				else {
					GRRLIB_DrawImg(150+imgx,99,344,224,tex_draw, 0, 1, 1, 255);
					GRRLIB_GPrintf(188+4+119+imgx,260,0xFFA67319,1,1, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_DRAW]);
				}

				GRRLIB_DrawImg(205, 330, 60, 44, tex_playerone, 0, 1, 1, 255- ((float)imgx*0.75));
				GRRLIB_DrawImg(365, 330, 60, 44, tex_playertwo, 0, 1, 1, 255- ((float)imgx*0.75));
				GRRLIB_GPrintf(315, 340,0xFFFFFFFF - ((float)imgx*0.75)*0x01000000,1,1,ALIGN_CENTRE,0,"%02d - %02d",totals[0],totals[1]);
			}
			else {
				GRRLIB_DrawImg(186+imgx,151,252,172,tex_finished, 0, 1, 1, 255);

				strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[LNG_GAME_FINISHED]);
				if(strlen>238) {
					f32 zoom = (float)238/(float)strlen;
					GRRLIB_GPrintf(185+8+imgx,255,0xFFEEEEBB,zoom,1, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_GAME_FINISHED]);
				}
				else {
					GRRLIB_GPrintf(188+8+119+imgx,255,0xFFEEEEBB,1,1, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_FINISHED]);
				}
			}

			if(imgx>0 && imgacc>0) {
				imgx-=imgacc;
				imgacc--;
			}

			GRRLIB_GPrintf(newgamepos[0], newgamepos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(newgameover),curtext[LNG_GAME_RESTART]);
			GRRLIB_GPrintf(randomgamepos[0], randomgamepos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(randomgameover),curtext[LNG_GAME_RANDOMLAYOUT]);

			GRRLIB_GPrintf(menupos[0], menupos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(menuover),curtext[LNG_GAME_MENU]);
			break;
		case GAME_NOMORE :
			GRRLIB_FillScreen(0xAA000000);
			GRRLIB_DrawImg(170+imgx,149,284,160,tex_nomorematches, 0, 1, 1, 255);

			strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[LNG_GAME_NOMORETILES]);
			if(strlen>238) {
				f32 zoom = (float)238/(float)strlen;
				GRRLIB_GPrintf(170+32+imgx,250,0xFFEEEEBB,zoom,0.8, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_GAME_NOMORETILES]);
			}
			else {
				GRRLIB_GPrintf(170+36+119+imgx,250,0xFFEEEEBB,1,0.8, ALIGN_CENTRE,CUR_FONT(false),curtext[LNG_GAME_NOMORETILES]);
			}

			if(imgx>0 && imgacc>0) {
				imgx-=imgacc;
				imgacc--;
			}

			GRRLIB_GPrintf(restartpos[0], restartpos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(restartover),curtext[LNG_GAME_RESTART]);

			GRRLIB_GPrintf(menupos[0], menupos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(menuover),curtext[LNG_GAME_MENU]);
			GRRLIB_GPrintf(shufflepos[0], shufflepos[1],0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(shuffleover),curtext[LNG_GAME_SHUFFLE]);
			break;
	}
}

void placeTilePair() {
	int tile1 = getTile();
	int retries=0;
	while(!checkTile(tile1) && retries++ < tilesLeft*8) {
		tile1 = getTile();
	}
	if(retries>=tilesLeft*8) {
		shuffleretries++;
		if(shuffleretries>7) {
			// we have tried to create a new board serveral times this probably means that there are no more goes
			// so lets bring place all the remaining tiles randomly
			placeRemainingtiles();
			checkSelectable();
			if(!goesLeft()) {
				if(shuffleretries>10) {
					// assume that we cannot place the remaining tiles
					finishGame();
					return;
				}
				resetToPlaces();
			}
		}
		else {
			checkAndSaveGrid();
			resetToPlaces();
		}
		return;
	}

	// place tile 1 in the grid
	grid[mtl[tile1*3]][mtl[tile1*3+1]][mtl[tile1*3+2]]=--tilesToPlace;

	// find spare tile 2
	int tile2 = getTile();
	retries=0;
	while((!checkTile(tile2) || tile1==tile2 || !checkWithoutTile1(tile1,tile2)) && retries++ < tilesLeft*8) {
		tile2 = getTile();
	}
	if(retries>=tilesLeft*8) {
		shuffleretries++;
		if(shuffleretries>7) {
			// we have tried to create a new board serveral times this probably means that there are no more goes
			// so lets bring place all the remaining tiles randomly
			placeRemainingtiles();
			checkSelectable();
			if(!goesLeft()) {
				if(shuffleretries>10) {
					// assume that we cannot place the remaining tiles
					finishGame();
					return;
				}
				resetToPlaces();
			}
		}
		else {
			checkAndSaveGrid();
			resetToPlaces();
		}
		return;
	}

	grid[mtl[tile2*3]][mtl[tile2*3+1]][mtl[tile2*3+2]]=--tilesToPlace;
}

// this function checks whether this is the best attempt to place the tiles,
// if it is then save the grid in case we cannot competely place the tiles at all
void checkAndSaveGrid() {
	if(tilesToPlace>=bestGuessNum) return;

	bestGuessNum=tilesToPlace;

	int x;

	for(x=0;x<MAX_TILES;x++) {
		bestGuess[x]=grid[mtl[x*3]][mtl[x*3+1]][mtl[x*3+2]];
	}
}

// this function is used when we have placed the first tile of a pair and need to check
// whether tile2 could still be placed if tile1 was not there to make sure that we are
// not placing two next to each other that are not accessible
bool checkWithoutTile1(int tile1, int tile2) {
	grid[mtl[tile1*3]][mtl[tile1*3+1]][mtl[tile1*3+2]]=PLACE;

	bool res = checkTile(tile2);

	grid[mtl[tile1*3]][mtl[tile1*3+1]][mtl[tile1*3+2]]=tilesToPlace;

	return res;
}

// This function is used to place all remaining tiles randomly to complete a uncompletable shuffle
// this can be resolved later when there are less tiles left
void placeRemainingtiles() {
	int x,xe;

	// set grid up with the best guess so far
	tilesToPlace=MAX_TILES;
	for(x=0;x<MAX_TILES;x++) {
		grid[mtl[x*3]][mtl[x*3+1]][mtl[x*3+2]]=bestGuess[x];
		if(bestGuess[x]!=PLACE) tilesToPlace--;
	}

	xe=tilesToPlace;

	for(x=0;x<xe;x++) {
		int tile = getTile();
		grid[mtl[tile*3]][mtl[tile*3+1]][mtl[tile*3+2]]=--tilesToPlace;
	}
}

int getTile() {
	int tile=rand()%MAX_TILES;
	int x=mtl[tile*3];
	int y=mtl[tile*3+1];
	int z=mtl[tile*3+2];

	// get tiles until we find one that we can place
	while(grid[x][y][z]!=PLACE) {
		tile=rand()%MAX_TILES;
		x=mtl[tile*3];
		y=mtl[tile*3+1];
		z=mtl[tile*3+2];
	}
	return tile;
}

// This checks whether this tile can be placed
bool checkTile(int tile) {
	int x=mtl[tile*3];
	int y=mtl[tile*3+1];
	int z=mtl[tile*3+2];

	//if z>0 check that tiles below are already placed
	if(z>0 && !isCovered(x,y,z)) {
		return false;
	}

	// check that we have tiles immediately to the left
	if(x>1) {
		if(grid[x-2][y][z]<BLANK) return true;
		if(y==0 && grid[x-2][y+1][z]<BLANK) return true;

		if(y>0) {
			if(grid[x-2][y-1][z]<BLANK && grid[x-2][y+1][z]<BLANK) return true;
			if(grid[x-2][y-1][z]<BLANK && grid[x-2][y+1][z]==BLANK) return true;
			if(grid[x-2][y-1][z]==BLANK && grid[x-2][y+1][z]<BLANK) return true;
		}
	}

	// check that we have tiles immediately to the right
	if(x<MAX_WIDTH-2) {
		if(grid[x+2][y][z]<BLANK) return true;
		if(y==0 && grid[x+2][y+1][z]<BLANK) return true;

		if(y>0) {
			if(grid[x+2][y-1][z]<BLANK && grid[x+2][y+1][z]<BLANK) return true;
			if(grid[x+2][y-1][z]<BLANK && grid[x+2][y+1][z]==BLANK) return true;
			if(grid[x+2][y-1][z]==BLANK && grid[x+2][y+1][z]<BLANK) return true;
		}
	}

	// if any other placed tiles on the same row return false
	//
	// tilebreak signifies that there is a break of tiles on the current row
	// tileplace indicates that there are two unplaced tiles on the current row
	// this works out that

	int col;

	// check left of the selected
	bool tilebreak=false,tileplace=false;
	int blank,yy;

	if(x==0) {
		tilebreak=true;
	}
	else {
		// copy y and use the copy as we may move up and down
		yy=y;

		for(col=x-2;col>=0 && tilebreak==false && tileplace==false;col-=2) {
			// check for tiles already placed on this row
			if(grid[col][yy][z]<BLANK || grid[col][yy+1][z]<BLANK) return false;
			if(yy>0 && grid[col][yy-1][z]<BLANK) return false;

			// check whether this is a tilebreak
			blank=3;
			if(grid[col][yy][z]==BLANK) blank--;
			if((yy>0 && grid[col][yy-1][z]==BLANK) || yy==0) blank--;
			if((yy<MAX_HEIGHT-2 && grid[col][yy+1][z]==BLANK) || yy>=MAX_HEIGHT-2) blank--;
			if(blank==0) tilebreak=true;

			// see if line moves up or down or finishes
			if(grid[col][yy][z]==PLACE) {
				// do nothing
			}
			else {
				if(yy>0) {
					if(grid[col][yy-1][z]==PLACE && grid[col][yy+1][z]==PLACE) tileplace=true;
					else
					if(grid[col][yy-1][z]==PLACE && grid[col][yy+1][z]==BLANK) yy--;
					else
					if(grid[col][yy-1][z]==BLANK && grid[col][yy+1][z]==PLACE) yy++;
				}
				else {
					// curently on top row
					if(grid[col][yy-1][z]==BLANK && grid[col][yy+1][z]==PLACE) yy++;
				}
			}
		}

		if(!tileplace && !tilebreak) tilebreak=true;
	}

	// check right of the selected
	if(x>=MAX_WIDTH-2) {
		if(tileplace) return false;
		if(tilebreak) return true;
	}
	else {
		// copy y and use the copy as we may move up and down
		yy=y;

		for(col=x+2;col<MAX_WIDTH-2;col+=2) {
			// check for tiles already placed on this row
			if(grid[col][yy][z]<BLANK || grid[col][yy+1][z]<BLANK) return false;
			if(yy>0 && grid[col][yy-1][z]<BLANK) return false;

			// check whether this is a tilebreak
			blank=3;
			if(grid[col][yy][z]==BLANK) blank--;
			if((yy>0 && grid[col][yy-1][z]==BLANK) || yy==0) blank--;
			if((yy<MAX_HEIGHT-2 && grid[col][yy+1][z]==BLANK) || yy>=MAX_HEIGHT-2) blank--;
			if(blank==0) {
				if(tileplace) return false;
				if(tilebreak) return true;
			}

			// see if line moves up or down or finishes
			if(grid[col][yy][z]==PLACE) {
				// do nothing
			}
			else {
				if(yy>0) {
					if(grid[col][yy-1][z]==PLACE && grid[col][yy+1][z]==PLACE) {
						if(tileplace) return true;
						if(tilebreak) return false;
					}
					else
					if(grid[col][yy-1][z]==PLACE && grid[col][yy+1][z]==BLANK) yy--;
					else
					if(grid[col][yy-1][z]==BLANK && grid[col][yy+1][z]==PLACE) yy++;
				}
				else {
					// curently on top row
					if(grid[col][yy-1][z]==BLANK && grid[col][yy+1][z]==PLACE) yy++;
				}
			}
		}

		if(tileplace) return false;
		if(tilebreak) return true;
	}

	return true;
}

// This checks out whether there the tile has a sound fondation from th elayer below
bool isCovered(int x,int y,int z) {
	// check directly below
	if(grid[x][y][z-1]<BLANK) return true;

	// check if above and below
	if(y>0 && y<MAX_HEIGHT-1 && grid[x][y-1][z-1]<BLANK && grid[x][y+1][z-1]<BLANK) {
		return true;
	}

	// check if to the left and right
	if(x>0 && x<MAX_WIDTH-1 && grid[x-1][y][z-1]<BLANK && grid[x+1][y][z-1]<BLANK) {
		return true;
	}

	// if we have one above and two diagonally below
	if(x>0 && x<MAX_WIDTH-1 && grid[x][y-1][z-1]<BLANK && grid[x-1][y+1][z-1]<BLANK && grid[x+1][y+1][z-1]<BLANK) {
		return true;
	}

	// if we have one below and two diagonally above
	if(x>0 && x<MAX_WIDTH-1 && grid[x][y+1][z-1]<BLANK && grid[x-1][y-1][z-1]<BLANK && grid[x+1][y-1][z-1]<BLANK) {
		return true;
	}

	// check if four tiles cover this one
	if(y>0 && y<MAX_HEIGHT-1 && x>0 && x<MAX_WIDTH-1) {
		if(grid[x-1][y-1][z-1]<BLANK && grid[x-1][y+1][z-1]<BLANK && grid[x+1][y-1][z-1]<BLANK && grid[x+1][y+1][z-1]<BLANK) {
			return true;
		}
	}

	return false;
}

void drawBoard() {
	int x,y,z,c,l;
	static int colind=0,dir=1;
	bool selected=false,colchange=false;

	for(z=0;z<MAX_LAYERS;z++) {
		for(x=0;x<MAX_WIDTH+MAX_HEIGHT;x++) {
			for(y=(x>=MAX_WIDTH?x-MAX_WIDTH:0),c=(x>=MAX_WIDTH?MAX_WIDTH-1:x);y<MAX_HEIGHT && c>=0;y++,c--) {
				if(grid[c][y][z]!=BLANK && grid[c][y][z]!=PLACE) {
					if(game_state==GAME_PAUSED) {
						GRRLIB_DrawGTile(TILEX(c,z),TILEY(y,z),44,60,tex_tiles,21,2,0,1,1,0x44111111,tiles[grid[c][y][z]]);
					}
					else {
						selected=false;
						for(l=0;l<6;l++) {
							if(l==1 || l==3) {
								// do nothing as this shuold never happen
							}
							else
							if(sel[l].type==SEL_TILE && c==sel[l].x && y==sel[l].y && z==sel[l].z) {
								selected=colchange=true;
								GRRLIB_DrawGTile(TILEX(c,z),TILEY(y,z),44,60,tex_tiles,21,2,0,1,1,0xFFFFFFFF - playercol[l/2]*((colind/3)%16),tiles[grid[c][y][z]]);
							}
						}
						if(!selected) {
							GRRLIB_DrawGTile(TILEX(c,z),TILEY(y,z),44,60,tex_tiles,21,2,0,1,1,selectable[grid[c][y][z]]?0xFFFFFFFF:0xFFEEEEEE,tiles[grid[c][y][z]]);
						}
					}
				}
			}
		}
	}
	if(colchange) {
		colind+=dir;
		if(colind==0 || colind==23) {
			dir=-dir;
		}
	}
}

void drawTime() {
	GRRLIB_DrawImg(588, 31, 40, 44, tex_clock, 0, 1, 1, 255);
	time_t curTime = time(NULL);
	time_t difTime;
	if(game_state==GAME_PAUSED)
		difTime= pauseTime - startTime;
	else
		difTime= curTime - startTime;
	char str[6];
	strftime(str,15,"%M:%S",localtime(tilesLeft==0?&endTime:&difTime));

	GRRLIB_GPrintf(602,72,0xFFFFFFFF,1,1,ALIGN_CENTRE,0,str);
}

void drawMatches() {
	GRRLIB_DrawImg( 578, 103, 60, 44, tex_matches, 0, 1, 1, 255);
	GRRLIB_GPrintf( 608,146,0xFFFFFFFF,1,1,ALIGN_CENTRE,0,"%02d",matches);
}

void drawTilesLeft() {
    GRRLIB_GPrintf( 608, 176, 0xFFFFFFFF, 1, 1, ALIGN_CENTRE, 0, "%03d", tilesLeft);
}

void drawScores() {
	GRRLIB_DrawImg(577, 30, 60, 44, tex_playerone, 0, 1, 1, 255);
	GRRLIB_GPrintf(609,74,0xFFFFFFFF,1,1,ALIGN_CENTRE,0,"%05d",score[0]);

	GRRLIB_DrawImg(577, 115, 60, 44, tex_playertwo, 0, 1, 1, 255);
	GRRLIB_GPrintf(609,159,0xFFFFFFFF,1,1,ALIGN_CENTRE,0,"%05d",score[1]);
}

void drawPauseIcon() {
	GRRLIB_DrawImg(pausepos[0], pausepos[1],pausepos[2],pausepos[3],pauseover?tex_pauseover:tex_pause, 0, 1, 1, 255);
}

void drawHintIcon() {
	GRRLIB_DrawImg(hintpos[0], hintpos[1],hintpos[2],hintpos[3],hintover?tex_hintover:tex_hint, 0, 1, 1, 255);
}

void drawUndoIcon() {
	GRRLIB_DrawImg(undopos[0], undopos[1],undopos[2],undopos[3],undoover?tex_undoover:tex_undo, 0, 1, 1, 255);
}

void drawRichTile(f32 xpos, f32 ypos, unsigned char tile,f32 scale,u8 alpha) {
	GRRLIB_DrawGTile(xpos,ypos,44,60,tex_tiles,21,2,0,scale,scale,0x00FFFFFF+ 0x01000000*alpha,tile);
}

// deal with wiimote and return true if return to the menu
bool gameWiimote(WPADData *wd_one, u32 btns_one, WPADData *wd_two, u32 btns_two) {
	if(game_state==GAME_BOARDINIT) return false;

	if(game_state==GAME_FINISHED || game_state==GAME_NOMORE) {
		if((wd_one->ir.x)>menupos[0] && (wd_one->ir.x)<(menupos[0]+menupos[2]) && (wd_one->ir.y)>menupos[1] && (wd_one->ir.y)<(menupos[1]+menupos[3]))
			menuover=true;
		else
			menuover=false;

		if(game_state==GAME_FINISHED )
		{
            if( CONTAINS( wd_one->ir.x, wd_one->ir.y, newgamepos[0], newgamepos[1],newgamepos[2],newgamepos[3]) )
                newgameover=true;
            else
                newgameover=false;

            if( CONTAINS( wd_one->ir.x, wd_one->ir.y, randomgamepos[0], randomgamepos[1], randomgamepos[2], randomgamepos[3]) )
                randomgameover=true;
            else
                randomgameover=false;
		}

		if(game_state==GAME_NOMORE && (wd_one->ir.x)>restartpos[0] && (wd_one->ir.x)<(restartpos[0]+restartpos[2]) && (wd_one->ir.y)>restartpos[1] && (wd_one->ir.y)<(restartpos[1]+restartpos[3]))
			restartover=true;
		else
			restartover=false;

		if(tilesLeft>0) {
			if((wd_one->ir.x)>shufflepos[0] && (wd_one->ir.x)<(shufflepos[0]+shufflepos[2]) && (wd_one->ir.y)>shufflepos[1] && (wd_one->ir.y)<(shufflepos[1]+shufflepos[3])) {
				shuffleover=true;
				if(btns_one & WPAD_BUTTON_A) {
					compressTiles();
					mixPairs(tilesLeft*6);
					resetToPlaces();
					shuffleretries=0;
					bestGuessNum=MAX_TILES;
					game_state=GAME_SHUFFLE;
				}
			}
			else
				shuffleover=false;
		}

		if(btns_one & WPAD_BUTTON_A) {
			// if user selects menu then return true to tell main to kill game and start menu
			if(menuover && btns_one & WPAD_BUTTON_A) {
				return true;
			}

			if(restartover || newgameover || randomgameover) {
			    if( randomgameover)
                {
                    opt_layout = rand()%LAYOUTS;
                    setLayout(opt_layout);
                }
				setupGame();
				restartover=false;
				newgameover=false;
				randomgameover=false;
			}
		}

		return false;
	}

	if(game_state==GAME_SHUFFLE) {
		if (btns_one & WPAD_BUTTON_HOME) {
			return true;
		}
		return false;
	}

	if((wd_one->ir.x)>pausepos[0] && (wd_one->ir.x)<(pausepos[0]+pausepos[2]) && (wd_one->ir.y)>pausepos[1] && (wd_one->ir.y)<(pausepos[1]+pausepos[3]))
		pauseover=true;
	else
		pauseover=false;

	if(gamemode!=TWO_PLAYER_VERSUS) {
		if((wd_one->ir.x)>hintpos[0] && (wd_one->ir.x)<(hintpos[0]+hintpos[2]) && (wd_one->ir.y)>hintpos[1] && (wd_one->ir.y)<(hintpos[1]+hintpos[3]))
			hintover=true;
		else
			hintover=false;
        //undo
		if((wd_one->ir.x)>undopos[0] && (wd_one->ir.x)<(undopos[0]+undopos[2]) && (wd_one->ir.y)>undopos[1] && (wd_one->ir.y)<(undopos[1]+undopos[3]))
			undoover=true;
		else
			undoover=false;
	}

	if(game_state==GAME_PAUSED && (wd_one->ir.x)>restartpos[0] && (wd_one->ir.x)<(restartpos[0]+restartpos[2]) && (wd_one->ir.y)>restartpos[1] && (wd_one->ir.y)<(restartpos[1]+restartpos[3]))
		restartover=true;
	else
		restartover=false;

	if(btns_one & WPAD_BUTTON_PLUS && (game_state==GAME_PAUSED || game_state==GAME_PLAY)) {
		if(game_state==GAME_PAUSED) {
			time_t curTime = time(NULL);
			startTime+=(curTime-pauseTime);
			game_state=GAME_PLAY;
		}
		else {
			pauseTime= time(NULL);
			game_state=GAME_PAUSED;
		}
		return false;
	}

	// button A selects a tile
	if(btns_one & WPAD_BUTTON_A) {

		// if we are over the pause icon then pause or unpause and return
		if(pauseover) {
			if(game_state==GAME_PAUSED) {
				time_t curTime = time(NULL);
				startTime+=(curTime-pauseTime);
				game_state=GAME_PLAY;
			}
			else {
				pauseTime= time(NULL);
				game_state=GAME_PAUSED;
			}
			return false;
		}

		if(game_state==GAME_PAUSED) {
			if(restartover) {
				setupGame();
			}
			return false;
		}

		// if we are over the hint icon then find hint and select it
		if(hintover) {
			findHint();
			return false;
		}

		if(undoover)
		{
		    undo();
		    return false;
		}


		selectProcessing(wd_one, 0);

		return false;
	}

	if(gamemode!=ONE_PLAYER_GAME && btns_two & WPAD_BUTTON_A) {
		selectProcessing(wd_two, 1);
		return false;
	}

	// button B clears the selected tiles
	if(btns_one & WPAD_BUTTON_B) {
		clearSelected(0);
		clearSelected(1);
		return false;
	}

	if(gamemode!=ONE_PLAYER_GAME && btns_two & WPAD_BUTTON_B) {
		clearSelected(2);
		clearSelected(3);
		return false;
	}

	if (btns_one & WPAD_BUTTON_HOME) {
		return true;
	}

	return false;
}

void selectProcessing(WPADData *wd, int player) {
	int selnum = player*2+(sel[player*2+0].type==SEL_NONE?0:1);

	// other represents the first possible selection of the other player
	int other = ((player+1)%2)*2;

	if(!selectTile(wd->ir.x, wd->ir.y,selnum)) {
		// no tile selected so return
		return;
	}

	if(sel[4].type!=SEL_NONE) {
		clearSelected(4);
		clearSelected(5);
	}

	// if we are in coop mode check for a same tile by other player then check for a completion of other players selection
	if(gamemode==TWO_PLAYER_COOP && sel[other].type!=SEL_NONE) {
		if(isSameTile(selnum,other)) {
			clearSelected(selnum);
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
			return;
		}
		if(checkMatch(selnum,other)) {
			removeMatched(player,selnum,other);
			if(selnum%2==1) clearSelected(selnum-1);
			return;
		}
	}

	// if this is the first tile selected check mode specifics then play selection sound
	if(selnum%2==0) {
		// if we are in versus mode check for a steal
		if(gamemode==TWO_PLAYER_VERSUS && sel[other].type!=SEL_NONE && isSameTile(selnum,other)) {
			clearSelected(other);
			if(player==0) {
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&Haha_raw, Haha_raw_size, opt_sound, opt_sound, NULL);
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 600,&nani_raw, nani_raw_size, opt_sound, opt_sound, NULL);
			}
			else {
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&Haha2_raw, Haha2_raw_size, opt_sound, opt_sound, NULL);
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 600,&doshiyo_raw, doshiyo_raw_size, opt_sound, opt_sound, NULL);
			}
		}
		else
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 8000, 0,&Click17a_raw, Click17a_raw_size, opt_sound, opt_sound, NULL);
		return;
	}

	// ok so this is the second selection for this player so lets deal with that

	// check that this player hasn't selected the same piece if they have then deselect it
	if(isSameTile(selnum, selnum-1)) {
		clearSelected(selnum);
		clearSelected(selnum-1);
		SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
		return;
	}

	// if we have a match then remove pieces
	if(checkMatch(selnum,selnum-1)) {
		// if we are in versus mode check for a completion of a piece already selected by the other player
		if(gamemode==TWO_PLAYER_VERSUS && sel[other].type!=SEL_NONE && isSameTile(selnum,other)) {
			clearSelected(other);
		}

		removeMatched(player, selnum, selnum-1);
	}
	else {
		// if no matches at all then best to clear the currently selected one then
		clearSelected(selnum);

		// no match so play the unmatched pair sound
		SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
	}
}

void removeMatched(int player,int s1, int s2) {
	int x;
	if(gamemode==TWO_PLAYER_VERSUS) {
		multi[player]++;
		multi[(player+1)%2]=0;
		score[player]+=calcScore(player);
	}
	storeUndo( s1, s2);
	removeTile(s1);
	removeTile(s2);
	checkSelectable();
	tilesLeft-=2;

	// rumble the pad that has matched two tiles
	if(opt_rumble) WPAD_Rumble(player==0?WPAD_CHAN_0:WPAD_CHAN_1, 1);

// This is used to test the finish sequence
//	tilesLeft=0;
//	score[1]=score[0];

	if(tilesLeft==0) {
		finishGame();
		return;
	}

	if(!goesLeft()) {
		// if there are no goals left and only two tiles then they must be over each other so finish the game
		if(tilesLeft==2) {
			finishGame();
			return;
		}
		// if versus mode then auto shuffle
		if(gamemode==TWO_PLAYER_VERSUS) {
			compressTiles();
			mixPairs(tilesLeft*6);
			resetToPlaces();
			game_state=GAME_SHUFFLE;
		}
		else {
			game_state=GAME_NOMORE;
			imgx=300;
			imgacc=24;
		}
	}

	// play the pair successfully matched sound
	if(gamemode==TWO_PLAYER_VERSUS) {
		switch(player) {
			case 0 :
				switch(multi[player]) {
					case 1 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&oh_raw, oh_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 2 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&ohhh_raw, ohhh_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 3 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&oh_raw, oh_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 300,&oh_raw, oh_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 4 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&ohhh_raw, ohhh_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 400,&ohhh_raw, ohhh_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 5 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&aiee_raw, aiee_raw_size, opt_sound, opt_sound, NULL);
						break;
					default :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 320,&ohhh_raw, ohhh_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&aiee_raw, aiee_raw_size, opt_sound, opt_sound, NULL);
						break;
				}
				break;
			case 1 :
				switch(multi[player]) {
					case 1 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&mm_raw, mm_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 2 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&mmmm_raw, mmmm_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 3 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&mm_raw, mm_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 260,&mm_raw, mm_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 4 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&mmmm_raw, mmmm_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 300,&mmmm_raw, mmmm_raw_size, opt_sound, opt_sound, NULL);
						break;
					case 5 :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&uss_raw, uss_raw_size, opt_sound, opt_sound, NULL);
						break;
					default :
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 280,&mmmm_raw, mmmm_raw_size, opt_sound, opt_sound, NULL);
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&uss_raw, uss_raw_size, opt_sound, opt_sound, NULL);
						break;
				}
				break;
		}
	}
	else {
		for(x=0;x<multi[player]+1 && x<5;x++) {
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, x*40,&ptwiiing_raw, ptwiiing_raw_size, ((x+1)%2)*opt_sound, (x%2)*opt_sound, NULL);
//			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 8000, 20+x*80,&ptwiiing_raw, ptwiiing_raw_size, 0, opt_sound, NULL);
		}
	}

}

void finishGame() {
	endTime=time(NULL)-startTime;
	game_state=GAME_FINISHED;
	imgx=300;
	imgacc=24;
	matches=0;
	if(gamemode==TWO_PLAYER_VERSUS) {
		if(score[0]>score[1]) {
			totals[0]++;
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&a_yokatta_raw, a_yokatta_raw_size, opt_sound, opt_sound, NULL);
		}
		else
		if(score[1]>score[0]) {
			totals[1]++;
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&yatta_raw, yatta_raw_size, opt_sound, opt_sound, NULL);
		}
	}
	else {
		int x;
		for(x=0;x<8;x++) {
			SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, x*148,&ptwiiing_raw, ptwiiing_raw_size, ((x+1)%2)*opt_sound, (x%2)*opt_sound, NULL);
		}

	}
}

bool isSameTile(int s1, int s2) {
	return sel[s1].x==sel[s2].x && sel[s1].y==sel[s2].y && sel[s1].z==sel[s2].z;
}

bool selectTile(f32 x, f32 y,int selnum) {
	int tx,ty,z;
	for(z=MAX_LAYERS-1;z>=0;z--) {
		tx = (x-STARTX +(z*(TILESIDE))) / (TILEWIDTH/2);
		ty = (y-STARTY +(z*(TILEBOTTOM))) / (TILEHEIGHT/2);

		if(tx>=0 && tx<MAX_WIDTH && ty>=0 && ty<MAX_HEIGHT) {
			if(selectExactTile(tx-1,ty-1,z,selnum)) {
				return true;
			}
			if(selectExactTile(tx-1,ty,z,selnum)) {
				return true;
			}
			if(selectExactTile(tx,ty-1,z,selnum)) {
				return true;
			}
			if(selectExactTile(tx,ty,z,selnum)) {
				return true;
			}
		}
	}
	return false;
}

bool selectExactTile(int x, int y, int z, int selnum) {
	if(x>=0 && y>=0 && grid[x][y][z]!=BLANK && selectable[grid[x][y][z]]) {
		sel[selnum].type = SEL_TILE;
		sel[selnum].tile=tiles[grid[x][y][z]];
		sel[selnum].x=x;
		sel[selnum].y=y;
		sel[selnum].z=z;
		return true;
	}
	return false;
}

void storeUndo( int sel1, int sel2)
{
    undoTiles[0].x = sel[sel1].x;
    undoTiles[0].y = sel[sel1].y;
    undoTiles[0].z = sel[sel1].z;
    undoTiles[0].tile = grid[sel[sel1].x][sel[sel1].y][sel[sel1].z];
    undoTiles[0].type = tiles[undoTiles[0].tile];
    undoTiles[1].x = sel[sel2].x;
    undoTiles[1].y = sel[sel2].y;
    undoTiles[1].z = sel[sel2].z;
    undoTiles[1].tile = grid[sel[sel2].x][sel[sel2].y][sel[sel2].z];
    undoTiles[1].type = tiles[undoTiles[1].tile];
}

void undo()
{
    if( undoTiles[0].x > 0)
    {
        int i;
        for( i = 0; i < 6; ++i) clearSelected(i);
        grid[undoTiles[0].x][undoTiles[0].y][undoTiles[0].z] = undoTiles[0].tile;
        tiles[undoTiles[0].tile] = undoTiles[0].type;

        grid[undoTiles[1].x][undoTiles[1].y][undoTiles[1].z] = undoTiles[1].tile;
        tiles[undoTiles[1].tile] = undoTiles[1].type;

        checkSelectable();
        goesLeft();
        tilesLeft += 2;
    }
    undoTiles[0].x = -1; //disable until next remove of a tile
}

void removeTile(int selnum) {

	tiles[grid[sel[selnum].x][sel[selnum].y][sel[selnum].z]]=BLANK;
	selectable[grid[sel[selnum].x][sel[selnum].y][sel[selnum].z]]=false;
	grid[sel[selnum].x][sel[selnum].y][sel[selnum].z]=BLANK;
	setupFade(selnum);
	clearSelected(selnum);
}

void clearSelected(selnum) {
	sel[selnum].type=SEL_NONE;
	sel[selnum].tile=BLANK;
}

bool checkMatch(int selnum1, int selnum2) {
	if(realTileNum(sel[selnum1].tile)==realTileNum(sel[selnum2].tile))
		return true;

	return false;
}

int calcScore(int player) {
	int tile = realTileNum(sel[player*2].tile);

	// normal tiles only five points
	if(tile<27) return 5*multi[player];

	// Directions 10 points
	if(tile<31) return 10*multi[player];

	// Dragons twenty points
	if(tile<34) return 20*multi[player];

	// Seasons and flowers 15 points
	return 10*multi[player];
}

bool goesLeft() {
	int x;
	for(x=0;x<36;x++) {
		openTiles[x]=0;
	}

	matches=0;

	for(x=0;x<MAX_TILES;x++) {
		if(selectable[x]) {
			openTiles[(int) realTileNum(tiles[x])]++;
			if(openTiles[(int) realTileNum(tiles[x])]>1) matches++;
		}
	}

	if(matches>0) return true;

	endTime=time(NULL)-startTime;
	return false;
}

void findHint() {
	int tn,x,y,z;

	if(!goesLeft()) {
		SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 22050, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
		return;
	}

	tn=rand()%36;
	while(openTiles[tn]<2) {
		tn=rand()%36;
	}

	int s=4;

	for(z=MAX_LAYERS-1;z>=0;z--) {
		for(y=0;y<MAX_HEIGHT;y++) {
			for(x=0;x<MAX_WIDTH;x++) {
				if(realTileNum(tiles[grid[x][y][z]])==tn && selectable[grid[x][y][z]]) {
					sel[s].type = SEL_TILE;
					sel[s].tile=tiles[grid[x][y][z]];
					sel[s].x=x;
					sel[s].y=y;
					sel[s].z=z;
					s++;
					if(s==6) {
						// play sound and add 30 seconds to the time
						startTime-=30;
						SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 44100, 0,&ptwiiing_raw, ptwiiing_raw_size, opt_sound, opt_sound, NULL);
						return;
					}
				}
			}
		}
	}

}

void compressTiles() {
	int x,y;
	for(x=0;x<tilesLeft;x+=2) {
		// if both tiles are blank get a pair for further away
		if(tiles[x]==BLANK && tiles[x+1]==BLANK) {
			y=x+2;
			while(tiles[y]==BLANK && tiles[y+1]==BLANK && y<MAX_TILES) y+=2;

			tiles[x]=tiles[y];
			tiles[x+1]=tiles[y+1];
			tiles[y]=BLANK;
			tiles[y+1]=BLANK;
		}

		// check if one of the tile pairs is blank and find the other one
		if(tiles[x]==BLANK || tiles[x+1]==BLANK) {
			int z;
			if(tiles[x]!=BLANK) {
				z=realTileNum(tiles[x]);
			} else {
				z=realTileNum(tiles[x+1]);
			}

			// find matching tile
			y=x+2;
			while(realTileNum(tiles[y])!=z && y<MAX_TILES) {
				y++;
			}

			// replace blank tile with found match
			if(tiles[x]==BLANK)
				tiles[x]=tiles[y];
			else
				tiles[x+1]=tiles[y];

			tiles[y]=BLANK;
		}
	}

}

char realTileNum(char tileNum) {
	if(tileNum<34)
		return tileNum;

	return (tileNum-34)/4+34;
}

void setupFade(int n) {
	fade[n].tile=sel[n].tile;
	fade[n].x=TILEX(sel[n].x,sel[n].z);
	fade[n].y=TILEY(sel[n].y,sel[n].z);
	fade[n].alpha=255;
	fade[n].scale=1;
	fade[n].fading=true;
}

int whatGameMode() {
	return gamemode;
}
