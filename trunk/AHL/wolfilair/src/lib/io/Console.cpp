/*
 * Console.cpp
 *
 *  Created on: 14 oct. 2010
 *      Author: Pierre
 */

#include "Console.h"
#include "../GL.h"

#include <stdarg.h>

namespace PS3 {

Console::Console() {
	consoleOpened = false;
	if (consoles == NULL) {
		consoles = new list<Console*>();
	}
	consoles->push_back(this);
}

Console::~Console() { }

list<Console*>* Console::consoles = NULL;
bool Console::consoleInitialized = false;

/**
 * Ferme toutes les consoles quand le système s'arrête.
 */
void consoleCallback(uint64_t status, uint64_t params, void* userdata) {
	(void) params;
	(void) userdata;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Console::closeAllConsoles();
	}
}

void Console::initConsole() {
	if (!consoleInitialized) {
		DebugFont::initDbgFont();
		GL::addUserCallback(consoleCallback);
		consoleInitialized = true;
	}
}

void Console::openConsole(float posX, float posY, int charW, int charH) {
	if(!consoleOpened) {
		initConsole();
		CellDbgFontConsoleConfig ccfg;
		ccfg.posLeft = posX;		// Left coordinate of the console
		ccfg.posTop = posY;		// Upper coordinate of the console
		ccfg.cnsWidth = charW;		// Console width (number of characters)
		ccfg.cnsHeight = charH;		// Console height (number of characters)
		ccfg.scale = 0.50f;		// Font size
		ccfg.color = 0xffffff00;	// Font color
		consoleId = cellDbgFontConsoleOpen(&ccfg);
		consoleOpened = true;
	}
}

void Console::closeAllConsoles() {
	if (consoles != NULL) {
		list<Console*>::iterator it = consoles->begin();
		while(it != consoles->end()) {
			(*it)->close();
			++it;
		}
		delete consoles;
	}
}

void Console::close() {
	cellDbgFontConsoleClose(consoleId);
}

void Console::clear() {
	cellDbgFontConsoleClear(consoleId);
}

void Console::printf(string str, ...) {
	openConsole(0.1f, 0.1f, 30, 15);
	va_list args;
	va_start(args, str);
	// Fonction spéciale de SCE permettant de construire des wrappers autour du
	// printf.
	cellDbgFontConsoleVprintf(consoleId, str.c_str(), args);
	va_end(args);
}

void Console::show() {
	cellDbgFontConsoleEnable(consoleId);
}

void Console::hide() {
	cellDbgFontConsoleDisable(consoleId);
}

Console& Console::operator<<(string s) {
	printf(s);
	return *this;
}

Console& Console::operator<<(basic_ostream<char, std::char_traits<char> >& (*f)(basic_ostream<char, std::char_traits<char> >&)) {
	(void) f;
	printf("\n");
	return *this;
}

}
