#pragma once
#include <string>

class Settings
{
public:
	Settings(int argc, char* argv[]);
	~Settings() = default;

	std::string inputName;
	std::string outputName;
	int brightness;
	double thickness;
	double xStart;
	double yStart;
	double xFinish;
	double yFinish;
	double gamma;
};

