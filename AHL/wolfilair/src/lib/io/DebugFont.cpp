/*
 * DebugFont.cpp
 *
 *  Created on: 14 oct. 2010
 *      Author: Pierre
 */

#include "DebugFont.h"

namespace PS3 {

DebugFont::DebugFont() { }

DebugFont::~DebugFont() { }

bool DebugFont::dbgInitialized = false;

// ATTENTION, AJOUTER FERMETURE DE TOUTES LES CONSOLES - STUB
void dbgCallback(uint64_t status, uint64_t params, void* userdata) {
	(void) params;
	(void) userdata;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		DebugFont::closeDbgFont();
	}
}

void DebugFont::initDbgFont() {
	if (!dbgInitialized) {
		CellDbgFontConfig cfg;
		memset(&cfg, 0, sizeof(CellDbgFontConfig));
		cfg.bufSize = 512;
		cfg.screenWidth = GL::getWidth();
		cfg.screenHeight = GL::getHeight();
		cellDbgFontInit(&cfg);
		GL::addUserCallback(dbgCallback);
		dbgInitialized = true;
	}
}

void DebugFont::drawDbgFont() {
	if (dbgInitialized) {
		cellDbgFontDraw();
	}
}

void DebugFont::closeDbgFont() {
	cellDbgFontExit();
}

}
