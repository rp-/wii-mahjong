#include <stdio.h>

#include "disk.h"

#define LINESIZE 128

bool loadConfig(const char* filename)
{
    FILE *fp;

    if((fp = fopen(filename,"rb"))==NULL) return false;

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


void saveConfig(const char* filename)
{
    FILE *fp;

    if((fp = fopen(filename,"wb"))==NULL) return;

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


int saveHighscore( const char* scoreFileName, unsigned long scores[])
{
    int retStatus = 1;
    int i = 0;
    char line[LINESIZE];

    FILE *fp;
    if( (fp = fopen(scoreFileName, "w+") ) != NULL && retStatus > 0) {
        for(i = 0; i < LAYOUTS * 2; i += 2) {
            snprintf( line, sizeof(line), "%d;%ld\n", i, scores[i]);
            fputs( line, fp);
            snprintf( line, sizeof(line), "%d;%ld\n", i + 1, scores[i + 1]);
            fputs( line, fp);
        }
    }
    else
        retStatus = -1;

    fclose( fp);

    return retStatus;
}


int loadHighscores( const char* scoreFileName, unsigned long scores[])
{
    char line[LINESIZE];
    int i = 0;
    unsigned long score;
    int retStatus = 1;
    int index;

    FILE* fp;
    if( (fp = fopen(scoreFileName, "r")) != NULL && retStatus > 0) {
        for(i = 0; i < LAYOUTS * 2; i += 2) {
            fgets( line, LINESIZE, fp);
            if( line != NULL) {
                sscanf( line, "%d;%lu", &index, &score);
                if( index >= 0 && index < LAYOUTS * 2)
                    scores[index] = score;
            }
            fgets( line, LINESIZE, fp);
            if( line != NULL) {
                sscanf( line, "%d;%lu", &index, &score);
                if( index >= 0 && index < LAYOUTS * 2)
                    scores[index] = score;
            }
        }
    }
    else
        retStatus = -1;
    fclose( fp);

    return retStatus;
}
