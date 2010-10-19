/*
 * FontTexture.cpp
 *
 *  Created on: 19 oct. 2010
 *      Author: Pierre
 */

#include "FontTexture.h"

namespace PS3 {

FontTexture::FontTexture(Font& f, string str) {
	font = &f;
	displayString = str;
	createRenderer();
	createSurface();
}

FontTexture::~FontTexture() { }

void FontTexture::createRenderer() {
	CellFontRendererConfig config;
	CellFontRendererConfig_initialize(&config);
	CellFontRendererConfig_setAllocateBuffer(&config, 1024 * 1024, 0);

	// Create renderer
	cellFontCreateRenderer(font->getLibrary(), &config, &renderer);

	// Bind the font with the renderer
	cellFontBindRenderer(font->getFontDescriptor(), &renderer);
	// Doesn't rescale: use previous values.
	cellFontSetupRenderScalePixel(font->getFontDescriptor(), 0.0f, 0.0f);

	// Creates the surface data structure, holding a buffer.
	void* buffer = malloc(1024 * 1024);
	cellFontRenderSurfaceInit(&surface, buffer, 4 * 100, 4, 100, 100);
	cellFontRenderSurfaceSetScissor(&surface, 0, 0, 100, 100);
}

void FontTexture::createSurface() {
	CellFontGlyphMetrics metrics;
	CellFontImageTransInfo transInfo;

	int currentX = 0;
	for (int index = 0; index < displayString.length(); index++) {
		cellFontRenderCharGlyphImage(font->getFontDescriptor(),
									 displayString.at(index),
									 &surface, 0, 0, &metrics, &transInfo);
		// Alpha copy with fixed color
		uint32_t* tex = (uint32_t*) surface.buffer + 99 * 100;
		uint8_t* img = transInfo.Image;
		uint32_t color = 0xffffff00;

		for (unsigned int v=0; v < transInfo.imageHeight; v++ ) {
			for (unsigned int u = 0; u < transInfo.imageWidth; u++ ) { //!!!!!!
				if ( (tex[u + currentX]&0x000000ff) < img[u] ){
					tex[u + currentX] = img[u]|color;
				}
			}
			tex -= transInfo.surfWidthByte/4; // TEX IS MODIFIED...
			img += transInfo.imageWidthByte;
		}
		currentX += transInfo.imageWidth;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, surface.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Removes the one-pixel line that appears sometimes around textures.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

GLuint FontTexture::getTextureId() {
	return texId;
}

}
