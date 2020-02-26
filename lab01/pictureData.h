//
// Created by serge on 25.02.2020.
//

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class pictureData
{
public:
	pictureData();
	int read(const string& fileName);
	void invert();
	int write(const string& fileName);
	void flipVertical();
	void flipHorisontal();
	int turnClockwise();
	int turnCounterclockwise();
	~pictureData();
private:
	enum Type
	{
		P5,
		P6
	};
	Type mode;
	int width;
	int height;
	int colorDepth;
	unsigned char* data; // [REDACTED]
};

enum operations
{
	INVERT,
	FLIP_VERTICAL,
	FLIP_HORISONTAL,
	TURN_CLOCKWISE,
	TURN_COUNTERCLOCKWISE
};

