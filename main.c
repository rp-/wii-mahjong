/*===========================================
		Mahjongg Solitaire Wii
		Code     : Justin Wood
 ============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <unistd.h>

#include "asndlib.h"       // sound library

#include "gcmodplay.h" // modplayer

#include "GRRLIB/GRRLIB.h"

#include "commons.h"

#include "menu.h"
#include "game.h"

#include "sushi_mod.h"
#include "dojo_dan_oriental_mod.h"
#include "graveyard_mod.h"
#include "egypt_crap_mod.h"
#include "childhood_mod.h"
#include "nebulos_mod.h"

#include "bigmenuback_jpg.h"
#include "credits_png.h"

#include "handpointerred_png.h"
#include "handpointergreen_png.h"

Mtx GXmodelView2D;
static GXRModeObj *rmode = NULL;

static MODPlay mod_track;

#define STARTUP 2
#define GAME 1
#define MENU 0

int main_mode=STARTUP;
int alpha=0;//,xm=64,xs=255;

bool opt_rumble=true, opt_hoverhint=false, opt_widescreen=false;
int opt_sound=255, opt_music=64, opt_layout=0, opt_lang=-1, opt_tileset=0;

int evctr = 0;

u8 *layouts[6];

static int languages[] = {JAPANESE,ENGLISH,GERMAN,FRENCH,SPANISH,ITALIAN,DUTCH};

bool loadConfig() {
	if(!fatInitDefault()) return false;

	FILE *fp;

	if((fp = fopen("sd:/mahjongg.cfg","rb"))==NULL) return false;

	// get the version number in case it older than current
	int version=getc(fp);

	opt_music=getc(fp);
	opt_sound=getc(fp);
	opt_layout=getc(fp);
	opt_lang=getc(fp);
	opt_rumble=getc(fp)==1?true:false;
	opt_hoverhint=getc(fp)==1?true:false;
	if(version>7) {
		opt_widescreen=getc(fp)==1?true:false;
		opt_tileset=getc(fp);
	}
	else {
		opt_widescreen=false;
		opt_tileset=0;
	}

	fclose(fp);
	
	return true;
}

void saveConfig() {
	if(!fatInitDefault()) return;
 
	FILE *fp;

	if((fp = fopen("sd:/mahjongg.cfg","wb"))==NULL) return;
	
	// first write the version number so we can identify this on load in case it changes
	putc(8,fp);

	putc(opt_music, fp);
	putc(opt_sound, fp);
	putc(opt_layout, fp);
	putc(opt_lang, fp);
	putc(opt_rumble?1:0, fp);
	putc(opt_hoverhint?1:0, fp);
	putc(opt_widescreen?1:0, fp);
	putc(opt_tileset, fp);

	fclose(fp);
}

void processMenuOption(int menuopt) {
	switch(menuopt) {
		case EXIT :
			killMenuLanguages();
			MODPlay_Unload (&mod_track);
			WPAD_Shutdown();
			SND_End();
			GRRLIB_Stop();
			saveConfig();
			// if we have been launched from a channel then reset to menu
			if (!*((u32*) 0x80001800)) {
				WII_Initialize();
				WII_ReturnToMenu();
			}
			else { // else the homebrew is launched from a loader so exit back to the loader
				exit(0);
			}
			break;
		case ONE_PLAYER_GAME :
		case TWO_PLAYER_COOP :
		case TWO_PLAYER_VERSUS :
			main_mode=GAME;
			initGame(menuopt);
			break;
	}
	if(menuopt>=MUSIC_BAR && menuopt<=(MUSIC_BAR+64)) {
		MODPlay_SetVolume( &mod_track, opt_music, opt_music);
	}
}

void countevs(int chan, const WPADData *data) {
	evctr++;
}

void setupLayouts() {
	layouts[0]=(u8 *)default_mtl;
	layouts[1]=(u8 *)cross_mtl;
	layouts[2]=(u8 *)butterfly_mtl;
	layouts[3]=(u8 *)fortress_mtl;
	layouts[4]=(u8 *)crab_mtl;
	layouts[5]=(u8 *)spider_mtl;
}

u8 HWButton = 0;

/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}
 
/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF_STANDBY;
}
 
/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
}


int main(){
   
	u8 *tex_ptrone=GRRLIB_LoadTexture(handpointerred_png);
	u8 *tex_ptrtwo=GRRLIB_LoadTexture(handpointergreen_png);
	u8 *tex_back=GRRLIB_LoadJPG(bigmenuback_jpg, bigmenuback_jpg_size);
	u8 *tex_fore=GRRLIB_LoadTexture(credits_png);

	GRRLIB_InitVideo();
	WPAD_Init();

	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);

	rmode = VIDEO_GetPreferredMode(NULL);

	SND_Init(INIT_RATE_48000); // Initialize the Sound Lib

	MODPlay_Init(&mod_track);

    SND_Pause(0); // the sound loop is running now

	if (MODPlay_SetMOD (&mod_track, dojo_dan_oriental_mod) < 0 ) // set the MOD song
      {
        MODPlay_Unload (&mod_track);   
      }
	else  
	  {
		// set the music volume to the minimum so we don't hear the music before saved volume is known
	    MODPlay_SetVolume( &mod_track, 0,0);
	    MODPlay_Start (&mod_track); // Play the MOD
	  }

	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

	WPAD_SetVRes(WPAD_CHAN_ALL, rmode->fbWidth, rmode->xfbHeight);
	
	// try to load a saved config, if none then get Wii language menu
	if(!loadConfig()) {
		// get the language of the Wii menu and map this to the Mahjongg Wii languages
		if(CONF_GetLanguage()<7) opt_lang = languages[CONF_GetLanguage()];
	}

	MODPlay_SetVolume( &mod_track, opt_music, opt_music);
	
	// setup the layouts array for use in menu and game
	setupLayouts();

	while(1) {
		WPAD_ScanPads();
		u32 wpad_one_down = WPAD_ButtonsDown(0);
		u32 wpad_two_down = WPAD_ButtonsDown(1);

		u32 type;
		WPADData *wd_one, *wd_two;
		WPAD_ReadPending(WPAD_CHAN_ALL, countevs);
		WPAD_Probe(WPAD_CHAN_ALL, &type);

		wd_one = WPAD_Data(0);
		wd_two = WPAD_Data(1);
		
		switch(main_mode) {
			case STARTUP :
				GRRLIB_FillScreen(0xFF000000);
				GRRLIB_DrawImg(0, 0, 640, 480, tex_back, 0, 1, 1, alpha>255?255:alpha);
				GRRLIB_DrawImg(68,308,212,48, tex_fore, 0, 1, 1, alpha>255?255:alpha);
				if(alpha++>394) {
					main_mode=MENU;
					if(tex_back) free(tex_back);
					if(tex_fore) free(tex_fore);
					initMenu();
				}
				break;
			case MENU :
				GRRLIB_FillScreen(0xFF000000);
				int menuopt = menuWiimote(wd_one,wpad_one_down);
				if(menuopt==EXIT) {
					if(tex_ptrone) free(tex_ptrone);
					if(tex_ptrtwo) free(tex_ptrtwo);
				}

				if(menuopt>NOTHING) {
					processMenuOption(menuopt);
					if(main_mode==GAME) {
						MODPlay_Unload (&mod_track);
						if(opt_tileset==SPOOKY)
							MODPlay_SetMOD (&mod_track, graveyard_mod);
						else
						if(opt_tileset==EGYPTIAN)
							MODPlay_SetMOD (&mod_track, egypt_crap_mod);
						else
						if(opt_tileset==SIMPLE)
							MODPlay_SetMOD (&mod_track, childhood_mod);
						else
						if(opt_tileset==SPACE)
							MODPlay_SetMOD (&mod_track, nebulos_mod);
						else
							MODPlay_SetMOD (&mod_track, sushi_mod);						
						MODPlay_SetVolume( &mod_track, opt_music, opt_music);
						MODPlay_Start (&mod_track);
					}
					else
						drawMenu(wd_one);
				}
				else
					drawMenu(wd_one);
				break;
			case GAME :
				if(gameWiimote(wd_one,wpad_one_down,wd_two,wpad_two_down)) {
					// we are exiting the game back to the menu
					main_mode=MENU;
					MODPlay_Unload (&mod_track);
					MODPlay_SetMOD (&mod_track, dojo_dan_oriental_mod);
					MODPlay_SetVolume( &mod_track, opt_music, opt_music);
					MODPlay_Start (&mod_track);
					killGame();
					initMenu();
				}
				else {
					drawGame();
				}
				break;
		}
		
		// alternate which pointer is on top every frame to not give the advantage to player one in two player mode
		static int ticktock=0;
		
		ticktock++;
		
		if(wd_two->ir.valid && ticktock%2==0) {
			if(main_mode==GAME && whatGameMode()==ONE_PLAYER_GAME) {
				// don't display second pointer in one player mode
			}
			else
				GRRLIB_DrawColImg(wd_two->ir.x - 9,wd_two->ir.y - 7,68,80,tex_ptrtwo,0,1,1,0xEEFFFFFF);
		}
		
		if(wd_one->ir.valid) {
			GRRLIB_DrawColImg(wd_one->ir.x - 9,wd_one->ir.y - 7,68,80,tex_ptrone,0,1,1,0xEEFFFFFF);
		}

		if(wd_two->ir.valid && ticktock%2!=0) {
			if(main_mode==GAME && whatGameMode()==ONE_PLAYER_GAME) {
				// don't display second pointer in one player mode
			}
			else
				GRRLIB_DrawColImg(wd_two->ir.x - 9,wd_two->ir.y - 7,68,80,tex_ptrtwo,0,1,1,0xEEFFFFFF);
		}
		
		if(wd_one->btns_h & WPAD_BUTTON_1) {
			GRRLIB_ScrShot("MahjonggWii_Screen_%y%m%d_%H%M%S.png",time(NULL));
		}

		if(HWButton)
			break;
		
		GRRLIB_Render();
	}

	if(HWButton) {
		killGame();
		killMenuLanguages();
		MODPlay_Unload (&mod_track);
		WPAD_Shutdown();
		SND_End();
		GRRLIB_Stop();
		saveConfig();
		SYS_ResetSystem(HWButton, 0, 0);
	}

	return 0;
}