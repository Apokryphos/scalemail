#include "engine.hpp"

using namespace ScaleMail;

//  ============================================================================
static EngineStartOptions processCommandLineArguments(int argc, char* argv[]) {
	EngineStartOptions startOptions = {};
	startOptions.screenWidth = 1024;
	startOptions.screenHeight = 1024;

	int c = 1;

	while (c < argc) {
		const int argsLeft = argc - c - 1;

		int argsUsed = 0;

		std::string cmd(argv[c]);

		if ((cmd == "-h" || cmd == "--height") && argsLeft >= 1) {
			startOptions.screenHeight = std::stoi(std::string(argv[c + 1]));

			argsUsed = 1;
		}

		if ((cmd == "-m" || cmd == "--map") && argsLeft >= 1) {
			startOptions.mapName = std::string(argv[c + 1]);

			//	Skip intro if map is specified
			startOptions.skipIntro = true;

			argsUsed = 1;
		}

		if (cmd == "-o" || cmd == "--opengl2") {
			startOptions.openGl2 = true;
		}

		if (cmd == "-s" || cmd == "--skipintro") {
			startOptions.skipIntro = true;
		}

		if ((cmd == "-w" || cmd == "--width") && argsLeft >= 1) {
			startOptions.screenWidth = std::stoi(std::string(argv[c + 1]));

			argsUsed = 1;
		}

		c += 1 + argsUsed;
	}

	return startOptions;
}

//  ============================================================================
int main(int argc, char* argv[]) {
	EngineStartOptions startOptions = processCommandLineArguments(argc, argv);

	return startEngine(startOptions);
}