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
#include "../graphic/GL.h"
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

	int fontId;

public:
	static const int MAX_FONTS_LOADABLE = 10;

	Font();
	virtual ~Font();

	/**
	 * Initializes the font library
	 */
	static void initFont();

	/**
	 * Shuts the font library down
	 */
	static void closeFonts();

	CellFont* getFontDescriptor();
	const CellFontLibrary* getLibrary() const;
protected:
	virtual void load() = 0;
	virtual void unload() = 0;

	int getFontId() const;
};

}

#endif /* FONT_H_ */
