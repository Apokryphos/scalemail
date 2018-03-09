#include "engine.hpp"

using namespace ScaleMail;

//  ============================================================================
static EngineStartOptions processCommandLineArguments(int argc, char* argv[]) {
	EngineStartOptions startOptions = {};

	int c = 1;

	while (c < argc) {
		const int argsLeft = argc - c - 1;

		int argsUsed = 0;

		std::string cmd(argv[c]);

		if (cmd == "-map" && argsLeft >= 1) {
			startOptions.mapName = std::string(argv[c + 1]);

			//	Skip intro if map is specified
			startOptions.skipIntro = true;

			argsUsed = 1;
		}

		if (cmd == "-skipintro") {
			startOptions.skipIntro = true;
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