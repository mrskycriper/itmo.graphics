#pragma once
#include <string>

enum Type
{
	P5,
	P6
};

enum ColorSpace
{
	RGB,
	YCBCR601
};

enum ConversionType
{
	RGB_MANUAL = 0,
	YCBCR601_MANUAL = 1,
	RGB_AUTO = 2,
	YCBCR601_AUTO = 3,
	RGB_AUTO_CUT = 4,
	YCBCR601_AUTO_CUT = 5,
};

class Pixel
{
public:
	unsigned char first_subpixel;
	unsigned char second_subpixel;
	unsigned char third_subpixel;
};

class Picture
{
public:
	Picture();
	~Picture();

	void Read(const std::string&);
	void Write(const std::string&);

	void SetColorSpace (const ColorSpace&) noexcept;

	void ManualConversion(const int&, const double&) noexcept;
	void AutoConversion(const ConversionType&) noexcept;
private:
	int height;
	int width;
	Type color;
	ColorSpace color_space;
	Pixel* data;

	std::pair <int, int> cut_dark_and_bright (const bool&) noexcept;
};
