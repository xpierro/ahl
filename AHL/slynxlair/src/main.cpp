#include "lib/2D/GL2D.h"
#include "lib/io/Pad.h"
#include "lib/tools/dialog/Dialog.h"
#include "lib/tools/dialog/InformationDialog.h"
#include "lib/io/Console.h"
#include "lib/io/DebugFont.h"
#include "lib/network/Socket.h"
#include "lib/texture/PNG.h"
#include "lib/2D/Sprite.h"

#include <fstream>
#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include <list>
#include <sys/paths.h>
#include <sys/process.h>

using namespace PS3;

int main() {
    GL2D::init();
    GL2D::setBoundaries();
    Sprite sprite(SYS_DEV_HDD0"/game/PLIB00000/USRDIR/flag.png");

    while(!GL2D::systemExited)
    {
        GL2D::preDraw();

        GL2D::drawSprite(sprite);

        psglSwap();

        sys_timer_usleep (1000 * 10);

        cellSysutilCheckCallback();
    }


	return 0;
}

SYS_PROCESS_PARAM(1001, 0x10000)
