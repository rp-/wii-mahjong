/*===========================================
		Mahjongg Solitaire Wii
		Code     : Justin Wood
		Menu class
 ============================================*/
#include "menu.h"
#include "commons.h"

#define MAIN_MENU 0
#define PLAY_MENU 1
#define OPTIONS_MENU 2
#define SOUND_MENU 3
#define LANG_MENU 4
#define LAYOUT_MENU 5
#define TILESET_MENU 6

#define MAIN_POS 0
#define PLAY_POS 5
#define OPTIONS_POS 12
#define SOUND_POS 18
#define REMARK_POS 22
#define LAYOUT_POS 35
#define TILESET_POS 41

int mainhs[5][4] = {{201,199,240,56},{226,250,192,44},{192,299,256,48},{181,348,280,44},{277,395,88,48}};

int playhs[4][4] = {{43,290,168,84},{230,291,176,76},{419,289,176,84},{495,395,100,44}};

int soundhs[5][4] = {{195,111,252,52},{195,220,252,52},{495,395,100,44},{263,60,116,48},{261,171,120,44}};

int opths[7][4] = {{176,30,292,48},{180,90,280,48},{164,270,212,48},{164,150,212,48},{164,210,212,48},{164,330,212,48},{495,395,100,44}};

int langhs[9][2] = {{65,40},{240,40},{415,40},{65,180},{240,180},{415,180},{65,320},{240,320},{415,320}};

int layouths[6][2] = {{20,72},{225,72},{440,72},{20,263},{225,263},{440,263}};

int tileseths[6][2] = {{20,72},{225,72},{440,72},{20,263},{225,263},{440,263}};

char *curtext[50];

unsigned char latinwidths[] = {25,26,26,27,24,26,27,26,20,24,27,22,27,
								24,26,26,27,27,25,25,27,27,36,29,26,24,
								16,24,24,27,25,26,24,24,26,26,28,12,25,
								25,26,24,26,25,26,27,27,25,25,26,20,25};

unsigned char japanwidths[] = {19,38,33,37,35,35,41,41,36,36,40,39,40,
								29,33,35,39,38,33,37,41,41,37,31,40,35,
								30,38,31,31,40,33,39,38,36,39,39,39,41,
								41,40,41,39,39,41,41,39,40,39,41,39,40,
								41,41,41,41,41,41,39,40,41,41,41,41,41,
								41,40,41,25,26,29,27,29,40,40,37,37,33,
								42,34,16,24,26,42,41,42,41,41,40,39,39,
								38,40,35,32,29,38,38,42,40,40,38,40};

int curmenunum,msel=-1;

u8 *tex_menufore,*tex_menuback, *tex_letters, *tex_whiteletters, *tex_japletters, *tex_japwhiteletters, *tex_flags, *tex_languages;
u8 *tex_shade,*tex_sball, *tex_uball, *tex_smalltile, *tex_plus, *tex_minus, *tex_tiles[6], *tex_bk[6], *tex_border, *tex_layout, *tex_shanghi;
int xsound,xmusic;

bool btnover=false;
int lang_page=0;

static int SinSize=83;

int SinTab[]=
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

int oldz=0;

void initMenu() {
	xmusic=soundhs[0][0]+((double)opt_music/64*(soundhs[0][2]-44));
	xsound=soundhs[1][0]+((double)opt_sound/255*(soundhs[1][2]-44));

	if(opt_lang==-1)
		initLangMenu();
	else {
		setLanguage();
		initMainMenu();
	}
}

void setLanguage() {
	if(!tex_letters) {
		tex_japletters=GRRLIB_LoadTexture(japan_png);
		tex_japwhiteletters=GRRLIB_LoadTexture(japanwhite_png);

		GRRLIB_SetFont(tex_japwhiteletters,44,48,japanese_map,13,8, japanwidths,3);
		GRRLIB_SetFont(tex_japletters,44,48,japanese_map,13,8, japanwidths,4);

		tex_letters=GRRLIB_LoadTexture(letters_png);
		tex_whiteletters=GRRLIB_LoadTexture(letterswhite_png);

		GRRLIB_SetFont(tex_whiteletters,44,48,latin_map,13,4, latinwidths,1);
		GRRLIB_SetFont(tex_letters,44,48,latin_map,13,4, latinwidths,2);
	}

	switch(opt_lang) {
		case ENGLISH :
			parseMenu((char *)english_lng, english_lng_size);
			break;
		case FRENCH :
			parseMenu((char *)french_lng, french_lng_size);
			break;
		case ITALIAN :
			parseMenu((char *)italian_lng, italian_lng_size);
			break;
		case DUTCH :
			parseMenu((char *)dutch_lng, dutch_lng_size);
			break;
		case GERMAN :
			parseMenu((char *)german_lng, german_lng_size);
			break;
		case SPANISH :
			parseMenu((char *)spanish_lng, spanish_lng_size);
			break;
		case CATALAN :
			parseMenu((char *)catalan_lng, catalan_lng_size);
			break;
		case JAPANESE :
			parseMenu((char *)japanese_lng, japanese_lng_size);
			break;
		case PORTUGUESE :
			parseMenu((char *)portuguese_lng, portuguese_lng_size);
			break;
		case HUNGARIAN :
			parseMenu((char *)hungarian_lng, hungarian_lng_size);
			break;
		case FINNISH :
			parseMenu((char *)finnish_lng, finnish_lng_size);
			break;
		case SWEDISH :
			parseMenu((char *)swedish_lng, swedish_lng_size);
			break;
		case DANISH :
			parseMenu((char *)danish_lng, danish_lng_size);
			break;
		case NORWEGIAN :
			parseMenu((char *)norwegian_lng, norwegian_lng_size);
			break;
	}
}

void killMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_menufore) free(tex_menufore);
}

void killMenuLanguages() {
	if(tex_japletters) free(tex_japletters);
	if(tex_japwhiteletters) free(tex_japwhiteletters);
	if(tex_letters) free(tex_letters);
	if(tex_whiteletters) free(tex_whiteletters);
}


void parseMenu(char *str,u32 str_len) {
	int x,pos=0;

	if(strlen(str)<40) {
		curtext[0]=str;
		for(x=1;x<49;x++) {
			curtext[x]=memchr(&str[pos], '\0',str_len) + 1;
			if(curtext[x-1]!=NULL)
				pos+=strlen(curtext[x-1]) + 1;
		}
	}
	else {
		curtext[0]=strtok(str,",\n\0");

		for(x=1;x<49;x++) {
			curtext[x]=strtok(NULL,",\n\0");
		}
	}
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
				GRRLIB_GPrintf(320,mainhs[x][1],0xFFFFFFFF,x==0?1.15:1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[x]);
			}

			break;
		case PLAY_MENU :
			GRRLIB_DrawImg(34,30,576,264, tex_menufore, 0, 1, 1, 255);
			f32 zoom;
			int strlen;

			for(x=0;x<3;x++) {
				strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[PLAY_POS+x*2]);
				if(strlen>playhs[x][2]) {
					zoom=(float)playhs[x][2]/(float)strlen;
					GRRLIB_GPrintf(playhs[x][0],playhs[x][1],0xFFFFFFFF,zoom,1, ALIGN_LEFT,CUR_FONT(msel==x),curtext[PLAY_POS+x*2]);
				}
				else {
					GRRLIB_GPrintf(playhs[x][0]+playhs[x][2]/2,playhs[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[PLAY_POS+x*2]);
				}
				strlen = GRRLIB_GetStringWidth(CUR_FONT(false), curtext[PLAY_POS+x*2+1]);
				if(strlen>playhs[x][2]) {
					zoom=(float)playhs[x][2]/(float)strlen;
					GRRLIB_GPrintf(playhs[x][0],playhs[x][1]+36,0xFFFFFFFF,zoom,1, ALIGN_LEFT,CUR_FONT(msel==x),curtext[PLAY_POS+x*2+1]);
				}
				else {
					GRRLIB_GPrintf(playhs[x][0]+playhs[x][2]/2,playhs[x][1]+36,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[PLAY_POS+x*2+1]);
				}
			}

			GRRLIB_GPrintf(600, playhs[3][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[REMARK_POS]);

			for(x=0;x<3;x++) {
				if(!(x==msel))
					GRRLIB_DrawImg(playhs[x][0], 90, 168, 196,tex_shade, 0, 1, 1, 255);
			}
			break;
		case OPTIONS_MENU :
			GRRLIB_DrawImg(-9, 30, 640, 420, tex_menufore, 0, 1, 1, 255);

			for(x=0;x<6;x++) {
				if(x==4 || x==2 || x==5) {
					char *ss = memchr(curtext[OPTIONS_POS+x],'\0',60);
					int len = ss - curtext[OPTIONS_POS+x];
					char *buffer = (char*) malloc(len+5);

					strcpy(buffer,curtext[OPTIONS_POS+x]);

					if((x==4 && opt_rumble) || (x==2 && opt_hoverhint) || (x==5 && opt_widescreen)) {
						char *s2 = strchr(buffer,'/');
						if(s2!=NULL)
							buffer[s2-buffer]='\0';
					}
					else {
						char *s2=strchr(buffer,'/') + 1;
						char *s1=strrchr(buffer,' ') + 1;
						if(s1!=NULL && s2!=NULL)
							memmove(s1,s2,len-(s2-buffer)+1);
					}

					GRRLIB_GPrintf(320,opths[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),buffer);

					free(buffer);

				}
				else
					GRRLIB_GPrintf(320,opths[x][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[OPTIONS_POS+x]);
			}
			GRRLIB_GPrintf(600, opths[6][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[REMARK_POS]);

			break;
		case SOUND_MENU :
			GRRLIB_DrawImg(86,30,524,436, tex_menufore, 0, 1, 1, 255);

			for(x=0;x<2;x++) {
				GRRLIB_GPrintf(320,soundhs[x+3][1],0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[SOUND_POS+x]);
			}
			GRRLIB_GPrintf(600, soundhs[2][1],0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(msel==x),curtext[REMARK_POS]);

			// draw the offs
			GRRLIB_GPrintf(186,115,0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(false),curtext[SOUND_POS+2]);
			GRRLIB_GPrintf(186,224,0xFFFFFFFF,1,1, ALIGN_RIGHT,CUR_FONT(false),curtext[SOUND_POS+2]);

			// draw the fulls
			GRRLIB_GPrintf(454,115,0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(false),curtext[SOUND_POS+3]);
			GRRLIB_GPrintf(454,224,0xFFFFFFFF,1,1, ALIGN_LEFT,CUR_FONT(false),curtext[SOUND_POS+3]);

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
				int tx = x + (lang_page==0?0:9);
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

			if(lang_page==0) {
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
			GRRLIB_GPrintf(320,27,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(true),curtext[OPTIONS_POS+1]);

			for(x=0;x<6;x++) {
				GRRLIB_DrawImg(layouths[x][0],layouths[x][1]-10,180,208, tex_layout, 0, 1, 0.8, 255);
				drawLayout(x);
			}

			for(x=0;x<6;x++) {
				if(!(x==msel))
					GRRLIB_DrawImg(layouths[x][0]+4, layouths[x][1]-8, 168, 196,tex_shade, 0, 1, 0.8, 255);
					if(opt_lang==JAPANESE)
						GRRLIB_GPrintf(layouths[x][0]+92,layouths[x][1]+152,0xFFFFFFFF,1,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LAYOUT_POS+x]);
					else
						GRRLIB_GPrintf(layouths[x][0]+77,layouths[x][1]+152,0xFFFFFFFF,0.55,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[LAYOUT_POS+x]);
			}

			break;
		case TILESET_MENU :
			GRRLIB_GPrintf(320,27,0xFFFFFFFF,1,1, ALIGN_CENTRE,CUR_FONT(true),curtext[OPTIONS_POS]);

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
						GRRLIB_GPrintf(layouths[x][0]+92,layouths[x][1]+152,0xFFFFFFFF,1,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[TILESET_POS+x]);
					else
						GRRLIB_GPrintf(layouths[x][0]+77,layouths[x][1]+152,0xFFFFFFFF,0.55,0.8, ALIGN_CENTRE,CUR_FONT(msel==x),curtext[TILESET_POS+x]);
			}

			break;
	}
}

void drawLayout(int n) {
	unsigned char grid[MAX_WIDTH][MAX_HEIGHT][MAX_LAYERS];
	int x,y,z,c,l,pos=0;

	for(z=0;z<MAX_LAYERS;z++) {
		for(x=0;x<MAX_WIDTH;x++) {
			for(y=0;y<MAX_HEIGHT;y++) {
				grid[x][y][z]=0;
			}
		}
	}

	for(l=0;l<MAX_TILES;l++) {
		x=layouts[n][pos++];
		y=layouts[n][pos++];
		z=layouts[n][pos++];
		grid[x][y][z]=255;
	}

	for(z=0;z<MAX_LAYERS;z++) {
		for(x=0;x<MAX_WIDTH+MAX_HEIGHT;x++) {
			for(y=(x>=MAX_WIDTH?x-MAX_WIDTH:0),c=(x>=MAX_WIDTH?MAX_WIDTH-1:x);y<MAX_HEIGHT && c>=0;y++,c--) {
				if(grid[c][y][z]==255) {
					GRRLIB_DrawImg(layouths[n][0]+14+c*5-z,layouths[n][1]+34+y*7-z,12,16,tex_smalltile,0,1,1,255);
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
					case 0 :
						playClick();
						killMainMenu();
						initPlayMenu();
						break;
					case 1 :
						playWrong();
						// Hi Score
						break;
					case 2 :
						playClick();
						killMainMenu();
						initOptionMenu();
						break;
					case 3 :
						playClick();
						killMainMenu();
						initSoundMenu();
						break;
					case 4 :
						playClick();
						killMainMenu();
						return EXIT;
						break;
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
					int tmp_lang=msel + (lang_page==0?0:9);
					if(tmp_lang>NORWEGIAN) return NOTHING;
					opt_lang=tmp_lang;
					setLanguage();
					playClick();
					killLangMenu();
					initOptionMenu();
				}
				if(btnover) {
					lang_page=++lang_page%2;
					playClick();
				}
				break;
			case LAYOUT_MENU :
				if(msel>-1) {
					playClick();
					opt_layout=msel;
					killLayoutMenu();
					initOptionMenu();
				}
				break;
			case TILESET_MENU :
				if(msel>-1) {
					playClick();
					opt_tileset=msel;
					killTilesetMenu();
					initOptionMenu();
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
				playClick();
				killTilesetMenu();
				initOptionMenu();
				break;
		}
	}

	if(wpaddown & WPAD_BUTTON_PLUS) {
		if(curmenunum==LANG_MENU && lang_page==0) lang_page++;
	}

	if(wpaddown & WPAD_BUTTON_MINUS) {
		if(curmenunum==LANG_MENU && lang_page==1) lang_page--;
	}

	return NOTHING;
}

void checkSelected(WPADData *wd) {
	int x;
	switch(curmenunum) {
		case MAIN_MENU :
			for(x=0;x<5;x++) {
				if((wd->ir.x)>mainhs[x][0] && (wd->ir.x)<(mainhs[x][0]+mainhs[x][2]) && (wd->ir.y)>mainhs[x][1] && (wd->ir.y)<(mainhs[x][1]+mainhs[x][3])) {
					msel=x;
					return;
				}
			}
			break;
		case PLAY_MENU :
			for(x=0;x<4;x++) {
				if((wd->ir.x)>playhs[x][0] && (wd->ir.x)<(playhs[x][0]+playhs[x][2]) && (wd->ir.y)>playhs[x][1] && (wd->ir.y)<(playhs[x][1]+playhs[x][3])) {
					msel=x;
					return;
				}

				if(x<3 && (wd->ir.x)>playhs[x][0] && (wd->ir.x)<(playhs[x][0]+168) && (wd->ir.y)>90 && (wd->ir.y)<(286)) {
					msel=x;
					return;
				}
			}
			break;
		case SOUND_MENU :
			for(x=0;x<3;x++) {
				if((wd->ir.x)>soundhs[x][0] && (wd->ir.x)<(soundhs[x][0]+soundhs[x][2]-(x==2?1:48)) && (wd->ir.y)>soundhs[x][1] && (wd->ir.y)<(soundhs[x][1]+soundhs[x][3])) {
					msel=x;
					return;
				}
			}
			break;
		case OPTIONS_MENU :
			for(x=0;x<7;x++) {
				if((wd->ir.x)>opths[x][0] && (wd->ir.x)<(opths[x][0]+opths[x][2]) && (wd->ir.y)>opths[x][1] && (wd->ir.y)<(opths[x][1]+opths[x][3])) {
					msel=x;
					return;
				}
			}
			break;
		case LANG_MENU :
			for(x=0;x<9;x++) {
				if((wd->ir.x)>langhs[x][0] && (wd->ir.x)<(langhs[x][0]+148) && (wd->ir.y)>langhs[x][1] && (wd->ir.y)<(langhs[x][1]+108)) {
					msel=x;
					return;
				}
			}
			if(lang_page==0) {
				if((wd->ir.x)>580 && (wd->ir.x)<620 && (wd->ir.y)>220 && (wd->ir.y)<260)
					btnover=true;
				else
					btnover=false;
			}
			else {
				if((wd->ir.x)>10 && (wd->ir.x)<50 && (wd->ir.y)>220 && (wd->ir.y)<260)
					btnover=true;
				else
					btnover=false;
			}
			break;
		case LAYOUT_MENU :
			for(x=0;x<6;x++) {
				if((wd->ir.x)>layouths[x][0] && (wd->ir.x)<(layouths[x][0]+168) && (wd->ir.y)>layouths[x][1] && (wd->ir.y)<(layouths[x][1]+170)) {
					msel=x;
					return;
				}
			}
			break;
		case TILESET_MENU :
			for(x=0;x<6;x++) {
				if((wd->ir.x)>tileseths[x][0] && (wd->ir.x)<(tileseths[x][0]+168) && (wd->ir.y)>tileseths[x][1] && (wd->ir.y)<(tileseths[x][1]+170)) {
					msel=x;
					return;
				}
			}
			break;
	}
	msel=-1;
}

void initMainMenu() {
	int x,strsize;

	tex_menuback=GRRLIB_LoadJPG(mainback_jpg, mainback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(mainfore_png);

	tex_shanghi=GRRLIB_LoadTexture(shanghi_png);

	for(x=0;x<5;x++) {
		strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[x]);
		mainhs[x][0]=320-strsize/2;
		mainhs[x][2]=strsize;
	}

	curmenunum = MAIN_MENU;
}

void killMainMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_menufore) free(tex_menufore);

	if(tex_shanghi) free(tex_shanghi);
}


void initPlayMenu() {
	int x;

	tex_menuback=GRRLIB_LoadJPG(playback_jpg, playback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(playfore_png);

	tex_shade=GRRLIB_LoadTexture(shade_png);

	int strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[REMARK_POS]);
	playhs[3][0]=600-strsize;
	playhs[3][2]=strsize;

	// if we only have one line then half the selectable hight
	for(x=0;x<3;x++) {
		if(strcmp(curtext[PLAY_POS+1+x*2]," ")==0)
			playhs[x][3]/=2;
	}

	curmenunum = PLAY_MENU;
}

void killPlayMenu() {
	killMenu();

	if(tex_shade) free(tex_shade);
}

void initSoundMenu() {
	tex_menuback=GRRLIB_LoadJPG(soundback_jpg, soundback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(soundfore_png);

	tex_uball=GRRLIB_LoadTexture(unselectedball_png);
	tex_sball=GRRLIB_LoadTexture(selectedball_png);

	int strsize=GRRLIB_GetStringWidth(CUR_FONT(false),curtext[REMARK_POS]);
	soundhs[2][0]=600-strsize;
	soundhs[2][2]=strsize;

	curmenunum = SOUND_MENU;
}

void killSoundMenu() {
	killMenu();

	if(tex_uball) free(tex_uball);
	if(tex_sball) free(tex_sball);
}

void initOptionMenu() {
	int x,strsize;

	tex_menuback=GRRLIB_LoadJPG(optionsback_jpg, optionsback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(optionsfore_png);

	for(x=0;x<7;x++) {
		strsize=GRRLIB_GetStringWidth(CUR_FONT(false),x==6?curtext[REMARK_POS]:curtext[OPTIONS_POS+x]);
		if(x==6)
			opths[x][0]=600-strsize;
		else
			opths[x][0]=320-strsize/2;

		opths[x][2]=strsize;
	}

	curmenunum = OPTIONS_MENU;
}

void initLangMenu() {
	tex_menuback=GRRLIB_LoadJPG(mainback_jpg, mainback_jpg_size);
	tex_menufore=GRRLIB_LoadTexture(mainfore_png);
	tex_flags=GRRLIB_LoadTexture(flags_png);
	tex_languages=GRRLIB_LoadTexture(languages_png);
	tex_plus=GRRLIB_LoadTexture(HSplusButton_png);
	tex_minus=GRRLIB_LoadTexture(HSminusButton_png);

	curmenunum = LANG_MENU;
}

void killLangMenu() {
	killMenu();

	if(tex_flags) free(tex_flags);
	if(tex_languages) free(tex_languages);

	if(tex_plus) free(tex_plus);
	if(tex_minus) free(tex_minus);
}

void initLayoutMenu() {
	tex_menuback=GRRLIB_LoadJPG(playback_jpg, playback_jpg_size);
	tex_layout=GRRLIB_LoadTexture(layoutback_png);
	tex_smalltile=GRRLIB_LoadTexture(smalltile_png);
	tex_shade=GRRLIB_LoadTexture(shade_png);

	curmenunum = LAYOUT_MENU;
}

void killLayoutMenu() {
	if(tex_menuback) free(tex_menuback);
	if(tex_layout) free(tex_layout);
	if(tex_smalltile) free(tex_smalltile);
	if(tex_shade) free(tex_shade);
}

void initTilesetMenu() {
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

void killTilesetMenu() {
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

void playWrong() {
	SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 8000, 0,&gromb_raw, gromb_raw_size, opt_sound, opt_sound, NULL);
}

void playClick() {
	SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, 8000, 0,&Click17a_raw, Click17a_raw_size, opt_sound, opt_sound, NULL);
}
