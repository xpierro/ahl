/*
 * Console.h
 *
 *  Created on: 14 oct. 2010
 *      Author: Pierre
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "DebugFont.h"

#include <iostream>
#include <list>

using namespace std;

namespace PS3 {

class Console {
private:
	static list<Console*> *consoles;
	static bool consoleInitialized;
	CellDbgFontConsoleId consoleId;
	bool consoleOpened;

public:
	Console();
	virtual ~Console();

	/**
	 * Rétablit la console à l'écran.
	 */
	void show();

	/**
	 * Efface la console de l'écran.
	 */
	void hide();

	/**
	 * Ferme la console.
	 */
	void close();

	/**
	 * Inits the console manager
	 */
	static void initConsole();

	/**
	 * Ferme toutes les consoles ouvertes
	 */
	static void closeAllConsoles();

	/**
	 * Imprime une chaine dans la console.
	 */
	void printf(string, ...);
	/**
	 * Efface le contenu de la console et se replace au début.
	 */
	void clear();


	/**
	 * Fills the console with the given string
	 */
	Console& operator<<(string);

    /**
     * Defines an operator<< to take in std::endl
     */
    Console& operator<<(basic_ostream<char, std::char_traits<char> >&
    				   (*)(basic_ostream<char, std::char_traits<char> >&));

private:
    /**
     * Opens this console
     * @param posX
     * @param posY
     * @param charW
     * @param charH
     */
	void openConsole(float posX, float posY, int charW, int charH);
};

}

#endif /* CONSOLE_H_ */
