#pragma once
#include <string>

enum ColorSpace
{
	RGB,
	HSL,
	HSV,
	YCbCr_601,
	YCbCr_709,
	YCoCg,
	CMY
};

class Pixel
{
public:
	unsigned char First;
	unsigned char Second;
	unsigned char Third;
};


class Picture
{
public:
	Picture ();
	~Picture ();
	void ReadPPM (const std::string&);
	void ReadPGMS (std::string&);
	void SetColorSpace (const ColorSpace&);
	void Convert (const ColorSpace&);
	void WritePPM (const std::string&);
	void WritePGMS(std::string&);
private:
	ColorSpace space;
	int width;
	int height;
	int colorDepth;
	Pixel* data;

	void convertToRGB ();
};

