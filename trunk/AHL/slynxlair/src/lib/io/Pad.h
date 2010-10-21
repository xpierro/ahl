/*
 * Pad.h
 *
 *  Created on: 12 oct. 2010
 *      Author: Pierre
 */

#ifndef PAD_H_
#define PAD_H_

#include <cell/pad.h>
#include <list>
#include "../tools/Thread.h"

using namespace std;

enum PadButton {
	LEFT_B,
	DOWN_B,
	RIGHT_B,
	UP_B,
	START_B,
	R3_B,
	L3_B,
	SELECT_B,

	SQUARE_B,
	CROSS_B,
	CIRCLE_B,
	TRIANGLE_B,
	R1_B,
	L1_B,
	R2_B,
	L2_B,
};

enum Stick {
	LEFT_STICK,
	RIGHT_STICK
};

enum Axis {
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

namespace PS3 {

class Pad : Thread {
public:
	static const int LEFT = CELL_PAD_CTRL_LEFT;
	static const int DOWN = CELL_PAD_CTRL_DOWN;
	static const int RIGHT = CELL_PAD_CTRL_RIGHT;
	static const int UP = CELL_PAD_CTRL_UP;
	static const int START = CELL_PAD_CTRL_START;
	static const int R3 = CELL_PAD_CTRL_R3;
	static const int L3 = CELL_PAD_CTRL_L3;
	static const int SELECT = CELL_PAD_CTRL_SELECT;

	static const int SQUARE = CELL_PAD_CTRL_SQUARE;
	static const int CROSS = CELL_PAD_CTRL_CROSS;
	static const int CIRCLE = CELL_PAD_CTRL_CIRCLE;
	static const int TRIANGLE = CELL_PAD_CTRL_TRIANGLE;
	static const int R1 = CELL_PAD_CTRL_R1;
	static const int L1 = CELL_PAD_CTRL_L1;
	static const int R2 = CELL_PAD_CTRL_R2;
	static const int L2 = CELL_PAD_CTRL_L2;
private:
	int socketNumber; // Numéro de pad, entre 0 et 6
	static bool padInitialized; // Préviens si la librairie a déja été init
	/**
	 * Structures interne à la librairie contenant les infos brutes
	 */
	CellPadData padData;
	static CellPadInfo info;

	// Retient toutes les instances de Pad
	static list<Pad*>* pads;


public:
	/**
	 * @param padId L'identifiant du pad à enregistrer (une instance par pad)
	 */
	Pad(int padId);
	virtual ~Pad();

	/**
	 * Relit les informations du pad, à faire à chaque tour de boucle, avant
	 * de demander des infos sur les boutons appuyés et leur puissance.
	 */
	void pollPadData();

	int getPadNumber() const;

	/**
	 * Initialise la librairie libpad
	 */
	static void initPad();
	/**
	 * Ferme proprement la librairie libpad
	 */
	static void closePad();

	/**
	 * Renvoie vrai si le boutton est appuyé.
	 */
	bool isButtonPressed(PadButton);

	/**
	 * Renvoie la puissance d'appuie sur le bouton (0: faible - 255 fort).
	 */
	int getButtonPressingPower(PadButton button);

	/**
	 * Fait vibrer la manette (beta - non implémenté)
	 */
	void makeVibration();

	/**
	 * Renvoie une valeur en %, positif ou negatif, indiquant l'orientation de l'axe
	 * X ou Y du stick donné en argument
	 * Main code done by Slynk, thx :)
	 */
	float getAxisPosition(Stick s, Axis a);

	/**
	 * Renvoie vrai si la manette est allumée.
	 */
	static bool isPadConnected(const Pad*);

	virtual void execute();
};

}

#endif /* PAD_H_ */
