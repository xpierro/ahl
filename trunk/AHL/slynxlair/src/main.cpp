#include "lib/GL.h"
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

const float allImage[] = {-1, -1,
                           1, -1,
                          -1,  1,
                           1,  1};


//static Socket* sock;

float camX = 0;
float camY = 0;
float camZ = 2;

float camTargetX = 0;
float camTargetY = 0;
float camTargetZ = 0;

PNG png1(SYS_DEV_HDD0"/game/PLIB00000/USRDIR/flag.png");
Sprite sprite1(png1);

static bool pngDebug = false;

int loop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAtf(camX, camY, camZ, camTargetX, camTargetY, camTargetZ, 0, 1, 0);


	void drawArrays(float*, float, float, float);
	glBindTexture(GL_TEXTURE_2D, sprite1.image.getTextureId());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, sprite1.getCoord());
    glDrawArrays(GL_QUADS, 0, 4);
	glTexCoordPointer(2, GL_FLOAT, 0, allImage);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();
	glPopMatrix();

	psglSwap();
	// Evite le blocage normalement de "quitter le jeu" en mode multitache.
	sys_timer_usleep (1000 * 10);
	return 1;
}

int main() {
	PS3::GL::executeMainLoop(loop);
	return 0;
}

SYS_PROCESS_PARAM(1001, 0x10000)
