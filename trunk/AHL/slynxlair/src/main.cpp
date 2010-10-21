#include "lib/2D/GL2D.h"
#include "lib/2D/SpriteA.h"

#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include <sys/paths.h>
#include <sys/process.h>
#include <cell/pad/libpad.h>

using namespace PS3;

struct CellPadData padData;

void checkPad(SpriteA &sprite)
{
    cellPadGetData(0, &padData);

    if((padData.button[6] < 0x0070))
    {sprite.setX(sprite.getX() - 1);}

    if((padData.button[6] > 0x0090))
    {sprite.setX(sprite.getX() + 1);}

    if((padData.button[7] < 0x0070))
    {sprite.setY(sprite.getY() - 1);}

    if((padData.button[7] > 0x0090))
    {sprite.setY(sprite.getY() + 1);}

    if((padData.button[4] < 0x0070))
    {GL2D::scrollX(-1);}

    if((padData.button[4] > 0x0090))
    {GL2D::scrollX(1);}

    if((padData.button[5] < 0x0070))
    {GL2D::scrollY(-1);}

    if((padData.button[5] > 0x0090))
    {GL2D::scrollY(1);}

    cellPadClearBuf(0);

}

int main() {
    GL2D::init(); /* Initialize OpenGl */

    GL2D::setBoundaries(); /* Sets the screen dimensions.
    Keep blank for 0 to screen dimesnions
    or insert 4 floats for a custom viewport (left, right, bottom, top) */

    cellPadInit(1);

    SpriteA sprite(SYS_DEV_HDD0"/game/PLIB00000/USRDIR/Sprites/digital_rain/digital-rain", 1, 39); /*Make a animated sprite from this png*/

    while(!GL2D::systemExited)
    {
        checkPad(sprite);

        GL2D::preDraw(); /*Sets the system up to draw*/

        GL2D::drawSprite(sprite); /*Takes a sprite and draws it to the screen*/

        psglSwap(); /* Swap the buffer to the screen */

        cellSysutilCheckCallback(); /* Check for user exiting the game */
    }

    cellPadEnd();
	return 0;
}

SYS_PROCESS_PARAM(1001, 0x10000) /* Always include this */
