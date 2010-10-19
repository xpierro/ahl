#include "Side.hpp"
#include "Solid.hpp"
#include "OGLWrapper.hpp"
#include "ProgressBar.hpp"
#include "World.hpp"

#include <string>


int main(int argc, char **argv) {
	OGLWrapper::init_SDL();
	OGLWrapper::init_Textures();

	ProgressBar::create();
	ProgressBar::registerType("world");

	//World w("cs_suburb_beta16.vmf");
	World w("de_cpl_mill.vmf");
	//World w("nt_ch0_00.vmf");
	//World w("info_player_start.vmf");
	//World w("test.txt");
	//World w("pl_goldrush_entitiy_setup.vmf");
	cout << "building tree" << endl;
	OGLWrapper::buildBSPTree();
	OGLWrapper::init_GL();
	OGLWrapper::setupLight();
	OGLWrapper::setCubeMap("Skybox/night1");
	cout << "displaying" << endl;
	OGLWrapper::display_loop();

	OGLWrapper::kill_SDL();
}
