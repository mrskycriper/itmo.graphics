#include "Settings.h"

using namespace std;

Settings::Settings(int argc, char** argv)
{
	inputName = argv[1];
	outputName = argv[2];
	brightness = stoi(argv[3]);
	thickness = stod(argv[4]);
	xStart = stod(argv[5]);
	yStart = stod(argv[6]);
	xFinish = stod(argv[7]);
	yFinish = stod(argv[8]);
	if (argc == 10)
	{
		gamma = stod(argv[9]);
	}
}
