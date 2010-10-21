/*
 * Pad.cpp
 *
 *  Created on: 12 oct. 2010
 *      Author: Pierre
 */

#include "Pad.h"

#include "../GL.h"
#include "../2D/GL2D.h"

#include <cell/pad.h>
#include <cell/sysmodule.h>

namespace PS3 {

CellPadInfo Pad::info;

Pad::Pad(int padId) : Thread(){
	initPad();
	socketNumber = padId;
	pads->push_back(this);
	cellPadSetPressMode(socketNumber, CELL_PAD_PRESS_MODE_ON);
	start(); // On démarre le thread de polling
}

Pad::~Pad() { }

/**
 * On surcharge la méthode execute pour qu'elle fasse le polling automatiquement
 */
void Pad::execute() {
	while(!threadStopped()) {
		pollPadData();
		sys_timer_usleep (1000 * 10); // On attend un peu pour rafraichir
	}
}

bool Pad::padInitialized = false;
list<Pad*>* Pad::pads = new list<Pad*>();

/**
 * Cette fonction ferme la librairie libpad quand le système ferme le programme.
 */
void padCallback(uint64_t status, uint64_t params, void* userdata) {
	(void) params;
	(void) userdata;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Pad::closePad();
	}
}

/**
 * Converti l'enumeration en la valeur constante associée
 */
int padButtonToPressedValue(PadButton b) {
	switch(b) {
	case UP_B: return Pad::UP;
	case DOWN_B: return Pad::DOWN;
	case LEFT_B: return Pad::LEFT;
	case RIGHT_B: return Pad::RIGHT;
	case START_B: return Pad::START;
	case SELECT_B: return Pad::SELECT;
	case R1_B: return Pad::R1;
	case R2_B: return Pad::R2;
	case R3_B: return Pad::R3;
	case L1_B: return Pad::L1;
	case L2_B: return Pad::L2;
	case L3_B: return Pad::L3;
	case CROSS_B: return Pad::CROSS;
	case SQUARE_B: return Pad::SQUARE;
	case CIRCLE_B: return Pad::CIRCLE;
	case TRIANGLE_B: return Pad::TRIANGLE;
	}
	return -1; // Empeche le warning
}

/**
 * Renvoie l'indice dans la structure interne associé à l'information de
 * pression (entre 8 et 19) associée.
 */
int padButtonToPressionIndex(PadButton b) {
	switch(b) {
	case RIGHT_B: return 8;
	case LEFT_B: return 9;
	case UP_B: return 10;
	case DOWN_B: return 11;
	case TRIANGLE_B: return 12;
	case CIRCLE_B: return 13;
	case CROSS_B: return 14;
	case SQUARE_B: return 15;
	case L1_B: return 16;
	case R1_B: return 17;
	case L2_B: return 18;
	case R2_B: return 19;
	default: return -1; // Pas d'info de pression pour les autres boutons.
	}
}

void Pad::initPad() {
	if (!padInitialized) {
		// On initialise la librairie en spécifiant le nombre maximal de pads
		// acceptés, fixé à 7.
		if (cellPadInit(7) == CELL_OK){
			padInitialized = true;
		}
		// On enregistre un callback pour fermer la librairie en cas
		// d'interruption du programme.
		if(GL2D::libraryStarted)
		{
		    GL2D::addUserCallback(padCallback);
		}
		else
		{
		    GL::addUserCallback(padCallback);
		}

	}
}

void Pad::closePad() {
	// On arrête les threads de toutes les manettes
	list<Pad*>::iterator it = pads->begin();
	while (it != pads->end()) {
		(*it)->stop();
		++it;
	}
	cellPadEnd();
}

bool Pad::isButtonPressed(PadButton b) {
	if (b >= LEFT_B && b <= SELECT_B) {
		if (padData.len > 0 && (padData.button[2] & padButtonToPressedValue(b))) {
			return true;
		}
	} else if (b >= SQUARE_B && b <= L2_B) {
		if (padData.len > 0 && (padData.button[3] & padButtonToPressedValue(b))) {
			return true;
		}
	}
	return false;
}

int Pad::getButtonPressingPower(PadButton b) {
	// Obligé de switcher ici, car les boutons ne sont pas bien ordonnés
	int index = padButtonToPressionIndex(b);
	return (int) padData.button[index];
}

float Pad::getAxisPosition(Stick s, Axis a) {
	int stickIndex = -1;
	switch(s) {
	case LEFT_STICK: stickIndex = 6; break;
	case RIGHT_STICK: stickIndex = 4;
	}

	switch(a) {
	case Y_AXIS: stickIndex += 1;
	default: break;
	}
	if ((padData.button[stickIndex] > 0x0078) && (padData.button[stickIndex] < 0x0088)){
		return 0;
	}
	return (padData.button[stickIndex] - 128) / 128;
}

void Pad::pollPadData() {
	cellPadGetData (socketNumber, &padData);
}

int Pad::getPadNumber() const{
	return socketNumber;
}

bool Pad::isPadConnected(const Pad* p) {
	cellPadGetInfo(&info);
	return info.status[p->getPadNumber()] == 1;
}

}
