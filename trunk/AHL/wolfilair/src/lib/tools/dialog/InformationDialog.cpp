/*
 * InformationDialog.cpp
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#include "InformationDialog.h"
namespace PS3 {

InformationDialog::InformationDialog(Background bg, string message, bool* closed) {
	msg = message;
	type |= bg;
	type |= CELL_MSGDIALOG_TYPE_SE_MUTE_ON;
	type |= BT_OK;
	type |= B_NO;
	dialogClosed = closed;

}

InformationDialog::~InformationDialog() { }

void InformationDialog::executeCallback(int buttonType) {
	switch(buttonType) {
	default: // nothing to do
		*dialogClosed = true;
		break;
	}
}

}
