/*
 * DebugFont.h
 *
 *  Created on: 14 oct. 2010
 *      Author: Pierre
 */

#ifndef DEBUGFONT_H_
#define DEBUGFONT_H_

#include <cell/dbgfont.h>

#include "../GL.h"

namespace PS3 {

class DebugFont {
private:
	static bool dbgInitialized;

public:
	DebugFont();
	virtual ~DebugFont();

	static void initDbgFont();
	static void drawDbgFont();
	static void closeDbgFont();
};

}

#endif /* DEBUGFONT_H_ */
