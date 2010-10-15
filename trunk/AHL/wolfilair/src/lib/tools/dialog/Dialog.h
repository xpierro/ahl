/*
 * Dialog.h
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#ifndef DIALOG_H_
#define DIALOG_H_

#include <string>
#include <sysutil/sysutil_msgdialog.h>

using namespace std;

enum Background {
	BG_TRANSPARENT = CELL_MSGDIALOG_TYPE_BG_INVISIBLE,
	BG_OPAQUE = CELL_MSGDIALOG_TYPE_BG_VISIBLE
};

enum Sound {
	SND_ERROR,
	SND_NORMAL,
	SND_OFF
};

enum ButtonType {
	BT_NONE = CELL_MSGDIALOG_TYPE_BUTTON_TYPE_NONE,
	BT_YESNO = CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO,
	BT_OK = CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK
};

enum BackButtonEnabled {
	B_YES = CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_OFF,
	B_NO = CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON
};


namespace PS3 {
/**
 * Interface décrivant les fenêtre de type Dialog
 */
class Dialog {
protected:
	unsigned int type;
	string msg;
public:
	Dialog();
	virtual ~Dialog();
	void show();
	/**
	 * A redéfinir dans chaque implémentation, sera la réaction à une
	 * action de l'utilisateur.
	 */
	virtual void executeCallback(int buttonType) = 0;

	static void closeDialogs();
};
}
#endif /* DIALOG_H_ */
