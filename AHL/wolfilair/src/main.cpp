#include "lib/GL.h"
#include "lib/io/Pad.h"
#include "lib/tools/dialog/Dialog.h"
#include "lib/tools/dialog/InformationDialog.h"
#include "lib/io/Console.h"
#include "lib/io/DebugFont.h"
#include "lib/network/Socket.h"

#include <fstream>
#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include <list>
#include <sys/paths.h>
#include <sys/process.h>

using namespace PS3;

float pos[] = { -1,-1,0,  -1,1,0,  1,-1,0,  1,1,0, };

float squareVerticesFront[] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
float squareVerticesBack[] =  {0, 0, -1, 1, 0, -1, 1, 1, -1, 0, 1, -1};
float squareVerticesLeft[] = {0, 0, 0, 0, 1, 0, 0, 1, -1, 0, 0, -1};
float squareVerticesRight[] = {1, 0, 0, 1, 1, 0, 1, 1, -1, 1, 0, -1};
float squareVerticesUp[] = {0, 1, 0, 1, 1, 0, 1, 1, -1, 0, 1, -1};
float squareVerticesDown[] = {0, 0, 0, 1, 0, 1, 1, 0, -1, 0, 0, -1};

void drawAxisAlignedLine(float cntrX, float cntrY, float halfWidth, float halfHeight)
{
  glPushMatrix();
  glTranslatef(cntrX,cntrY,0);
  glScalef(halfWidth,halfHeight,1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glPopMatrix();
}

void drawGrid(const int numLines)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pos);

  const float hw=0.005f, hh=1.0f;
  float dwh = 2.0f/numLines;

  glColor4f(1,1,1,1);
  for (float x=-1+(dwh*0.5f); x<1; x+=dwh)
    drawAxisAlignedLine(x,0,hw,hh);
  for (float y=-1+(dwh*0.5f); y<1; y+=dwh)
    drawAxisAlignedLine(0,y,hh,hw);

  glColor4f(1,0,0,1);
  drawAxisAlignedLine(-1,0,hw,hh);
  drawAxisAlignedLine(1,0,hw,hh);
  drawAxisAlignedLine(0,-1,hh,hw);
  drawAxisAlignedLine(0,1,hh,hw);
  drawAxisAlignedLine(0,0,hw*2,hw*2);
}

static float zRot=0;
static bool rotate = true;
static int rotation_direction = 1; // Prend 1 ou -1 selon le sens de rotation autour de z
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

int loop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAtf(camX, camY, camZ, camTargetX, camTargetY, camTargetZ, 0, 1, 0);

	if (!socketCreated) {
		socketCreated = true;
		sock = new Socket("irc.fansub-irc.eu", 6667);
		if (sock->connectToHost()) {
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

	glEnableClientState(GL_VERTEX_ARRAY);
	drawArrays(squareVerticesFront, 0.2, 0.3, 0.4);
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
	  ofstream myfile;
	  myfile.open(SYS_DEV_HDD0"/game/PLIB00000/example2.txt");
	  myfile << "Writing this to a file.\n";
	  myfile.close();
	PS3::GL::executeMainLoop(loop);
	return 0;
}

SYS_PROCESS_PARAM(1001, 0x10000)
