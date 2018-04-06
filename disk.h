#ifndef DISK_H_INCLUDED
#define DISK_H_INCLUDED

#include <gctypes.h>

#include "commons.h"

bool loadConfig(const char* filename);
void saveConfig(const char* filename);

/**
 * Save highscores into file.
 * @param scoreFileName file name to save the scores.
 * @param layoutIndex Index number of the layout for which the score is.
 * @param score Score that was reached and should be saved.
 * @return 1 if save was successfull.
 **/
int saveHighscore( const char* scoreFileName, unsigned long scores[]);

/**
 * load Highscores from file.
 * @param scoreFileName file name to load.
 * @param scores Array where the score will get stored.
 * @return 1 load was successfull.
 **/
int loadHighscores( const char* scoreFileName, unsigned long scores[]);
#endif                           // DISK_H_INCLUDED
