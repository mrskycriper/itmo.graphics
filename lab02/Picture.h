#pragma once
#include <string>
#include "Line.h"

enum Result
{
	OK,
	FILE_ERROR,
	OUT_OF_MEMORY,
	WRONG_FORMAT,
};

enum Mode
{
	P5
};

enum Direction
{
	DOWN,
	RIGHT,
};

int errorHandler(Result error);



class Picture
{
public:
	Picture();
	~Picture() = default;
	Result read(const std::string& fileName);
	Result write(const std::string& fileName);
	void drawLine(Line& NewLine);
	void replaceGamma(const double& newGamma);
private:
	double gammaCorrection(double value);
	double reverseGammaCorrection(double value);
	void paint(const int& x, const int& y, double brightness);
	void makeSmoothEdges(const int& x, const int& y, const double& radius);
	void drawStrait(Line& NewLine);
	void drawDiagonal(Line& NewLine);

	Mode mode;
	int width;
	int height;
	int colorDepth;
	unsigned char* data;

	double gamma;
	Direction direction;
	double lineBrightness;
};

