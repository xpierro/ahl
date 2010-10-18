/*
 * Font.cpp
 *
 *  Created on: 18 oct. 2010
 *      Author: Pierre
 */

#include "./Font.h"

#include "../GL.h"
#include <cell/sysmodule.h>
#include <string>

using namespace std;

namespace PS3 {

bool Font::fontInitialized = false;
const CellFontLibrary* Font::library = NULL;
int Font::uniqueId = 1;

Font::Font(string path, Console& c) {
	initFont(c);
	c.printf("ouv = %d\n", cellFontOpenFontFile(library, (uint8_t*) path.c_str(), 0, uniqueId, &font));
	uniqueId += 1;

	cellFontSetResolutionDpi(&font, 72, 72);
	cellFontSetScalePoint(&font, 24, 24);
	createRenderer(c);
}

Font::~Font() { }

static void fontCallback(const uint64_t status, const uint64_t p, void *u) {
	(void) p;
	(void) u;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Font::closeFonts();
	}
}

static void* fonts_malloc( void*obj, uint32_t size )
{
	obj=NULL;
	return malloc( size );
}
static void  fonts_free( void*obj, void*p )
{
	obj=NULL;
	free( p );
}
static void* fonts_realloc( void*obj, void* p, uint32_t size )
{
	obj=NULL;
	return realloc( p, size );
}
static void* fonts_calloc( void*obj, uint32_t numb, uint32_t blockSize )
{
	obj=NULL;
	return calloc( numb, blockSize );
}




void Font::initFont(Console& c) {
	if (!fontInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONT);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FREETYPE );
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONTFT);

		// 4MB cache
		uint32_t* fontCache = (uint32_t*) malloc(1024 * 1024 * 4);
		CellFontEntry* userFontEntrys =
			(CellFontEntry*) malloc(sizeof(CellFontEntry) * MAX_FONTS_LOADABLE);
		CellFontConfig config;
		config.FileCache.buffer = fontCache;
		config.FileCache.size = 4 * 1024 * 1024;
		config.userFontEntrys = userFontEntrys;
		config.userFontEntryMax = MAX_FONTS_LOADABLE;
		c.printf("libinit0 = %d\n", cellFontInit(&config));

		CellFontLibraryConfigFT ftConfig;

		CellFontLibraryConfigFT_initialize(&ftConfig);

		ftConfig.MemoryIF.Object  = NULL;
		ftConfig.MemoryIF.Malloc  = fonts_malloc;
		ftConfig.MemoryIF.Free    = fonts_free;
		ftConfig.MemoryIF.Realloc = fonts_realloc;
		ftConfig.MemoryIF.Calloc  = fonts_calloc;


		c.printf("libinit = %d\n", cellFontInitLibraryFreeType(&ftConfig, &library));

		GL::addUserCallback(fontCallback);
		fontInitialized = true;
	}
}

void Font::closeFonts() {

}

void Font::getTextDimension(string str, float& width, float& height) {
	CellFontHorizontalLayout layout;
	CellFontGlyphMetrics metrics;

	cellFontGetHorizontalLayout(&font, &layout);

	// Specify write position
	float x = 0.0f;
	float y = layout.baseLineY;  // Base line position (spacing from top line)
	int charsInLine = 0; // count
	int lineNb = 0;

	string::iterator it = str.begin();
	char c = *it;
	while (it != str.end()) {
		if (c == '\n') {
			charsInLine = 0;
		} else if (c >= ' ') {
			cellFontGetCharGlyphMetrics(&font, c, &metrics);

			if (charsInLine == 0) {
				//!\ may be Vertical or X bearing
				x = - metrics.Horizontal.bearingY; // Align first character of each line at the left side
				y += layout.lineHeight; // Line feed
				lineNb += 1;
			}
			x += metrics.Horizontal.advance;  // Add advance
			charsInLine += 1;
		}

		c = *(++it);
	}
	width  = x - metrics.Horizontal.advance + metrics.width;
	height = lineNb * layout.lineHeight;
}

void Font::createRenderer(Console& c) {
	CellFontRendererConfig config;
	CellFontRendererConfig_initialize(&config);
	CellFontRendererConfig_setAllocateBuffer(&config, 1024 * 1024, 0);

	// Create renderer
	c.printf("Erreur create renderer = %d\n", cellFontCreateRenderer(library, &config, &renderer));

	// Bind the font with the renderer
	c.printf("Erreur Bind renderer = %d\n", cellFontBindRenderer(&font, &renderer));

	c.printf("Erreur scale pixel = %d\n", cellFontSetupRenderScalePixel(&font, 24.f, 24.f));

	void* buffer = malloc(1024 * 1024);
	cellFontRenderSurfaceInit(&surface, buffer, 4 * 100, 4, 100, 100);
	cellFontRenderSurfaceSetScissor(&surface, 0, 0, 100, 100);
}

GLuint Font::renderChar(char c, Console& cc) {
	(void) c;
	CellFontGlyphMetrics metrics;
	CellFontImageTransInfo transInfo;
	cc.printf("Erreyr glyph = %d\n", cellFontRenderCharGlyphImage(&font, 'A', &surface, 0, 0, &metrics, &transInfo));
	// Alpha copy with fixed color
	uint32_t* tex = (uint32_t*) transInfo.Surface;
	uint8_t* img = transInfo.Image;
	uint32_t color = 0xffffff00;
	cc.printf("w = %d | h = %d | trans = %d\n", transInfo.imageWidth, transInfo.imageHeight, transInfo.surfWidthByte);

	for (unsigned int v=0; v < transInfo.imageHeight; v++ ) {
		for (unsigned int u=0; u < transInfo.imageWidth; u++ ) { //!!!!!!
			if ( (tex[u]&0x000000ff)<img[u] ){
				tex[u] = img[u]|color;
			}
		}
		tex += transInfo.surfWidthByte/4;
		img += transInfo.imageWidthByte;
	}
	GLuint i;
	glGenTextures(1, &i);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Removes the one-pixel line that appears sometimes around textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return i;
}

}
