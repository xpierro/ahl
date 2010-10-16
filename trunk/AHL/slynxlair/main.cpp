#include <sys/process.h>
#include <sys/paths.h>
#include <sys/spu_initialize.h>
#include <PSGL/psglu.h>
#include <gfxCommon.h>
#include <gfxObject.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "src/png.h"

float squareVertices[] = {-1.0f, 1.0f, 0.0f,
                           1.0f, 1.0f, 0.0f,
                           1.0f,-1.0f, 0.0f,
                          -1.0f,-1.0f, 0.0f};

float textVertices[] = {0.0f, 1.0f,
                           1.0f, 1.0f,
                           1.0f,0.0f,
                          0.0f,0.0f};

char pngPath[] = "/dev_hdd0/game/HELO11111/USRDIR/test.png";
GLuint texture;
char* message;
int error = 0;

bool systemExited = false;

void getError(){
    switch(error){
    case 0:
        message = "Woot!";
        break;
    case 1:
        message = "Could not load CELL_SYSMODULE_FS";
        break;
    case 2:
        message = "Could not load CELL_SYSMODULE_PNGDEC";
        break;
    case 3:
        message = "Could not create Decoder";
        break;
    case 4:
        message = "Could not Open Decoder";
        break;
    case 5:
        message = "Could not read Header";
    case 6:
        message = "Could not set Decoder Parameters";
        break;
    case 7:
        message = "Could not Decode the PNG!";
        break;
    default:
        message = "shit >.<";
        break;
    }
}



void renderFunc(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();


    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, squareVertices);

    //glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    //glTexCoordPointer(3, GL_FLOAT, 0, textVertices);

    glPushMatrix();
    //glColor4f(1.0, 1.0, 0.0, 0.3);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    dbgFontPrintf(100,100,1.5f, message);
    dbgFontDraw();
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

    // init PSGL and get the current system width and height
    sys_spu_initialize(6, 1);

    cellSysutilRegisterCallback(0, systemCallback, NULL);

    gfxInitGraphics();

    dbgFontInit();

    //glDisable(GL_CULL_FACE);

    error = pngDecode(gfxWidth, gfxHeight,texture, pngPath);
    getError();

    //Register the styemcallback function


	while(!systemExited){

        //clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render
		renderFunc();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();


		// swap PSGL buffers -
		psglSwap();

		// check for exit
		cellSysutilCheckCallback();

	}


	//PS3::GL::close();
	psglExit();
    sys_process_exit(0);

}
