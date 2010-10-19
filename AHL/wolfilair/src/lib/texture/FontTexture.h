/*
 * FontTexture.h
 *
 *  Created on: 19 oct. 2010
 *      Author: Pierre
 */

#ifndef FONTTEXTURE_H_
#define FONTTEXTURE_H_

#include "../font/Font.h"

#include <string>

using namespace std;

namespace PS3 {

/**
 * A texture displaying a text written thanks to a font.
 */
class FontTexture {
private:
	Font *font;
	CellFontRenderer renderer;
	CellFontRenderSurface surface;

	GLuint texId;
	string displayString;
public:
	FontTexture(Font& f, string str);
	virtual ~FontTexture();

	/**
	 * Returns the unique id PSGL uses to register this texture.
	 */
	GLuint getTextureId();
private:
	void createRenderer();
	void createSurface();
};

}

#endif /* FONTTEXTURE_H_ */
