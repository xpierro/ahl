/*
 * PNG.h
 *
 *  Created on: 16 oct. 2010
 *      Author: Pierre
 */

#ifndef PNG_H_
#define PNG_H_

#include "../GL.h"

#include <cell/codec.h>
#include <cell/sysmodule.h>
#include <string>
using namespace std;

namespace PS3 {

/**
 * Represents a PNG located on the hard drive, and installed in memory, ready
 * to be applied as a texture.
 */
class PNG {
private:
	static bool pngInitialized;
	bool loaded;
	GLuint texId;
	string pngPath;

	int width;
	int height;
	int bitDepth;
	CellPngDecColorSpace colorSpace;

	uint8_t* buffer;
public:
	PNG(string path);
	virtual ~PNG();

	static void initPngDec();
	static void closePngDec();

	/**
	 * Load the png if it isn't loaded already and returns the texture id
	 */
	GLuint getTextureId();
private:
	void loadFromDisc();
	//void drawTexCoordArray();
	void convertToTexture();
};

}

#endif /* PNG_H_ */
