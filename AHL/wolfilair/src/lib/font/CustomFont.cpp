/*
 * CustomFont.cpp
 *
 *  Created on: 19 oct. 2010
 *      Author: Pierre
 */

#include "CustomFont.h"

#include <cell/sysmodule.h>

namespace PS3 {

CustomFont::CustomFont(string path) : Font() {
	fontPath = path;
	load();
}

CustomFont::~CustomFont() { }

void CustomFont::load() {
	cellFontOpenFontFile(getLibrary(), (uint8_t*) fontPath.c_str(), 0,
						 getFontId(), getFontDescriptor());
	cellFontSetResolutionDpi(getFontDescriptor(), 72, 72);
	cellFontSetScalePoint(getFontDescriptor(), 60, 60);
}

void CustomFont::unload() { }

}

/*
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
}*/
