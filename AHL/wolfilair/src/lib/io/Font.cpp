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
#include <fstream>
#include <sys/paths.h>

using namespace std;

namespace PS3 {

bool Font::fontInitialized = false;
const CellFontLibrary* Font::library = NULL;
int Font::uniqueId = 1;

Font::Font(string path) {
	initFont();
	cellFontOpenFontFile(library, (uint8_t*) path.c_str(), 0, uniqueId, &font);
	uniqueId += 1;

	cellFontSetResolutionDpi(&font, 72, 72);
	cellFontSetScalePoint(&font, 60, 60);
	createRenderer();
}

Font::~Font() { }

static void fontCallback(const uint64_t status, const uint64_t p, void *u) {
	(void) p;
	(void) u;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Font::closeFonts();
	}
}

static void* fontMalloc(void* object, unsigned int size) {
	(void) object;
	return malloc(size);
}

static void fontFree(void* object, void* ptr) {
	(void) object;
	free(ptr);
}

static void* fontRealloc(void* object, void* ptr, unsigned int newSize) {
	(void) object;
	return realloc(ptr, newSize);
}
static void* fontCalloc(void* object, unsigned int elementNumber,
									  unsigned int elementSize ) {
	(void) object;
	return calloc(elementNumber, elementSize);
}




void Font::initFont() {
	if (!fontInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONT);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FREETYPE );
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONTFT);

		// 4MB cache used to speed up Font operations
		int cacheSize = 4 * 1024 * 1024;
		void* fontCache = malloc(cacheSize);
		// Array of font description
		CellFontEntry* userFontEntrys =
			(CellFontEntry*) malloc(sizeof(CellFontEntry) * MAX_FONTS_LOADABLE);
		CellFontConfig config;
		config.FileCache.buffer = (unsigned int*) fontCache;
		config.FileCache.size = cacheSize;
		config.userFontEntrys = userFontEntrys;
		config.userFontEntryMax = MAX_FONTS_LOADABLE;
		cellFontInit(&config);

		// Now we configure the freetype part.
		CellFontLibraryConfigFT freetypeConfig;
		CellFontLibraryConfigFT_initialize(&freetypeConfig);
		freetypeConfig.MemoryIF.Object  = NULL; // First arg of the functions
		freetypeConfig.MemoryIF.Malloc  = fontMalloc;
		freetypeConfig.MemoryIF.Free    = fontFree;
		freetypeConfig.MemoryIF.Realloc = fontRealloc;
		freetypeConfig.MemoryIF.Calloc  = fontCalloc;
		cellFontInitLibraryFreeType(&freetypeConfig, &library);

		GL::addUserCallback(fontCallback);
		fontInitialized = true;
	}
}

void Font::closeFonts() {

}

void Font::getTextDimensions(string str, float& width, float& height) {
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

void Font::createRenderer() {
	CellFontRendererConfig config;
	CellFontRendererConfig_initialize(&config);
	CellFontRendererConfig_setAllocateBuffer(&config, 1024 * 1024, 0);

	// Create renderer
	cellFontCreateRenderer(library, &config, &renderer);

	// Bind the font with the renderer
	cellFontBindRenderer(&font, &renderer);
	// Doesn't rescale: use previous values.
	cellFontSetupRenderScalePixel(&font, 0.0f, 0.0f);

	// Creates the surface data structure, holding a buffer.
	void* buffer = malloc(1024 * 1024);
	cellFontRenderSurfaceInit(&surface, buffer, 4 * 100, 4, 100, 100);
	cellFontRenderSurfaceSetScissor(&surface, 0, 0, 100, 100);
}

GLuint Font::renderChar(char c) {
	(void) c;
	CellFontGlyphMetrics metrics;
	CellFontImageTransInfo transInfo;
	char str[4];
	strcpy(str, "Hey");
	int currentX = 0;
	for (int index = 0; index < 4; index++) {
		cellFontRenderCharGlyphImage(&font, str[index], &surface, 0, 0, &metrics, &transInfo);
		// Alpha copy with fixed color
		uint32_t* tex = (uint32_t*) surface.buffer;
		uint8_t* img = transInfo.Image;
		uint32_t color = 0xffffff00;

		for (unsigned int v=0; v < transInfo.imageHeight; v++ ) {
			for (unsigned int u = 0; u < transInfo.imageWidth; u++ ) { //!!!!!!
				if ( (tex[u + currentX]&0x000000ff) < img[u] ){
					tex[u + currentX] = img[u]|color;
				}
			}
			tex += transInfo.surfWidthByte/4; // TEX IS MODIFIED...
			img += transInfo.imageWidthByte;
		}
		currentX += transInfo.imageWidth;
	}

	GLuint i;
	glGenTextures(1, &i);
	glBindTexture(GL_TEXTURE_2D, i);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Removes the one-pixel line that appears sometimes around textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return i;
}

}
