#include <iostream>
#include "Picture.h"
#include "Settings.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 9 || argc > 10)
	{
		cerr << "Invalid arguments" << endl;
		return 1;
	}
	Settings Settings(argc, argv);
	Picture NewPicture;

	Result warning;
	warning = NewPicture.read(Settings.inputName);
	if (errorHandler(warning))
	{
		cerr << "r" << endl;
		return 1;
	}

	Line NewLine(Settings.xStart, Settings.yStart, Settings.xFinish, Settings.yFinish, Settings.brightness, Settings.thickness);
	if (argc == 10)
	{
		NewPicture.replaceGamma(Settings.gamma);
	}
	NewPicture.drawLine(NewLine);

	warning = NewPicture.write(Settings.outputName);
	if (errorHandler(warning))
	{
		cerr << "w" << endl;
		return 1;
	}

	return 0;
}

