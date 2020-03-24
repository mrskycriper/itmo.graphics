#include <iostream>
#include <fstream>
#include <cmath>
#include "Picture.h"

using namespace std;

int errorHandler(Result result)
{
	switch (result)
	{
		case OK:
			return 0;
		case FILE_ERROR:
			cerr << "Error occupied while reading/writing image file" << endl;
			return 1;
		case OUT_OF_MEMORY:
			cerr << "Not enough memory on a disk" << endl;
			return 1;
		case WRONG_FORMAT:
			cerr << "Wrong file format or file corrupted" << endl;
			return 1;
	}
}

Picture::Picture()
{
	mode = P5;
	width = 0;
	height = 0;
	colorDepth = 0;
	data = nullptr;
	gamma = -1.0; // -1.0 is for sRGB
	direction = DOWN;
	lineBrightness = 0.0;
}

Result Picture::read(const string& fileName)
{
	ifstream input;
	input.open(fileName, ios_base::in | ios_base::binary);
	if (!input.is_open())
	{
		return FILE_ERROR;
	}
	string tmpMode;
	input >> tmpMode >> width >> height >> colorDepth;
	if (tmpMode != "P5")
	{
		return WRONG_FORMAT;
	}

	if (!mode)
	{
		data = new unsigned char[height * width + 1];
		if (data == nullptr)
		{
			return OUT_OF_MEMORY;
		}
		input.read((char*) data, height * width + 1);
		input.close();
		return OK;
	}
	else
	{
		return FILE_ERROR;
	}
}

Result Picture::write(const string& fileName)
{
	ofstream output;
	output.open(fileName, ios_base::out | ios_base::binary);
	if (!output.is_open())
	{
		return FILE_ERROR;
	}
	string tmpMode;
	if (!mode)
	{
		tmpMode = "P5";
	}

	output << tmpMode << '\n' << width << '\n' << height << '\n' << colorDepth;
	if (!mode)
	{
		output.write((char*) data, height * width + 1);
		output.close();
		return OK;
	}
	output.close();
	return FILE_ERROR;
}

void Picture::drawLine(Line& NewLine)
{
	lineBrightness = NewLine.brightness;
	if (NewLine.xStart > NewLine.xFinish)
	{
		swap(NewLine.xStart, NewLine.xFinish);
	}
	if (NewLine.yStart > NewLine.yFinish)
	{
		swap(NewLine.yStart, NewLine.yFinish);
	}
	if (NewLine.horizontalCheck() || NewLine.leftToRightCheck())
	{
		direction = RIGHT;
	}
	if (NewLine.verticalCheck() || NewLine.topToBottomCheck())
	{
		direction = DOWN;
		swap(NewLine.xStart, NewLine.yStart);
		swap(NewLine.xFinish, NewLine.yFinish);
	}

	if (NewLine.horizontalCheck() || NewLine.verticalCheck())
	{
		drawStrait(NewLine);
	}
	else
	{
		drawDiagonal(NewLine);
	}
}

void Picture::replaceGamma(const double& newGamma)
{
	gamma = newGamma;
}

void Picture::drawStrait(Line& NewLine)
{
	for (int xPos = (int) NewLine.xStart; xPos < NewLine.xFinish; ++xPos)
	{
		double y = NewLine.yStart + NewLine.thickness / 2;
		int yPos = floor(y);
		double smoothEdgeThickness = y - yPos;
		if (smoothEdgeThickness > 0.5)
		{
			smoothEdgeThickness -= 0.5;
			++yPos;
		}
		else if (smoothEdgeThickness < 0.5)
		{
			smoothEdgeThickness += 0.5;
		}
		else
		{
			smoothEdgeThickness = 1;
		}
		if ((y - smoothEdgeThickness) > NewLine.yStart - NewLine.thickness / 2)
		{
			paint(xPos, yPos, smoothEdgeThickness);
		}
		else
		{
			paint(xPos, yPos, y - NewLine.yStart + NewLine.thickness / 2);
		}
		--yPos;
		y -= smoothEdgeThickness;
		while (y - (NewLine.yStart - NewLine.thickness / 2) > 1)
		{
			paint(xPos, yPos, 1);
			--yPos;
			--y;
		}
		if (y > NewLine.yStart - NewLine.thickness / 2)
		{
			paint(xPos, yPos, y - (NewLine.yStart - NewLine.thickness / 2));
		}
	}
	makeSmoothEdges((int) NewLine.xStart, (int) NewLine.yStart, NewLine.thickness / 2 - 1);
	makeSmoothEdges((int) NewLine.xFinish, (int) NewLine.yFinish, NewLine.thickness / 2 - 1);
}

void Picture::drawDiagonal(Line& NewLine)
{
	for (int xPos = (int) NewLine.xStart; xPos <= NewLine.xFinish; xPos++)
	{
		double slopeCoefficent = ((NewLine.yFinish - NewLine.yStart) / (NewLine.xFinish - NewLine.xStart)) *
								 (xPos - NewLine.xStart) + NewLine.yStart;
		double y = slopeCoefficent + NewLine.thickness / 2;
		int yPos = floor(y);
		double smoothEdgeThickness = y - yPos;
		if (smoothEdgeThickness > 0.5)
		{
			smoothEdgeThickness -= 0.5;
			++yPos;
		}
		else if (smoothEdgeThickness < 0.5)
		{
			smoothEdgeThickness += 0.5;
		}
		else
		{
			smoothEdgeThickness = 1;
		}
		if ((y - smoothEdgeThickness) > slopeCoefficent - NewLine.thickness / 2)
		{
			paint(xPos, yPos, smoothEdgeThickness);
		}
		else
		{
			paint(xPos, yPos, y - slopeCoefficent + NewLine.thickness / 2);
		}
		--yPos;
		y -= smoothEdgeThickness;
		while (y - (slopeCoefficent - NewLine.thickness / 2) > 1)
		{
			paint(xPos, yPos, 1);
			--yPos;
			--y;
		}
		if (y > slopeCoefficent - NewLine.thickness / 2)
		{
			paint(xPos, yPos, y - (slopeCoefficent - NewLine.thickness / 2));
		}
	}
	makeSmoothEdges((int) NewLine.xStart, (int) NewLine.yStart, NewLine.thickness / 2 - 1);
	makeSmoothEdges((int) NewLine.xFinish, (int) NewLine.yFinish, NewLine.thickness / 2 - 1);
}

double Picture::gammaCorrection(double value)
{
	value = value / 255;
	if (gamma == -1) // sRGB gamma correction logic
	{
		if (value < 0.0031308)
		{
			return value * 12.92 * 255;
		}
		else
		{
			return 255 * ((211.0 * pow(value, 0.4166) - 11.0) / 200.0);
		}
	}
	else
	{
		return 255 * pow(value, gamma);
	}
}

double Picture::reverseGammaCorrection(double value)
{
	value = value / 255;
	if (gamma == -1) // reverse sRGB gamma correction logic
	{
		if (value < 0.04045)
		{
			return 255 * value * 12.92;
		}
		else
		{
			return 255 * (pow((200.0 * value + 11.0) / 211.0, 2.4));
		}
	}
	else
	{
		return 255 * pow(value, 1 / gamma);
	}
}

void Picture::paint(const int& x, const int& y, double brightness)
{
	if ((((y < height) && direction) || ((x < height) && !direction)) &&
		(((x < height) && direction) || ((y < height) && !direction)) && (x >= 0) && (y >= 0))
	{
		if (brightness > 1.0 - 1.0 / 255.0)
		{
			brightness = 1.0;
		}
		if (direction)
		{
			data[y * width + x + 1] = (unsigned char) gammaCorrection(
					reverseGammaCorrection((data[y * width + x + 1])) * (1.0 - brightness) +
					brightness * lineBrightness);
		}
		else
		{
			data[x * width + y + 1] = (unsigned char) gammaCorrection(
					reverseGammaCorrection(data[x * width + y + 1]) * (1.0 - brightness) + brightness * lineBrightness);
		}
	}
}

void Picture::makeSmoothEdges(const int& x, const int& y, const double& radius)
{
	for (int xPos = x; xPos < x + radius; ++xPos)
	{
		int _yPos = y - (int) sqrt(pow(radius, 2) - pow(x - xPos, 2));
		for (int yPos = _yPos; yPos < 2 * y - _yPos; ++yPos)
		{
			paint(xPos, yPos, 1);
			paint(2 * x - xPos, yPos, 1);
		}
	}
}




