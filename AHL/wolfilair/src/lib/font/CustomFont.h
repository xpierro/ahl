/*
 * CustomFont.h
 *
 *  Created on: 19 oct. 2010
 *      Author: Pierre
 */

#ifndef CUSTOMFONT_H_
#define CUSTOMFONT_H_

#include "Font.h"

namespace PS3 {

class CustomFont : public Font {
private:
	string fontPath;
public:
	CustomFont(string path);
	virtual ~CustomFont();
protected:
	virtual void load();
	virtual void unload();
};

}

#endif /* CUSTOMFONT_H_ */
