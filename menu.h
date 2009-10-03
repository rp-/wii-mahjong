#include <wiiuse/wpad.h>

void initMenu();

void setLanguage();

void killMenu();

void killMenuOther();

void killMenuLanguages();

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

void initHighscoreMenu();

void killHighscoreMenu();

void playWrong();

void playClick();
