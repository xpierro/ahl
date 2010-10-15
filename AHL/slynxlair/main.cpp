#include <sys/process.h>
#include <sys/paths.h>
#include <sys/spu_initialize.h>
#include <PSGL/psglu.h>
#include <gfxCommon.h>
#include <gfxObject.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/png.h"

float bgCoord[] = { -2.0, -1.5, 0.0,
					-2.0,  0.0, 0.0,
				 	 2.0, -1.5, 0.0,
					 2.0,  0.0, 0.0 };
float textCoord[] = {0.0 , 0.0,
					 0.0 , 1.0,
					 1.0 , 0.0,
					 1.0 , 1.0,};


GLuint texture;

bool systemExited = false;
int test = 0; // I threw this in to make sure the screen was updating and not just writing once and quiting

void renderFunc(){

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_CULL_FACE);

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textCoord);
	glVertexPointer(3, GL_FLOAT, 0, bgCoord);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

//thanks to Wolfi@metagames
//this is used to catch the user quitting the application
void systemCallback(const uint64_t status, const uint64_t param,
                          void *userdata) {
   (void)userdata; // Pour supprimer les warnings
   (void)param;
   switch (status) {
   case CELL_SYSUTIL_REQUEST_EXITGAME: systemExited = true;
                                       break;
   default: break;
   }
}


SYS_PROCESS_PARAM(1001, 0x10000)


int main(){

	// Initialize 6 SPUs but reserve 1 SPU as a raw SPU for PSGL
	sys_spu_initialize(6, 1);

    //Register the styemcallback function
	cellSysutilRegisterCallback(0, systemCallback, NULL);

	// init PSGL and get the current system width and height
	gfxInitGraphics();

	// initalize the dbgFonts
	dbgFontInit();

	char* path = "";
	sprintf(path, "%s%s%s", SYS_APP_HOME,"/", "test.png");
	texture = pngDecode(gfxWidth, gfxHeight, path);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, texture);


	while(!systemExited){

        //clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render
		renderFunc();

		// swap PSGL buffers -
		psglSwap();

		// check for exit
		cellSysutilCheckCallback();

	}


	psglExit();
    sys_process_exit(0);

}
