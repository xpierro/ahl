/*
 * InformationDialog.h
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#ifndef INFORMATIONDIALOG_H_
#define INFORMATIONDIALOG_H_

#include "Dialog.h"

#include <string>

namespace PS3 {

class InformationDialog : public Dialog {

private:
	bool* dialogClosed;
public:
	InformationDialog(Background, string, bool* b);
	virtual ~InformationDialog();

	virtual void executeCallback(int buttonType);
};
}

#endif /* INFORMATIONDIALOG_H_ */
