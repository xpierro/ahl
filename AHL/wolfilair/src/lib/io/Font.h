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
	static const CellFontLibrary* library;
	static int uniqueId;

	CellFont font;
	CellFontRenderer renderer;
	CellFontRenderSurface surface;

public:
	static const int MAX_FONTS_LOADABLE = 10;

	/**
	 * Creates a new font object from a TTF font file.
	 */
	Font(string path, Console& c);
	virtual ~Font();

	static void initFont(Console &c);
	static void closeFonts();

	void getTextDimension(string str, float& width, float& height);
	void createRenderer(Console& c);
	GLuint renderChar(char c, Console& c);
};

}

#endif /* FONT_H_ */
