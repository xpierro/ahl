#include "lib/GL.h"
#include "lib/io/Pad.h"
#include "lib/tools/dialog/Dialog.h"
#include "lib/tools/dialog/InformationDialog.h"
#include "lib/io/Console.h"
#include "lib/io/DebugFont.h"
#include "lib/network/Socket.h"
#include "lib/texture/PNG.h"
#include "lib/font/Font.h"
#include "lib/font/CustomFont.h"
#include "lib/texture/FontTexture.h"

#include <fstream>
#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include <list>
#include <sys/paths.h>
#include <sys/process.h>

using namespace PS3;

float pos[] = { -1,-1,0,  -1,1,0,  1,-1,0,  1,1,0, };

float squareVerticesFront[] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
float squareTexCoordFront[] = {0., 0., 1., 0., 1., 1., 0., 1.};
float squareVerticesBack[] =  {0, 0, -1, 1, 0, -1, 1, 1, -1, 0, 1, -1};
float squareVerticesLeft[] = {0, 0, 0, 0, 1, 0, 0, 1, -1, 0, 0, -1};
float squareVerticesRight[] = {1, 0, 0, 1, 1, 0, 1, 1, -1, 1, 0, -1};
float squareVerticesUp[] = {0, 1, 0, 1, 1, 0, 1, 1, -1, 0, 1, -1};
float squareVerticesDown[] = {0, 0, 0, 1, 0, 1, 1, 0, -1, 0, 0, -1};

static Pad p1(0); // On init le premier pad
static Console c1; // On init la premiere console

static bool dialogOpened = false;
static bool dialogClosed = false;

static bool socketCreated = false;

static char line[250];

static Socket* sock;

float camX = 0;
float camY = 0;
float camZ = 2;

float camTargetX = 0;
float camTargetY = 0;
float camTargetZ = 0;

PNG png1(SYS_DEV_HDD0"/game/PLIB00000/USRDIR/flag.png");
GLuint j = png1.getTextureId();
CustomFont f1(SYS_DEV_HDD0"/game/PLIB00000/USRDIR/font.ttf");
FontTexture ft(f1, "Hey");
GLuint i = ft.getTextureId();

int loop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAtf(camX, camY, camZ, camTargetX, camTargetY, camTargetZ, 0, 1, 0);

	if (!socketCreated) {
		socketCreated = true;
		sock = new Socket("irc.fansub-irc.eu", 6667);
		if (!sock->connectToHost()) {
			c1 << "couldn't connect :'(" << endl;
		} else {
			c1 << "connected!!" << endl;
		}
	}

	// Make sure we have coherent values in readDataLine for the length
	sock->readDataLine(line, 240);

	if(strlen(line) > 0) {
		c1 << line << endl;
	}
	if(!dialogOpened && !Pad::isPadConnected(&p1)) {
		dialogOpened = true;
		Dialog *d = new InformationDialog(BG_OPAQUE, "Veuillez reconnecter la manette", &dialogClosed);
		d->show();
	}

	if (dialogClosed) {
		dialogOpened = false;
		dialogClosed = false;
	}
/*
	if (p1.isButtonPressed(START_B)) {
		rotate =  rotate ? false : true;
		c1.printf(rotate ? "reprise de la rotation\n" : "arrêt de la rotation\n");
	}

	if (p1.isButtonPressed(LEFT_B) && rotation_direction == 1) {
		rotation_direction = -1;
		c1 << "direction changée vers la gauche" << endl;
	} else if (p1.isButtonPressed(RIGHT_B) && rotation_direction == -1) {
		rotation_direction = 1;
		c1 << "direction changée vers la droite" << endl;
	}

	glRotatef(zRot,0,0,1);

	drawGrid(20);
	if (rotate) {
		zRot += rotation_direction * 0.01f * p1.getButtonPressingPower(R2_B);
		if (zRot>360) zRot-=360;
		if (zRot<360) zRot += 360;
	}
*/

	// TOWARD/BACKWARD translation
	if (p1.isButtonPressed(UP_B)) {
		camZ -= 0.1;
		camTargetZ -= 0.1;
	} else if (p1.isButtonPressed(DOWN_B)) {
		camZ += 0.1;
		camTargetZ += 0.1;
	}

	//LEFT/RIGHT translation
	if(p1.isButtonPressed(LEFT_B)) {
		camTargetX -= 0.1;
		camX -= 0.1;
	} else if (p1.isButtonPressed(RIGHT_B)) {
		camTargetX += 0.1;
		camX += 0.1;
	}

	void drawArrays(float*, float, float, float);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, i);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, squareTexCoordFront);
	drawArrays(squareVerticesFront, 0.2, 0.3, 0.4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	drawArrays(squareVerticesBack, 0.5, 0.3, 0.4);
	drawArrays(squareVerticesLeft, 0.2, 0.3, 0.5);
	drawArrays(squareVerticesRight, 0.2, 0.5, 0.4);
	drawArrays(squareVerticesUp, 0.1, 0.8, 0.4);
	drawArrays(squareVerticesDown, 0.6, 0.7, 0.4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	DebugFont::drawDbgFont();
	glPopMatrix();

	psglSwap();
	// Evite le blocage normalement de "quitter le jeu" en mode multitache.
	sys_timer_usleep (1000 * 10);
	return 1;
}

void drawArrays(float *a, float r, float g, float b) {
	glVertexPointer(3, GL_FLOAT, 0, a);
	glColor4f(r, g, b, 1);
	glDrawArrays(GL_QUADS, 0, 4);
}

int main() {
	PS3::GL::executeMainLoop(loop);
	return 0;
}

SYS_PROCESS_PARAM(1001, 0x10000)
