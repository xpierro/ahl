/*
 * Font.h
 *
 *  Created on: 18 oct. 2010
 *      Author: Pierre
 */

#ifndef FONT_H_
#define FONT_H_

#include <string>
#include <cell/font.h>
#include <cell/fontFT.h>
#include "../GL.h"
#include "Console.h"

using namespace std;

namespace PS3 {

class Font {
private:
	static bool fontInitialized;
	/**
	 * Freetype library informations.
	 */
	static const CellFontLibrary* library;
	/**
	 * Id used to describes the font, must be unique for each font loaded.
	 */
	static int uniqueId;

	/**
	 * The current font.
	 */
	CellFont font;

	/**
	 * Tool that can render a glyph to a surface.
	 */
	CellFontRenderer renderer;

	/**
	 * Surface bearing several glyphs printed on it thanks to the renderer
	 */
	CellFontRenderSurface surface;

public:
	static const int MAX_FONTS_LOADABLE = 10;

	/**
	 * Creates a new font object from a TTF font file.
	 */
	Font(string path);
	virtual ~Font();

	/**
	 * Initializes the font library
	 */
	static void initFont();

	/**
	 * Shut the font library down
	 */
	static void closeFonts();

	/**
	 * Return the text dimension in pixels for a string.
	 */
	void getTextDimensions(string str, float& width, float& height);

	/**
	 * Renter a char to a texture.
	 */
	GLuint renderChar(char c);
private:
	void createRenderer();
};

}

#endif /* FONT_H_ */
