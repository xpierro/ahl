#include <sys/process.h>
#include <sys/spu_initialize.h>
#include <PSGL/psglu.h>
#include "Common/gfxCommon.h"
#include "Common/gfxObject.h"
#include "png.h"

#define DEFAULT_VIEW_DIST  -5.f
#define NEAR_CLIP	0.1f
#define FAR_CLIP	8500.1f
#define FOV_Y		40.0f

bool systemExited = false;
int test = 0; // I threw this in to make sure the screen was updating and not just writing once and quiting

void renderFunc(){

    test++;
    dbgFontPrintf(100,100,1.5f,"Hello World: %d", test);
    dbgFontDraw();

    if(test >= 100){test = 0;}
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
