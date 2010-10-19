/*
 * FontTexture.h
 *
 *  Created on: 19 oct. 2010
 *      Author: Pierre
 */

#ifndef FONTTEXTURE_H_
#define FONTTEXTURE_H_

namespace PS3 {

/**
 * A texture displaying a text written thanks to a font.
 */
class FontTexture {
public:
	FontTexture(Font& f);
	virtual ~FontTexture();

	/**
	 * Returns the unique id PSGL uses to register this texture.
	 */
	GLuint getTextureId();
};

}

#endif /* FONTTEXTURE_H_ */
