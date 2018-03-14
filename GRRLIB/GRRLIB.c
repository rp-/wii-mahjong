/*===========================================
        GRRLIB (GX version) 3.0.1 alpha
        Code     : NoNameNo
        GX hints : RedShade
===========================================*/

#include "GRRLIB.h"

#define DEFAULT_FIFO_SIZE (256 * 1024)

#define ALIGN_LEFT 1
#define ALIGN_CENTRE 2
#define ALIGN_RIGHT 3

extern GXRModeObj TVEurgb60Hz480IntDf;

 u32 fb=0;
 u32 *xfb[2] = { NULL, NULL};
 GXRModeObj *rmode;
 void *gp_fifo = NULL;

typedef struct font {
	u8 *font_array;
	u16 char_width;
	u16 char_height;
	u8 chars_wide;
	u8 chars_high;
	char *char_map;
	u8 *widths;
} font;

font fonts[5];

/* inline */void GRRLIB_FillScreen(u32 color){
	GRRLIB_Rectangle(-40, -40, 680,520, color, 1);
}

inline void GRRLIB_Plot(f32 x,f32 y, u32 color){
   guVector  v[]={{x,y,0.0f}};
   GXColor c[]={GRRLIB_Splitu32(color)};

	GRRLIB_NPlot(v,c,1);
}
void GRRLIB_NPlot(guVector v[],GXColor c[],long n){
	GRRLIB_GXEngine(v,c,n,GX_POINTS);
}

inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color){
   guVector  v[]={{x1,y1,0.0f},{x2,y2,0.0f}};
   GXColor col = GRRLIB_Splitu32(color);
   GXColor c[]={col,col};

	GRRLIB_NGone(v,c,2);
}

inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled){
   guVector  v[]={{x,y,0.0f},{x+width,y,0.0f},{x+width,y+height,0.0f},{x,y+height,0.0f},{x,y,0.0f}};
   GXColor col = GRRLIB_Splitu32(color);
   GXColor c[]={col,col,col,col,col};

	if(!filled){
		GRRLIB_NGone(v,c,5);
	}
	else{
		GRRLIB_NGoneFilled(v,c,4);
	}
}
void GRRLIB_NGone(guVector v[],GXColor c[],long n){
	GRRLIB_GXEngine(v,c,n,GX_LINESTRIP);
}
void GRRLIB_NGoneFilled(guVector v[],GXColor c[],long n){
	GRRLIB_GXEngine(v,c,n,GX_TRIANGLEFAN);
}

/******************************************************************************/
/* LIBJPEG EXAMPLE START */

/* Convert a raw bmp (RGB, no alpha) to 4x4RGBA */
static void RawTo4x4RGBA(const unsigned char *src, void *dst, const unsigned int width, const unsigned int height) {
	unsigned int block = 0;
	unsigned int i = 0;
	unsigned int c = 0;
	unsigned int ar = 0;
	unsigned int gb = 0;
	unsigned char *p = (unsigned char*)dst;

	for (block = 0; block < height; block += 4) {
		for (i = 0; i < width; i += 4) {
			/* Alpha and Red */
			for (c = 0; c < 4; ++c) {
				for (ar = 0; ar < 4; ++ar) {
					/* Alpha pixels */
					*p++ = 255;
					/* Red pixels */
					*p++ = src[((i + ar) + ((block + c) * width)) * 3];
				}
			}

			/* Green and Blue */
			for (c = 0; c < 4; ++c) {
				for (gb = 0; gb < 4; ++gb) {
					/* Green pixels */
					*p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 1];
					/* Blue pixels */
					*p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 2];
				}
			}
		} /* i */
	} /* block */
}

/* NOTE: This loads a jpeg into a 4x4RGBA texture, and as such you wont
 * reduce your memory usage by using jpg instead of a png, but it is
 * still a useful function to have :) */
extern void *GRRLIB_LoadJPG(const unsigned char *src, unsigned int size) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* Init the JPEG decompressor */
	jpeg_create_decompress(&cinfo);

	/* Use the standard error handler */
	cinfo.err = jpeg_std_error(&jerr);

	/* Don't use a progress handler */
	cinfo.progress = NULL;

	/* Set the source buffer */
	// FIXME: jpeg_memory_src(&cinfo, src, size);
	jpeg_mem_src(&cinfo, (unsigned char*)src, size);

	/* Read the default header information */
	jpeg_read_header(&cinfo, TRUE);

	/* Get ready to decompress */
	jpeg_start_decompress(&cinfo);

	/* Create a buffer to hold the final image */
	unsigned char *tempBuffer = (unsigned char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
	if (tempBuffer == NULL) {
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	/* Decompress the JPEG into tempBuffer, one row at a time */
	JSAMPROW row_pointer[1];
	row_pointer[0] = (unsigned char*) malloc(cinfo.output_width * cinfo.num_components);
	unsigned int i = 0;
	size_t location = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i = 0; i < cinfo.image_width * cinfo.num_components; i++) {
			/* Put the decoded scanline into the tempBuffer */
			tempBuffer[ location++ ] = row_pointer[0][i];
		}
	}

	/* Create a buffer to hold the final texture */
	void *texture = memalign(32, cinfo.output_width * cinfo.output_height * 4);
	RawTo4x4RGBA(tempBuffer, texture, cinfo.output_width, cinfo.output_height);
	DCFlushRange(texture, cinfo.output_width * cinfo.output_height * 4);

	/* Done - do cleanup and release memory */
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	free(tempBuffer);

	return texture;
}

/* LIBJPEG EXAMPLE END */
/******************************************************************************/

u8 * GRRLIB_LoadTexture(const unsigned char my_png[]) {
   PNGUPROP imgProp;
   IMGCTX ctx;
   void *my_texture;
   int width, height;

   	ctx = PNGU_SelectImageFromBuffer(my_png);
	PNGU_GetImageProperties (ctx, &imgProp);
	my_texture = memalign (32, imgProp.imgWidth * imgProp.imgHeight * 4);
	// FIXME: PNGU_DecodeTo4x4RGBA8 (ctx, imgProp.imgWidth, imgProp.imgHeight, my_texture, 255);
	PNGU_DecodeTo4x4RGBA8 (ctx, imgProp.imgWidth, imgProp.imgHeight, &width, &height, my_texture);
	PNGU_ReleaseImageContext (ctx);
	DCFlushRange (my_texture, imgProp.imgWidth * imgProp.imgHeight * 4);
	return my_texture;
}

/* inline */ void GRRLIB_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u8 alpha ){
   GXTexObj texObj;

	GX_InitTexObj(&texObj, data, width,height, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
	//GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

	Mtx m,m1,m2, mv;
	width *=.5;
	height*=.5;
	guMtxIdentity (m1);
	guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
	guVector axis =(guVector) {0 , 0, 1 };
	guMtxRotAxisDeg (m2, &axis, degrees);
	guMtxConcat(m2,m1,m);

	guMtxTransApply(m,m, xpos+width,ypos+height,0);
	guMtxConcat (GXmodelView2D, m, mv);
	GX_LoadPosMtxImm (mv, GX_PNMTX0);

	GX_Begin(GX_QUADS, GX_VTXFMT0,4);
  	GX_Position3f32(-width, -height,  0);
  	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(0, 0);

  	GX_Position3f32(width, -height,  0);
 	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(1, 0);

  	GX_Position3f32(width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(1, 1);

  	GX_Position3f32(-width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(0, 1);
	GX_End();
	GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);

}

/* inline */ void GRRLIB_DrawColImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u32 col ){
   GXTexObj texObj;

    GXColor c = GRRLIB_Splitu32(col);

	GX_InitTexObj(&texObj, data, width,height, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
	//GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

	Mtx m,m1,m2, mv;
	width *=.5;
	height*=.5;
	guMtxIdentity (m1);
	guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
	guVector axis =(guVector) {0 , 0, 1 };
	guMtxRotAxisDeg (m2, &axis, degrees);
	guMtxConcat(m2,m1,m);

	guMtxTransApply(m,m, xpos+width,ypos+height,0);
	guMtxConcat (GXmodelView2D, m, mv);
	GX_LoadPosMtxImm (mv, GX_PNMTX0);

	GX_Begin(GX_QUADS, GX_VTXFMT0,4);
  	GX_Position3f32(-width, -height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(0, 0);

  	GX_Position3f32(width, -height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(1, 0);

  	GX_Position3f32(width, height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(1, 1);

  	GX_Position3f32(-width, height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(0, 1);
	GX_End();
	GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);

}

inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, u8 alpha, f32 frame,f32 maxframe ){
GXTexObj texObj;
f32 s1= frame/maxframe;
f32 s2= (frame+1)/maxframe;
f32 t1=0;
f32 t2=1;

	GX_InitTexObj(&texObj, data, width*maxframe,height, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
	GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

	Mtx m,m1,m2, mv;
	width *=.5;
	height*=.5;
	guMtxIdentity (m1);
	guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
	guVector axis =(guVector) {0 , 0, 1 };
	guMtxRotAxisDeg (m2, &axis, degrees);
	guMtxConcat(m2,m1,m);
	guMtxTransApply(m,m, xpos+width,ypos+height,0);
	guMtxConcat (GXmodelView2D, m, mv);
	GX_LoadPosMtxImm (mv, GX_PNMTX0);
	GX_Begin(GX_QUADS, GX_VTXFMT0,4);
  	GX_Position3f32(-width, -height,  0);
  	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(s1, t1);

  	GX_Position3f32(width, -height,  0);
 	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(s2, t1);

  	GX_Position3f32(width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(s2, t2);

  	GX_Position3f32(-width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
  	GX_TexCoord2f32(s1, t2);
	GX_End();
	GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);

}

/* inline */ void GRRLIB_DrawGTile(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], u16 chars_wide, u16 chars_high,float degrees, float scaleX, f32 scaleY, u32 col, u16 frame){
GXTexObj texObj;
f32 s1= ((frame    %chars_wide))    /(f32)chars_wide;
f32 s2= ((frame    %chars_wide)+1)    /(f32)chars_wide;
f32 t1= ((frame    /chars_wide))    /(f32)chars_high;
f32 t2= ((frame    /chars_wide)+1)    /(f32)chars_high;

    GXColor c = GRRLIB_Splitu32(col);

    GX_InitTexObj(&texObj, data, width*chars_wide,height*chars_high, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
    GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
      GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

    Mtx m,m1,m2,mv;
    width *=.5;
    height*=.5;

    guMtxIdentity (m1);
    guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
    guVector axis =(guVector) {0,0,1};
    guMtxRotAxisDeg (m2, &axis, degrees);
    guMtxConcat(m2,m1,m);
    guMtxTransApply(m,m, xpos+width,ypos+height,0);
    guMtxConcat (GXmodelView2D, m, mv);
    GX_LoadPosMtxImm (mv, GX_PNMTX0);

    GX_Begin(GX_QUADS, GX_VTXFMT0,4);

	GX_Position3f32(-width, -height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
	GX_TexCoord2f32(s1, t1);

	GX_Position3f32(width, -height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
	GX_TexCoord2f32(s2, t1);

	GX_Position3f32(width, height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
	GX_TexCoord2f32(s2, t2);

	GX_Position3f32(-width, height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
	GX_TexCoord2f32(s1, t2);

    GX_End();

    GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);
    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
      GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}

inline void GRRLIB_DrawChar(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], float degrees, float scaleX, f32 scaleY, f32 frame,f32 maxframe, GXColor c ){
GXTexObj texObj;
f32 s1= frame/maxframe;
f32 s2= (frame+1)/maxframe;
f32 t1=0;
f32 t2=1;

	GX_InitTexObj(&texObj, data, width*maxframe,height, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
	GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

	Mtx m,m1,m2, mv;
	width *=.5;
	height*=.5;
	guMtxIdentity (m1);
	guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
	guVector axis =(guVector) {0 , 0, 1 };
	guMtxRotAxisDeg (m2, &axis, degrees);
	guMtxConcat(m2,m1,m);
	guMtxTransApply(m,m, xpos+width,ypos+height,0);
	guMtxConcat (GXmodelView2D, m, mv);
	GX_LoadPosMtxImm (mv, GX_PNMTX0);
	GX_Begin(GX_QUADS, GX_VTXFMT0,4);
  	GX_Position3f32(-width, -height,  0);
  	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(s1, t1);

  	GX_Position3f32(width, -height,  0);
 	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(s2, t1);

  	GX_Position3f32(width, height,  0);
	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(s2, t2);

  	GX_Position3f32(-width, height,  0);
	GX_Color4u8(c.r,c.g,c.b,c.a);
  	GX_TexCoord2f32(s1, t2);
	GX_End();
	GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
  	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);

}

// This version adds ability to have a font with different number of characters wide and high instead of hardcoded to 16x16
// *** It may be worth looking at modifying the draw tile routine for this as we don't need to specify the colour here
inline void GRRLIB_DrawGChar(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], u16 chars_wide, u16 chars_high,float degrees, float scaleX, f32 scaleY, u8 frame, GXColor c ){
GXTexObj texObj;
f32 s1= ((frame    %chars_wide))    /(f32)chars_wide;
f32 s2= ((frame    %chars_wide)+1)    /(f32)chars_wide;
f32 t1= ((frame    /chars_wide))    /(f32)chars_high;
f32 t2= ((frame    /chars_wide)+1)    /(f32)chars_high;

    GX_InitTexObj(&texObj, data, width*chars_wide,height*chars_high, GX_TF_RGBA8,GX_CLAMP, GX_CLAMP,GX_FALSE);
    GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
      GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

    Mtx m,m1,m2,mv;
    width *=.5;
    height*=.5;

    guMtxIdentity (m1);
    guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
    guVector axis =(guVector) {0,0,1};
    guMtxRotAxisDeg (m2, &axis, degrees);
    guMtxConcat(m2,m1,m);
    guMtxTransApply(m,m, xpos+width,ypos+height,0);
    guMtxConcat (GXmodelView2D, m, mv);
    GX_LoadPosMtxImm (mv, GX_PNMTX0);

    GX_Begin(GX_QUADS, GX_VTXFMT0,4);

          GX_Position3f32(-width, -height,  0);
          GX_Color4u8(c.r,c.g,c.b,c.a);
          GX_TexCoord2f32(s1, t1);

          GX_Position3f32(width, -height,  0);
         GX_Color4u8(c.r,c.g,c.b,c.a);
          GX_TexCoord2f32(s2, t1);

          GX_Position3f32(width, height,  0);
        GX_Color4u8(c.r,c.g,c.b,c.a);
          GX_TexCoord2f32(s2, t2);

          GX_Position3f32(-width, height,  0);
        GX_Color4u8(c.r,c.g,c.b,c.a);
          GX_TexCoord2f32(s1, t2);

    GX_End();

    GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);
    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
      GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}


unsigned int convert2UTF8(char *txt) {
	if(txt[0]<0x80) {
		return (unsigned int) txt[0];
	}

	if((txt[0]&0xE0)==0xC0) {
		return (unsigned int) ((txt[0]&0x1F)<<6) + (txt[1]&0x3F);
	}

	if((txt[0]&0xF0)==0xE0) {
		return (unsigned int) ((txt[0]&0x0F)<<12) + ((txt[1]&0x3F)<<6) + (txt[2]&0x3F);
	}

	if((txt[0]&0xF8)==0xF0) {
		return (unsigned int) ((txt[0]&0x0E)<<18) + ((txt[1]&0x3F)<<12) + ((txt[2]&0x3F)<<6) + (txt[3]&0x3F);
	}

	return 0;
}

int getNumChars(char *ch) {
	if((ch[0]&0xF8)==0xF0) return 4;
	if((ch[0]&0xF0)==0xE0) return 3;
	if((ch[0]&0xE0)==0xC0) return 2;

	return 1;
}

int getUTF8CharPos(char *txt, int fnum) {
	int x,ch;

	unsigned int in = convert2UTF8(txt);

	for(x=0,ch=0;x<strlen(fonts[fnum].char_map);x+=getNumChars(&fonts[fnum].char_map[x]),ch++) {
		unsigned int out = convert2UTF8(&fonts[fnum].char_map[x]);

		if(in==out) return ch;
	}

	return -1;
}

/*
 *  UTF-8 version
	This version of GRRLIB_Printf takes in a fontmap along with the number of character wide and high of the the font.
	The fontmap is just a terminated string containing the characters that appear in the font in the position they
	appear. e.g. "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:-+!" I would advise this to part of the font file along with
	the number of characters wide and high.

	It will not output anything for a character that is not in present in the fontmap
 */
int GRRLIB_GPrintf(f32 xpos, f32 ypos, u32 color, f32 zoomx, f32 zoomy, u8 align, int fnum, const char *text,...) {
    int i,size=0;
    char tmp[1024];
	int offset=0,pix_size=0;

	if(text==NULL) return 0;

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

	// get the width in pixels
    for(i=0;i<size;i+=getNumChars(&tmp[i])) {
		int charn=getUTF8CharPos(&tmp[i], fnum);

		if(fonts[fnum].widths!=NULL) {
			if(charn!=-1) {
				pix_size+=fonts[fnum].widths[charn];
			}
			else {
				pix_size+=fonts[fnum].char_width/5;
			}
		}
		else {
			pix_size+=fonts[fnum].char_width;
		}
	}

	pix_size*=zoomx;

	// change the x position depending on the alignment
	switch(align) {
		case ALIGN_LEFT :
			break;
		case ALIGN_CENTRE :
			xpos-=(pix_size/2);
			break;
		case ALIGN_RIGHT :
			xpos-=pix_size;
			break;
	}

    GXColor col = GRRLIB_Splitu32(color);
    for(i=0;i<size;i+=getNumChars(&tmp[i])) {
		int charn=getUTF8CharPos(&tmp[i], fnum);

		if(charn!=-1) {
			GRRLIB_DrawGChar(xpos + offset * zoomx, ypos, fonts[fnum].char_width, fonts[fnum].char_height, fonts[fnum].font_array, fonts[fnum].chars_wide, fonts[fnum].chars_high, 0, zoomx, zoomy, charn, col );
		}

		if(fonts[fnum].widths!=NULL) {
			if(charn!=-1) {
				offset+=fonts[fnum].widths[charn]-1;
			}
			else {
				offset+=fonts[fnum].char_width/5;
			}
		}
		else {
			offset+=fonts[fnum].char_width;
		}
    }

	return pix_size;
}

void GRRLIB_SetFont(const u8 font[], const u16 font_width, const u16 font_height, const char fontmap[], const u8 chars_wide, const u8 chars_high, const u8 fontwidths[], const int fnum) {
	fonts[fnum].font_array=(u8 *)font;
	fonts[fnum].char_width=font_width;
	fonts[fnum].char_height=font_height;
	fonts[fnum].char_map=(char *)fontmap;
	fonts[fnum].chars_wide=chars_wide;
	fonts[fnum].chars_high=chars_high;
	if(fontwidths!=NULL) {
		fonts[fnum].widths=(u8 *) fontwidths;
	}
	else {
		fonts[fnum].widths=NULL;
	}
}

void GRRLIB_FreeFont(int fnum) {
	free(fonts[fnum].widths);
	free(fonts[fnum].char_map);
	free(fonts[fnum].font_array);
}

/*
 *	utf-8 get string width
 */
int GRRLIB_GetStringWidth(int fnum, const char *text,...) {
    int i,size=0,pix_size=0;
    char tmp[1024];

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

	// get the width in pixels
    for(i=0;i<size;i+=getNumChars(&tmp[i])) {
		int charn=getUTF8CharPos(&tmp[i], fnum);
		if(fonts[fnum].widths!=NULL) {
			if(charn!=-1) {
				pix_size+=fonts[fnum].widths[charn];
			}
			else {
				pix_size+=fonts[fnum].char_width/5;
			}
		}
		else {
			pix_size+=fonts[fnum].char_width;
		}
	}
	return pix_size;
}


void GRRLIB_Printf(f32 xpos, f32 ypos, u8 data[], u32 color, f32 zoom, const char *text,...){
    int i ;
    char tmp[1024];
    int size=0;
    u16 charHeight = 16, charWidth = 8;

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

    GXColor col = GRRLIB_Splitu32(color);
    for(i=0; i<size; i++){
        u8 c = tmp[i];
        GRRLIB_DrawChar(xpos+i*charWidth*zoom, ypos, charWidth, charHeight, data, 0, zoom, zoom, c, 128, col );
    }
}

void GRRLIB_GXEngine(guVector v[], GXColor c[], long n,u8 fmt){
   int i=0;

	GX_Begin(fmt, GX_VTXFMT0,n);
	for(i=0;i<n;i++){
  		GX_Position3f32(v[i].x, v[i].y,  v[i].z);
  		GX_Color4u8(c[i].r, c[i].g, c[i].b, c[i].a);
  	}
	GX_End();
}

GXColor GRRLIB_Splitu32(u32 color){
   u8 a,r,g,b;

	a = (color >> 24) & 0xFF;
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color) & 0xFF;

	return (GXColor){r,g,b,a};
}




//********************************************************************************************
void GRRLIB_InitVideo () {

    f32 yscale;
    u32 xfbHeight;
    Mtx perspective;

    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
//	if ((rmode->viTVMode >> 2) == VI_PAL) rmode = &TVEurgb60Hz480IntDf;

    xfb[0] = (u32 *)MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    xfb[1] = (u32 *)MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    VIDEO_Configure (rmode);

    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    fb ^= 1;

    gp_fifo = (u8 *) memalign(32,DEFAULT_FIFO_SIZE);

    memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);

    GX_Init (gp_fifo, DEFAULT_FIFO_SIZE);
    GXColor background = {0x00, 0x00, 0x00, 0xff};
    GX_SetCopyClear(background, 0x00ffffff);

    // other gx setup
    yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
    xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
    GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
    GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
    GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
    GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

    if (rmode->aa) {
        GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GX_SetCullMode(GX_CULL_NONE);
    GX_CopyDisp(xfb[0],GX_TRUE);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);

    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_InvVtxCache ();

    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);

    GX_SetNumTexGens(1);
    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    GX_InvalidateTexAll();

    guMtxIdentity(GXmodelView2D);
    guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -50.0F);
    GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

    guOrtho(perspective,0,479,0,639,0,300);
    GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetCullMode(GX_CULL_NONE);

}

void GRRLIB_Widescreen(bool wd_on) {
	// This is meant to fix widescreen TVs
	if (wd_on) {
		rmode->viWidth = 678;
		rmode->viXOrigin = (VI_MAX_WIDTH_PAL - 678) / 2;
	}
	else {
		rmode->viWidth = 640;
		rmode->viXOrigin = (VI_MAX_WIDTH_PAL - 640) / 2;
	}

    VIDEO_Configure (rmode);
}

void GRRLIB_Stop() {

    GX_AbortFrame();
    GX_Flush();

    free(MEM_K1_TO_K0(xfb[0])); xfb[0] = NULL;
    free(MEM_K1_TO_K0(xfb[1])); xfb[1] = NULL;
    free(gp_fifo); gp_fifo = NULL;

}

void GRRLIB_Render () {
    GX_DrawDone ();

	fb ^= 1;		// flip framebuffer
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(xfb[fb],GX_TRUE);
	VIDEO_SetNextFramebuffer(xfb[fb]);
 	VIDEO_Flush();
 	VIDEO_WaitVSync();

}

/**
 * Make a PNG screenshot on the SD card.
 * @param File Name of the file to write.
 * @return True if every thing worked, false otherwise.
 */
bool GRRLIB_ScrShot(const char* text,...)
{
    IMGCTX pngContext;
    int ErrorCode = -1;

    char file[1024];

    va_list argp;
    va_start(argp, text);
    vsprintf(file, text, argp);
    va_end(argp);

    if(fatInitDefault() && (pngContext = PNGU_SelectImageFromDevice(file)))
    {
        ErrorCode = PNGU_EncodeFromYCbYCr(pngContext, 640, 480, xfb[fb], 0);
        PNGU_ReleaseImageContext(pngContext);

/*        if(!fatUnmount(PI_INTERNAL_SD))
        {    // I can only hope it's better than nothing
            fatUnsafeUnmount(PI_INTERNAL_SD);
        }*/
    }
    return !ErrorCode;
}
