/*
 * Dialog.cpp
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#include "Dialog.h"

#include <stdio.h>
#include <sysutil/sysutil_msgdialog.h>

namespace PS3 {
Dialog::Dialog() {
	type = 0;
}

Dialog::~Dialog() {
	// TODO Auto-generated destructor stub
}

/**
 * object sera casté en Dialog pour appeler la méthode executeCallback
 */
static void dialogCallback(int buttonType, void* object) {
	Dialog* dialog = static_cast<Dialog*>(object);
	dialog->executeCallback(buttonType);
}

/**
 * On affiche le message.
 */
void Dialog::show() {
	cellMsgDialogOpen2(type, msg.c_str(), dialogCallback, this, NULL);
}

void Dialog::closeDialogs() {
	cellMsgDialogAbort();
}

}
