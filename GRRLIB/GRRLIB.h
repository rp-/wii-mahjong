/*===========================================
        GRRLIB (GX version) 3.0.1 alpha
        Code     : NoNameNo
        GX hints : RedShade
===========================================*/

#ifndef __GXHDR__
#define __GXHDR__
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <fat.h>

#include "../lib/libpng/pngu/pngu.h"
#undef boolean
#include "../lib/libjpeg/jpeglib.h"

#define ALIGN_LEFT 1
#define ALIGN_CENTRE 2
#define ALIGN_RIGHT 3

extern Mtx GXmodelView2D;

inline void GRRLIB_FillScreen(u32 color);

inline void GRRLIB_Plot(f32 x,f32 y, u32 color);
void GRRLIB_NPlot(Vector v[],GXColor c[],long n);

inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color);

inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled);
void GRRLIB_NGone(Vector v[],GXColor c[],long n);
void GRRLIB_NGoneFilled(Vector v[],GXColor c[],long n);

/* LIBJPEG EXAMPLE START */
extern void *GRRLIB_LoadJPG(const unsigned char *src, unsigned int size);
/* LIBJPEG EXAMPLE END */

u8 * GRRLIB_LoadTexture(const unsigned char my_png[]);
inline void GRRLIB_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u8 alphaF );
inline void GRRLIB_DrawColImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u32 c );


inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u8 alpha, f32 frame,f32 maxframe );
inline void GRRLIB_DrawGTile(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], u16 chars_wide, u16 chars_high,float degrees, float scaleX, f32 scaleY, u32 col, u16 frame);

inline void GRRLIB_DrawChar(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, f32 frame,f32 maxframe, GXColor c );

inline void GRRLIB_DrawGChar(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], u16 chars_wide, u16 chars_high,float degrees, float scaleX, f32 scaleY, u8 frame, GXColor c);

//void GRRLIB_GPrintf(f32 xpos, f32 ypos, u8 font[], u16 font_width, u16 font_height, char *fontmap, u16 chars_wide, u16 chars_high, u32 color, f32 zoom, char *text,...);
int GRRLIB_GPrintf(f32 xpos, f32 ypos, u32 color, f32 zoomx, f32 zoomy, u8 align, int fnum, const char *text,...);

void GRRLIB_SetFont(const u8 font[], const u16 font_width, const u16 font_height, const char fontmap[], const u8 chars_wide, const u8 chars_high, const u8 fontwidths[], const int fnum);

void GRRLIB_FreeFont(int fnum);

int GRRLIB_GetStringWidth(int fnum, const char *text,...);

void GRRLIB_Printf(f32 xpos, f32 ypos, u8 data[], u32 color, f32 zoom, const char* text,...);

GXColor GRRLIB_Splitu32(u32 color);

void GRRLIB_GXEngine(Vector v[], GXColor c[], long count,u8 fmt);

void GRRLIB_InitVideo ();

void GRRLIB_Widescreen(bool wd_on);

void GRRLIB_Stop();
void GRRLIB_Render ();

bool GRRLIB_ScrShot(const char *text,...);

#endif
