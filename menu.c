/*===========================================
		Mahjongg Solitaire Wii
		Code     : Justin Wood
		Menu class
 ============================================*/
#include <stdio.h>
#include <string.h>

// FIXME:#include "lib/libpng/pngu/pngu.h"
#include <pngu.h>
#include "GRRLIB/GRRLIB.h"
#include "asndlib.h"       // sound library

#include "commons.h"
#include "menu.h"

#include "gfx/letterswhite_png.h"
#include "gfx/letters_png.h"

#include "gfx/japanwhite_png.h"
#include "gfx/japan_png.h"

#include "gfx/gamenumbers_png.h"

//#include "latin_map.h"
//#include "japanese_map.h"

//#include "english_lng.h"
//#include "french_lng.h"
//#include "italian_lng.h"
//#include "dutch_lng.h"
//#include "german_lng.h"
//#include "spanish_lng.h"
//#include "catalan_lng.h"
//#include "japanese_lng.h"
//#include "portuguese_lng.h"
//#include "hungarian_lng.h"
//#include "finnish_lng.h"
//#include "swedish_lng.h"
//#include "danish_lng.h"
//#include "norwegian_lng.h"

// main menu graphics
#include "gfx/mainback_jpg.h"
#include "gfx/mainfore_png.h"
#include "gfx/shanghi_png.h"

// play menu graphics
#include "gfx/playback_jpg.h"
#include "gfx/shade_png.h"
#include "gfx/playfore_png.h"

// options menu graphics
#include "gfx/optionsback_jpg.h"
#include "gfx/optionsfore_png.h"

// sound menu graphics
#include "gfx/soundback_jpg.h"
#include "gfx/unselectedball_png.h"
#include "gfx/selectedball_png.h"
#include "gfx/soundfore_png.h"

// language menu graphics
#include "gfx/flags_png.h"
#include "gfx/languages_png.h"
#include "gfx/HSminusButton_png.h"
#include "gfx/HSplusButton_png.h"

// layout menu graphics
#include "gfx/layoutback_png.h"
#include "gfx/smalltile_png.h"

// tileset menu graphics
#include "gfx/border_png.h"

#include "sound/Click17a_raw.h"
#include "sound/gromb_raw.h"

#define MAIN_MENU 0
#define PLAY_MENU 1
#define OPTIONS_MENU 2
#define SOUND_MENU 3
#define LANG_MENU 4
#define LAYOUT_MENU 5
#define TILESET_MENU 6
#define HIGHSCORE_MENU 7

//function definitions
static void drawHighscoreMenu();

static void initMainMenu();

static void killMainMenu();

static void initPlayMenu();

static void killPlayMenu();

static void initSoundMenu();

static void killSoundMenu();

static void initOptionMenu();

static void initLangMenu();

static void killLangMenu();

static void initLayoutMenu();

static void killLayoutMenu();

static void initTilesetMenu();

static void killTilesetMenu();

static void initHighscoreMenu();

static void killHighscoreMenu();

static void playWrong();

static void playClick();

static void checkSelected(WPADData *wd);

static void setLanguage();

static void killMenu();

static void drawLayout(int n);

//variable definitions
static int mainhs[5][4] = {{201,199,240,56},{226,250,192,44},{192,299,256,48},{181,348,280,44},{277,395,88,48}};

static int playhs[4][4] = {{43,290,168,84},{230,291,176,76},{419,289,176,84},{495,395,100,44}};

static int soundhs[5][4] = {{195,111,252,52},{195,220,252,52},{495,395,100,44},{263,60,116,48},{261,171,120,44}};

static int opths[7][4] = {{176,30,292,48},{180,90,280,48},{164,270,212,48},{164,150,212,48},{164,210,212,48},{164,330,212,48},{495,395,100,44}};

static int langhs[9][2] = {{65,40},{240,40},{415,40},{65,180},{240,180},{415,180},{65,320},{240,320},{415,320}};

static int layouths[6][2] = {{20,72},{225,72},{440,72},{20,263},{225,263},{440,263}};

static int tileseths[6][2] = {{20,72},{225,72},{440,72},{20,263},{225,263},{440,263}};

char** curtext = 0 ;

static const char latin_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.'.ÄÜÉÓ...ÚÁÂÖÍ.";
static const char japanese_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ' ÄÜÉÓ   ÚÁÂÖÍ ";

static const unsigned char latinwidths[] = {25,26,26,27,24,26,27,26,20,24,27,22,27,
								24,26,26,27,27,25,25,27,27,36,29,26,24,
								16,24,24,27,25,26,24,24,26,26,28,12,25,
								25,26,24,26,25,26,27,27,25,25,26,20,25};

static const unsigned char japanwidths[] = {19,38,33,37,35,35,41,41,36,36,40,39,40,
								29,33,35,39,38,33,37,41,41,37,31,40,35,
								30,38,31,31,40,33,39,38,36,39,39,39,41,
								41,40,41,39,39,41,41,39,40,39,41,39,40,
								41,41,41,41,41,41,39,40,41,41,41,41,41,
								41,40,41,25,26,29,27,29,40,40,37,37,33,
								42,34,16,24,26,42,41,42,41,41,40,39,39,
								38,40,35,32,29,38,38,42,40,40,38,40};

static const unsigned char numwidths[] = {10,15,16,17,15,16,16,15,16,16,10,11,6};

static int curmenunum,msel=-1;

static u8 *tex_menufore,*tex_menuback, *tex_numbers, *tex_letters, *tex_whiteletters, *tex_japletters, *tex_japwhiteletters, *tex_flags, *tex_languages;
static u8 *tex_shade,*tex_sball, *tex_uball, *tex_smalltile, *tex_plus, *tex_minus, *tex_tiles[6], *tex_bk[6], *tex_border, *tex_layout, *tex_shanghi;
static int xsound,xmusic;

static bool btnover=false;
static int curpage=0;

static int SinSize=83;

static const int SinTab[]=
{-20 ,-20 ,-20 ,-19 ,-19 ,-18 ,-17 ,-17 ,
-16 ,-15 ,-14 ,-13 ,-12 ,-10 ,-9 ,-8 ,
-6 ,-5 ,-3 ,-2 , 0 , 1 , 2 , 4 ,
 5 , 7 , 8 , 9 , 11 , 12 , 13 , 14 ,
 15 , 16 , 17 , 18 , 18 , 19 , 19 , 20 ,
 20 , 20 , 20 , 20 , 20 , 19 , 19 , 18 ,
 18 , 17 , 16 , 15 , 14 , 13 , 12 , 11 ,
 10 , 9 , 7 , 6 , 4 , 3 , 1 , 0 ,
-1 ,-3 ,-4 ,-6 ,-7 ,-9 ,-10 ,-11 ,
-12 ,-13 ,-14 ,-15 ,-16 ,-17 ,-18 ,-18 ,
-19 ,-19 ,-20 ,-20 -20 };

static int oldz=0;

void initMenu() {
	xmusic=soundhs[0][0]+((double)opt_music/64*(soundhs[0][2]-44));
	xsound=soundhs[1][0]+((double)opt_sound/255*(soundhs[1][2]-44));

	if(opt_lang==-1)
		initLangMenu();
//	else {
	    if( curtext == 0) {
            setLanguage();
	    }
		initMainMenu();
//	}
}

static void setLanguage() {
	if(!tex_letters) {
		tex_japletters=GRRLIB_LoadTexture(japan_png);
		tex_japwhiteletters=GRRLIB_LoadTexture(japanwhite_png);
		tex_numbers=GRRLIB_LoadTexture(gamenumbers_png);

		//tex_numbers=GRRLIB_LoadTexture(gamenumbers_png);
		GRRLIB_SetFont(tex_numbers,20, 24, "1234567890:x-", 13, 1, numwidths, 0);

		GRRLIB_SetFont(tex_japwhiteletters,44,48,japanese_map,13,8, japanwidths,3);
		GRRLIB_SetFont(tex_japletters,44,48,japanese_map,13,8, japanwidths,4);

		tex_letters=GRRLIB_LoadTexture(letters_png);
		tex_whiteletters=GRRLIB_LoadTexture(letterswhite_png);

		GRRLIB_SetFont(tex_whiteletters,44,48,latin_map,13,4, latinwidths,1);
		GRRLIB_SetFont(tex_letters,44,48,latin_map,13,4, latinwidths,2);
	}

	switch(opt_lang) {
		case ENGLISH :
			curtext =  (char **)english_lng;
			break;
//		case FRENCH :
//			parseMenu((char *)french_lng, french_lng_size);
//			break;
//		case ITALIAN :
//			parseMenu((char *)italian_lng, italian_lng_size);
//			break;
//		case DUTCH :
//			parseMenu((char *)dutch_lng, dutch_lng_size);
//			break;
		case GERMAN :
			curtext = (char **)german_lng;
			break;
//		case SPANISH :
//			parseMenu((char *)spanish_lng, spanish_lng_size);
//			break;
//		case CATALAN :
//			parseMenu((char *)catalan_lng, catalan_lng_size);
//			break;
//		case JAPANESE :
//			parseMenu((char *)japanese_lng, japanese_lng_size);
//			break;
//		case PORTUGUESE :
//			parseMenu((char *)portuguese_lng, portuguese_lng_size);
//			break;
//		case HUNGARIAN :
//			parseMenu((char *)hungarian_lng, hungarian_lng_size);
//			break;
//		case FINNISH :
//			parseMenu((char *)finnish_lng, finnish_lng_size);
//			break;
//		case SWEDISH :
//			parseMenu((char *)swedish_lng, swedish_lng_size);
//			break;
//		case DANISH :
//			parseMenu((char *)danish_lng, danish_lng_size);
//			break;
//		case NORWEGIAN :
//			parseMenu((char *)norwegian_lng, norwegian_lng_size);
//			break;
        default:
			curtext = (char **)english_lng;
			break;
	}
}

static void killMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_menufore) free(tex_menufore);
}

void killMenuLanguages() {
	if(tex_japletters) free(tex_japletters);
	if(tex_japwhiteletters) free(tex_japwhiteletters);
	if(tex_letters) free(tex_letters);
	if(tex_whiteletters) free(tex_whiteletters);
	if(tex_numbers) free(tex_numbers);
}

void drawMenu(WPADData *wd) {
	int x,z=0,l,y,zz=0;
	GRRLIB_DrawImg(0, 0, 640, 480, tex_menuback, 0, 1, 1, 255);

	switch(curmenunum) {
		case MAIN_MENU :
			GRRLIB_DrawImg(0, 52, 608, 400, tex_menufore, 0, 1, 1, 255);

			// Overlay the Japanese name for Mahjongg Solitaire - Shanghi
			if(opt_lang==JAPANESE)
				GRRLIB_DrawImg(354, 142, 72, 40, tex_shanghi, 0, 1, 1, 255);

			for(x=0;x<5;x++) {
				GRRLIB_GPrintf(320,mainhs[x][1],0xFFFFFFFF,x==0?1.15:1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_MMENU_PLAY + x]);
			}

			GRRLIB_GPrintf( 10, 420, 0xFFFFFFFF, 0.5, 0.5, ALIGN_LEFT, CUR_FONT(true), VERSION_STRING);
			break;
		case PLAY_MENU :
			GRRLIB_DrawImg(34,30,576,264, tex_menufore, 0, 1, 1, 255);
			f32 zoom;
			int strlen;

			for(x=0;x<3;x++) {
				strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[LNG_PMENU_1PLAYER+x*2]);
				if(strlen>playhs[x][2]) {
					zoom=(float)playhs[x][2]/(float)strlen;
					GRRLIB_GPrintf(playhs[x][0],playhs[x][1],0xFFFFFFFF,zoom,1, ALIGN_LEFT,CUR_FONT(msel==x),curtext[LNG_PMENU_1PLAYER+x*2]);
				}
				else {
					GRRLIB_GPrintf(playhs[x][0]+playhs[x][2]/2,playhs[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_PMENU_1PLAYER+x*2]);
				}
				strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[LNG_PMENU_1PLAYER+x*2+1]);
				if(strlen>playhs[x][2]) {
					zoom=(float)playhs[x][2]/(float)strlen;
					GRRLIB_GPrintf(playhs[x][0],playhs[x][1]+36,0xFFFFFFFF,zoom,1, ALIGN_LEFT,CUR_FONT(msel==x),curtext[LNG_PMENU_1PLAYER+x*2+1]);
				}
				else {
					GRRLIB_GPrintf(playhs[x][0]+playhs[x][2]/2,playhs[x][1]+36,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_PMENU_1PLAYER+x*2+1]);
				}
			}

			GRRLIB_GPrintf(600, playhs[3][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[LNG_GAME_BACK]);

			for(x=0;x<3;x++) {
				if(!(x==msel))
					GRRLIB_DrawImg(playhs[x][0], 90, 168, 196,tex_shade, 0, 1, 1, 255);
			}
			break;
		case OPTIONS_MENU :
			GRRLIB_DrawImg(-9, 30, 640, 420, tex_menufore, 0, 1, 1, 255);

			for(x=0;x<6;x++) {
				// check if on/off entry
				if(x==4 || x==2 || x==5) {
					char buffer[60];

					strcpy(buffer,curtext[LNG_OMENU_CTILESET+x]);

					if((x==4 && opt_rumble) || (x==2 && opt_hoverhint) || (x==5 && opt_widescreen)) {
						char *s2 = strchr(buffer,'/');
						if(s2!=NULL)
							buffer[s2-buffer]='\0';
					}
					else {
						char *s2=strchr(buffer,'/') + 1;
						char *s1=strrchr(buffer,' ') + 1;
						if(s1!=NULL && s2!=NULL)
							memmove(s1,s2,60-(s2-buffer)+1);
					}

					GRRLIB_GPrintf(320,opths[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),buffer);

				}
				else
					GRRLIB_GPrintf(320,opths[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_OMENU_CTILESET+x]);
			}
			GRRLIB_GPrintf(600, opths[6][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[LNG_GAME_BACK]);

			break;
		case SOUND_MENU :
			GRRLIB_DrawImg(86,30,524,436, tex_menufore, 0, 1, 1, 255);

			for(x=0;x<2;x++) {
				GRRLIB_GPrintf(320,soundhs[x+3][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_SMENU_MUSIC+x]);
			}
			GRRLIB_GPrintf(600, soundhs[2][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[LNG_GAME_BACK]);

			// draw the offs
			GRRLIB_GPrintf(186,115,0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(false),curtext[LNG_SMENU_MUSIC+2]);
			GRRLIB_GPrintf(186,224,0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(false),curtext[LNG_SMENU_MUSIC+2]);

			// draw the fulls
			GRRLIB_GPrintf(454,115,0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_SMENU_MUSIC+3]);
			GRRLIB_GPrintf(454,224,0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(false),curtext[LNG_SMENU_MUSIC+3]);

			if(msel==0) {
				if(wd->btns_h & WPAD_BUTTON_A)
					GRRLIB_DrawImg(xmusic-4, soundhs[0][1]-2, 60, 60, tex_sball, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(xmusic, soundhs[0][1]+2, 44, 44, tex_uball, 0, 1, 1, 255);
			}
			else
				GRRLIB_DrawImg(xmusic, soundhs[0][1]+2, 44, 44, tex_uball, 0, 1, 1, 255);

			if(msel==1) {
				if(wd->btns_h & WPAD_BUTTON_A)
					GRRLIB_DrawImg(xsound-4, soundhs[1][1]-2, 60, 60, tex_sball, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(xsound, soundhs[1][1]+2, 44, 44, tex_uball, 0, 1, 1, 255);
			}
			else
				GRRLIB_DrawImg(xsound, soundhs[1][1]+2, 44, 44, tex_uball, 0, 1, 1, 255);

			break;
		case LANG_MENU :
			for(x=0;x<9;x++) {
				// set
				int tx = x + (curpage==0?0:9);
				// only show if overall less then number of languages supported
				if(tx<=13) {
					if(x==msel) {
						zz=512-oldz;
						for(y=0;y<108;y+=4) {
							z=oldz;
							for(l=0;l<148;l+=4) {
								GRRLIB_DrawGTile(langhs[x][0]-5+l+SinTab[zz%SinSize]/4,langhs[x][1]+y+SinTab[z%SinSize]/4,4,4,tex_flags,148,108,0,1.1,1.1,0xFFFFFFFF,(tx%4)*37+(tx/4)*27*37*4 +l/4+(y/4)*37*4);
								z++;
							}
							zz--;
						}
					}
					else
						GRRLIB_DrawGTile(langhs[x][0],langhs[x][1],148,108,tex_flags,4,4,0,1,1,0xFFEEEEEE,tx);

					GRRLIB_DrawGTile(langhs[x][0]-8,langhs[x][1]+80,160,40,tex_languages,4,4,0,1,1,0xFFFFFFFF,tx);
				}
			}

			oldz+=1;
			if(oldz>=256) oldz=0;

			if(curpage==0) {
				if(btnover)
					GRRLIB_DrawImg(580,220,40,40, tex_plus, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(580,220,40,40, tex_plus, 0, 0.8, 0.8, 127);
			}
			else {
				if(btnover)
					GRRLIB_DrawImg(10,220,40,40, tex_minus, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(10,220,40,40, tex_minus, 0, 0.8, 0.8, 128);
			}

			break;
		case LAYOUT_MENU :
			GRRLIB_GPrintf(320,27,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(true),curtext[LNG_OMENU_CLAYOUT]);

			for(x=0;x<6;x++) {
				GRRLIB_DrawImg(layouths[x][0],layouths[x][1]-10,180,208, tex_layout, 0, 1, 0.8, 255);
				int tx = x + (curpage==0?0:6);
				drawLayout(tx);
			}

			for(x=0;x<6;x++) {
				int tx = x + (curpage==0?0:6);
				if(!(x==msel))
					GRRLIB_DrawImg(layouths[x][0]+4, layouths[x][1]-8, 168, 196,tex_shade, 0, 1, 0.8, 255);
					if(opt_lang==JAPANESE)
						GRRLIB_GPrintf(layouths[x][0]+92,layouths[x][1]+152,0xFFFFFFFF,1,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_LAYOUT_DEFAULT+tx]);
					else
						GRRLIB_GPrintf(layouths[x][0]+77,layouths[x][1]+152,0xFFFFFFFF,0.55,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_LAYOUT_DEFAULT+tx]);
			}

			if(curpage==0) {
				if(btnover)
					GRRLIB_DrawImg(600,240,40,40, tex_plus, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(600,240,40,40, tex_plus, 0, 0.8, 0.8, 127);
			}
			else {
				if(btnover)
					GRRLIB_DrawImg(0,240,40,40, tex_minus, 0, 1, 1, 255);
				else
					GRRLIB_DrawImg(0,240,40,40, tex_minus, 0, 0.8, 0.8, 128);
			}
			break;
		case TILESET_MENU :
		{
			GRRLIB_GPrintf(320,27,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(true),curtext[LNG_OMENU_CTILESET]);

			for(x=0;x<6;x++) {
				GRRLIB_DrawImg(layouths[x][0],layouths[x][1]-10,180,208, tex_layout, 0, 1, 0.8, 255);

				GRRLIB_DrawImg(layouths[x][0]+4-236,layouths[x][1]-148,640,480,tex_bk[x],0,0.24,0.3,255);

				GRRLIB_DrawImg(layouths[x][0]+4,layouths[x][1]-10+3,168, 200, tex_border, 0, 1, 0.8, 255);

				for(y=0;y<4;y++) {
					GRRLIB_DrawGTile(layouths[x][0]+21+y*30,layouths[x][1]+29+y*22,44,60,tex_tiles[x],21,2,0,1,1,0xFFFFFFFF,y*11);
				}
			}

			for(x=0;x<6;x++) {
				if(!(x==msel))
					GRRLIB_DrawImg(layouths[x][0]+4, layouths[x][1]-8, 168, 196,tex_shade, 0, 1, 0.8, 255);
					if(opt_lang==JAPANESE)
						GRRLIB_GPrintf(layouths[x][0]+92,layouths[x][1]+152,0xFFFFFFFF,1,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_TILESET_DEFAULT+x]);
					else
						GRRLIB_GPrintf(layouths[x][0]+77,layouths[x][1]+152,0xFFFFFFFF,0.55,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LNG_TILESET_DEFAULT+x]);
			}
		}
		break;
		case HIGHSCORE_MENU:
		{
		    drawHighscoreMenu();
		}
		break;
	}
}

static void drawHighscoreMenu()
{
    GRRLIB_GPrintf( 320, 27, 0xFFFFFFFF, 1, 1, ALIGN_CENTRE, CUR_FONT(true), curtext[LNG_MMENU_HISCORES]);

    int layIndex;
    int column = 100;
    int YOFFSET = 100;
    int YSPACE = 90;
    int y = 0;
    char strTime[6];

    for( layIndex = 0; layIndex < LAYOUTS; ++layIndex)
    {
        GRRLIB_GPrintf( column, YOFFSET + y * YSPACE, 0xFFFFFFFF, 0.6, 0.6, ALIGN_CENTRE, CUR_FONT(true), curtext[LNG_LAYOUT_DEFAULT + layIndex]);

        snprintf( strTime, sizeof(strTime), "%02d:%02d", (int)(g_scores[layIndex * 2] / 60), (int)(g_scores[layIndex * 2] % 60));
        if( g_scores[layIndex * 2] == 0)
            snprintf( strTime, sizeof(strTime), "--:--");

        GRRLIB_GPrintf( column - 90, YOFFSET + y * YSPACE + 25, 0xFFFFFFFF, 0.5, 0.5, ALIGN_LEFT, CUR_FONT(false), curtext[LNG_PMENU_1PLAYER]);
        GRRLIB_GPrintf( column + 30, YOFFSET + y * YSPACE + 35, 0xFFFFFFFF, 1, 1, ALIGN_LEFT, 0, strTime);

        snprintf( strTime, sizeof(strTime), "%02d:%02d", (int)(g_scores[layIndex * 2 + 1] / 60), (int)(g_scores[layIndex * 2 + 1] % 60));
        if( g_scores[layIndex * 2 + 1] == 0)
            snprintf( strTime, sizeof(strTime), "--:--");

        GRRLIB_GPrintf( column - 90, YOFFSET + y * YSPACE + 50, 0xFFFFFFFF, 0.5, 0.5, ALIGN_LEFT, CUR_FONT(false), curtext[LNG_PMENU_2PLAYERCOOP]);
        GRRLIB_GPrintf( column + 30, YOFFSET + y * YSPACE + 60, 0xFFFFFFFF, 1, 1, ALIGN_LEFT, 0, strTime);

        y++;
        if( (layIndex + 1) % 4 == 0)
        {
            y = 0;
            column += 210;
        }
    }
}

static void drawLayout(int n) {
	unsigned char grid[MAX_WIDTH][MAX_HEIGHT][MAX_LAYERS];
	int x,y,z,c,l,pos=0;
	int coord = n % 6;

	for(z=0;z<MAX_LAYERS;++z) {
		for(x=0;x<MAX_WIDTH;++x) {
			for(y=0;y<MAX_HEIGHT;++y) {
				grid[x][y][z]=0;
			}
		}
	}

	for(l=0;l<MAX_TILES;++l) {
		x=layouts[n][pos++];
		y=layouts[n][pos++];
		z=layouts[n][pos++];
		grid[x][y][z]=255;
	}

	for(z=0;z<MAX_LAYERS;++z) {
		for(x=0;x<MAX_WIDTH+MAX_HEIGHT;++x) {
			for(y=(x>=MAX_WIDTH?x-MAX_WIDTH:0),c=(x>=MAX_WIDTH?MAX_WIDTH-1:x);y<MAX_HEIGHT && c>=0;++y,--c) {
				if(grid[c][y][z]==255) {
					GRRLIB_DrawImg(layouths[coord][0]+14+c*5-z,layouths[coord][1]+34+y*7-z,12,16,tex_smalltile,0,1,1,255);
				}
			}
		}
	}

}

int menuWiimote(WPADData *wd, u32 wpaddown) {
	checkSelected(wd);

	// button A selects an option
	if(wpaddown & WPAD_BUTTON_A) {
		switch(curmenunum) {
			case MAIN_MENU :
				switch(msel) {
					case 0:
                        //play menu
						playClick();
						killMainMenu();
						initPlayMenu();
						break;
					case 1 :
                        //hi score
						playClick();
                        killMainMenu();
                        initHighscoreMenu();
						break;
					case 2 :
                        //optionsmenu
						playClick();
						killMainMenu();
						initOptionMenu();
						break;
					case 3 :
                        //sound menu
						playClick();
						killMainMenu();
						initSoundMenu();
						break;
					case 4 :
                        //exit
						playClick();
						killMainMenu();
						return EXIT;
				}
				break;
			case PLAY_MENU :
				switch(msel) {
					case 0 :
						playClick();
						killPlayMenu();
						return ONE_PLAYER_GAME;
						break;
					case 1 :
						playClick();
						killPlayMenu();
						return TWO_PLAYER_COOP;
						break;
					case 2 :
						playClick();
						killPlayMenu();
						return TWO_PLAYER_VERSUS;
						break;
					case 3 :
						playClick();
						killPlayMenu();
						initMainMenu();
						break;
				}
				break;
			case OPTIONS_MENU :
				switch(msel) {
					case 0 :
						// change tileset
						playClick();
						killMenu();
						initTilesetMenu();
						break;
					case 1 :
						// change layout
						playClick();
						killMenu();
						initLayoutMenu();
						break;
					case 2 :
						// hover hint
						playWrong();
						break;
					case 3 :
						// language selection
						playClick();
						killMenu();
						initLangMenu();
						break;
					case 4 :
						// rumble on/off
						if(opt_rumble)
							opt_rumble=false;
						else
							opt_rumble=true;
						playClick();
						break;
					case 5 :
						// widescreen on/off
						if(opt_widescreen)
							opt_widescreen=false;
						else
							opt_widescreen=true;
						GRRLIB_Widescreen(opt_widescreen);
						playClick();
						break;
					case 6 :
						// back
						playClick();
						killMenu();
						initMainMenu();
						break;
				}
				break;
			case SOUND_MENU :
				switch(msel) {
					case 2 :
						playClick();
						killSoundMenu();
						initMainMenu();
						break;
				}
				break;
			case LANG_MENU :
				if(msel>-1) {
					int tmp_lang=msel + (curpage==0?0:9);
					if(tmp_lang>NORWEGIAN) return NOTHING;
					opt_lang=tmp_lang;
					setLanguage();
					playClick();
					killLangMenu();
					initOptionMenu();
				}
				if(btnover) {
				    curpage++;
					curpage = curpage % 2;
					playClick();
				}
				break;
			case LAYOUT_MENU :
				if(msel>-1) {
					playClick();
					int tmp_sel = msel + (curpage==0?0:6);
					opt_layout=tmp_sel;
					killLayoutMenu();
					initOptionMenu();
				}
				if(btnover) {
				    curpage++;
					curpage = curpage % 2;
					playClick();
				}
				break;
			case TILESET_MENU :
			{
				if(msel>-1) {
					playClick();
					opt_tileset=msel;
					killTilesetMenu();
					initOptionMenu();
				}
			}
			break;

			case HIGHSCORE_MENU:
			{
                playClick();
                killHighscoreMenu();
                initMainMenu();
			}
			break;
		}
	}

	if (curmenunum==SOUND_MENU && wd->btns_h & WPAD_BUTTON_A) {
		switch(msel) {
			case 0 :
				xmusic=wd->ir.x;
				opt_music = (int) (((double)xmusic-soundhs[0][0])/soundhs[0][2]*64);
				return MUSIC_BAR + opt_music;
				break;
			case 1 :
				xsound=wd->ir.x;
				opt_sound = (int) (((double)xsound-soundhs[1][0])/soundhs[1][2]*255);
				playWrong();
				break;
		}
	}

	// button B acts the same as back
	if(wpaddown & WPAD_BUTTON_B) {
		switch(curmenunum) {
			case MAIN_MENU :
				break;
			case PLAY_MENU :
				playClick();
				killPlayMenu();
				initMainMenu();
				break;
			case OPTIONS_MENU :
				playClick();
				killMenu();
				initMainMenu();
				break;
			case SOUND_MENU :
				playClick();
				killSoundMenu();
				initMainMenu();
				break;
			case LANG_MENU :
				playClick();
				killLangMenu();
				initOptionMenu();
				break;
			case LAYOUT_MENU :
				playClick();
				killLayoutMenu();
				initOptionMenu();
				break;
			case TILESET_MENU :
			{
				playClick();
				killTilesetMenu();
				initOptionMenu();
			}
			break;

			case HIGHSCORE_MENU:
			{
                playClick();
                killHighscoreMenu();
                initMainMenu();
			}
			break;
		}
	}

	if(wpaddown & WPAD_BUTTON_PLUS) {
		if((curmenunum==LANG_MENU || curmenunum==LAYOUT_MENU) && curpage==0) curpage++;
	}

	if(wpaddown & WPAD_BUTTON_MINUS) {
		if((curmenunum==LANG_MENU || curmenunum==LAYOUT_MENU) && curpage==1) curpage--;
	}

	return NOTHING;
}

static void checkSelected(WPADData *wd) {
	int x;
	switch(curmenunum) {
		case MAIN_MENU :
			for(x=0;x<5;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, mainhs[x][0], mainhs[x][1], mainhs[x][2], mainhs[x][3]) ) {
					msel=x;
					return;
				}
			}
			break;
		case PLAY_MENU :
			for(x=0;x<4;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, playhs[x][0], playhs[x][1], playhs[x][2], playhs[x][3]) ) {
					msel=x;
					return;
				}

				if(x<3 && CONTAINS( wd->ir.x, wd->ir.y, playhs[x][0], 90, 168, 286) ) {
					msel=x;
					return;
				}
			}
			break;
		case SOUND_MENU :
			for(x=0;x<3;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, soundhs[x][0], soundhs[x][1], soundhs[x][2] - (x==2?1:48), soundhs[x][3] ) ) {
					msel=x;
					return;
				}
			}
			break;
		case OPTIONS_MENU :
			for(x=0;x<7;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, opths[x][0], opths[x][1], opths[x][2], opths[x][3]) ) {
					msel=x;
					return;
				}
			}
			break;
		case LANG_MENU :
			for(x=0;x<9;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, langhs[x][0], langhs[x][1], 148, 108) ) {
					msel=x;
					return;
				}
			}
			if(curpage==0) {
				if( CONTAINS( wd->ir.x, wd->ir.y, 580, 220, 620, 260) )
					btnover=true;
				else
					btnover=false;
			}
			else {
				if( CONTAINS( wd->ir.x, wd->ir.y, 10, 220, 50, 260) )
					btnover=true;
				else
					btnover=false;
			}
			break;
		case LAYOUT_MENU :
		{
			for(x=0;x<6;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, layouths[x][0], layouths[x][1], 168, 170) ) {
					msel=x;
					return;
				}
			}
			if(curpage==0) {
				if( CONTAINS( wd->ir.x, wd->ir.y, 600, 240, 640, 280) )
					btnover=true;
				else
					btnover=false;
			}
			else {
				if( CONTAINS( wd->ir.x, wd->ir.y, 0, 240, 40, 280) )
					btnover=true;
				else
					btnover=false;
			}
			break;
        }
        break;
		case TILESET_MENU :
		{
			for(x=0;x<6;x++) {
				if( CONTAINS( wd->ir.x, wd->ir.y, tileseths[x][0], tileseths[x][1], 168, 170) ) {
					msel=x;
					return;
				}
			}
		}
		break;

		case HIGHSCORE_MENU:
		{
		    msel = -1;
		}
		break;
	}
	msel=-1;
}

static void initMainMenu() {
	int x,strsize;

	tex_menuback=GRRLIB_LoadJPG(mainback_jpg, mainback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(mainfore_png);

    tex_shanghi=0;
	tex_shanghi=GRRLIB_LoadTexture(shanghi_png);

	for(x=0;x<5;x++) {
		strsize=GRRLIB_GetStringWidth(CUR_FONT(0),curtext[x]);
		mainhs[x][0]=320-strsize/2;
		mainhs[x][2]=strsize;
	}

	curmenunum = MAIN_MENU;
}

static void killMainMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_menufore) free(tex_menufore);

	if(tex_shanghi) free(tex_shanghi);
}


static void initPlayMenu() {
	int x;

	tex_menuback=GRRLIB_LoadJPG(playback_jpg, playback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(playfore_png);

	tex_shade=GRRLIB_LoadTexture(shade_png);

	int strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_BACK]);
	playhs[3][0]=600-strsize;
	playhs[3][2]=strsize;

	// if we only have one line then half the selectable hight
	for(x=0;x<3;x++) {
		if(strcmp(curtext[LNG_PMENU_1PLAYER+1+x*2]," ")==0)
			playhs[x][3]/=2;
	}

	curmenunum = PLAY_MENU;
}

static void killPlayMenu() {
	killMenu();

	if(tex_shade) free(tex_shade);
}

static void initSoundMenu() {
	tex_menuback=GRRLIB_LoadJPG(soundback_jpg, soundback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(soundfore_png);

	tex_uball=GRRLIB_LoadTexture(unselectedball_png);
	tex_sball=GRRLIB_LoadTexture(selectedball_png);

	int strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[LNG_GAME_BACK]);
	soundhs[2][0]=600-strsize;
	soundhs[2][2]=strsize;

	curmenunum = SOUND_MENU;
}

static void killSoundMenu() {
	killMenu();

	if(tex_uball) free(tex_uball);
	if(tex_sball) free(tex_sball);
}

static void initOptionMenu() {
	int x,strsize;

	tex_menuback=GRRLIB_LoadJPG(optionsback_jpg, optionsback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(optionsfore_png);

	for(x=0;x<7;x++) {
		strsize=GRRLIB_GetStringWidth(CUR_FONT(false),x==6?curtext[LNG_GAME_BACK]:curtext[LNG_OMENU_CTILESET+x]);
		if(x==6)
			opths[x][0]=600-strsize;
		else
			opths[x][0]=320-strsize/2;

		opths[x][2]=strsize;
	}

	curmenunum = OPTIONS_MENU;
}

static void initLangMenu() {
//    curtext[0] = 0;
	tex_menuback=GRRLIB_LoadJPG(mainback_jpg, mainback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(mainfore_png);
	tex_flags=0;
	tex_languages=0;
	tex_flags=GRRLIB_LoadTexture(flags_png);
	tex_languages=GRRLIB_LoadTexture(languages_png);
	tex_plus=GRRLIB_LoadTexture(HSplusButton_png);
	tex_minus=GRRLIB_LoadTexture(HSminusButton_png);
	curpage = 0;

	curmenunum = LANG_MENU;
}

static void killLangMenu() {
	killMenu();

	if(tex_flags) free(tex_flags);
	if(tex_languages) free(tex_languages);

	if(tex_plus) free(tex_plus);
	if(tex_minus) free(tex_minus);
}

static void initLayoutMenu() {
	tex_menuback=GRRLIB_LoadJPG(playback_jpg, playback_jpg_size);
	tex_layout=GRRLIB_LoadTexture(layoutback_png);
	tex_smalltile=GRRLIB_LoadTexture(smalltile_png);
	tex_shade=GRRLIB_LoadTexture(shade_png);
	tex_plus=GRRLIB_LoadTexture(HSplusButton_png);
	tex_minus=GRRLIB_LoadTexture(HSminusButton_png);
	curpage = 0;

	curmenunum = LAYOUT_MENU;
}

static void killLayoutMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_layout) free(tex_layout);
	if(tex_smalltile) free(tex_smalltile);
	if(tex_shade) free(tex_shade);
	if(tex_plus) free(tex_plus);
	if(tex_minus) free(tex_minus);
}

static void initTilesetMenu() {
	tex_menuback=GRRLIB_LoadJPG(playback_jpg, playback_jpg_size);
	tex_layout=GRRLIB_LoadTexture(layoutback_png);
	tex_border=GRRLIB_LoadTexture(border_png);
	tex_shade=GRRLIB_LoadTexture(shade_png);

	tex_tiles[0]=GRRLIB_LoadTexture(ts_default_png);
	tex_tiles[1]=GRRLIB_LoadTexture(ts_simple_png);
	tex_tiles[2]=GRRLIB_LoadTexture(ts_old_png);
	tex_tiles[3]=GRRLIB_LoadTexture(ts_spooky_png);
	tex_tiles[4]=GRRLIB_LoadTexture(ts_egypt_png);
	tex_tiles[5]=GRRLIB_LoadTexture(ts_space_png);

	tex_bk[0]=GRRLIB_LoadJPG(gameback_jpg, gameback_jpg_size);
	tex_bk[1]=tex_bk[0];
	tex_bk[2]=tex_bk[0];
	tex_bk[3]=GRRLIB_LoadJPG(bk_spooky_jpg, bk_spooky_jpg_size);
	tex_bk[4]=GRRLIB_LoadJPG(bk_egypt_jpg, bk_egypt_jpg_size);
	tex_bk[5]=GRRLIB_LoadJPG(bk_space_jpg, bk_space_jpg_size);

	curmenunum = TILESET_MENU;
}

static void killTilesetMenu() {
	if(tex_menuback) free(tex_menuback);

	if(tex_layout) free(tex_layout);

	if(tex_border) free(tex_border);
	if(tex_shade) free(tex_shade);

	if(tex_tiles[0]) free(tex_tiles[0]);
	if(tex_tiles[1]) free(tex_tiles[1]);
	if(tex_tiles[2]) free(tex_tiles[2]);
	if(tex_tiles[3]) free(tex_tiles[3]);
	if(tex_tiles[4]) free(tex_tiles[4]);
	if(tex_tiles[5]) free(tex_tiles[5]);

	if(tex_bk[0]) free(tex_bk[0]);
	if(tex_bk[3]) free(tex_bk[3]);
	if(tex_bk[4]) free(tex_bk[4]);
	if(tex_bk[5]) free(tex_bk[5]);
}

static void initHighscoreMenu()
{
    tex_menuback=GRRLIB_LoadJPG(mainback_jpg, mainback_jpg_size);
    curmenunum = HIGHSCORE_MENU;
}

static void killHighscoreMenu()
{
    if( tex_menuback) free(tex_menuback);
}

static void playWrong() {
    if( opt_sound > 0)
        SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 22050, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
}

static void playClick() {
    if( opt_sound > 0)
        SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_16BIT, 36000, 0,&Click17a_raw, Click17a_raw_size, opt_sound, opt_sound, NULL);
}

// vim:et sw=4 ts=4 ai
