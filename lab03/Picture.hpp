#pragma once

#include <string>

enum Result
{
	OK = 0,
	FILE_ERROR = 1,
	OUT_OF_MEMORY = 2,
	WRONG_FORMAT = 3
};

enum Mode
{
	P5 = 0
};

enum DitheringType
{
	NO_DITHERING = 0,
	ORDERED_8x8 = 1,
	RANDOM = 2,
	FLOYD_STEINBERG = 3,
	JARVIS_JUDICE_NINKE = 4,
	SIERRA_3 = 5,
	ATKINSON = 6,
	HALFTONE_4x4 = 7
};

int ErrorHandler (Result error);

class Picture
{
public:
	Picture ();
	~Picture () = default;
	Result Read (const std::string& fileName);
	Result Write (const std::string& fileName);
	void SetGamma (const double& gamma);
	void MakeGradient ();
	void MakeDither (const DitheringType& Type, const int& BitRate);
private:
	Mode mode;
	int width;
	int height;
	int colorDepth;
	unsigned char* data;
	double Gamma;

	double GammaCorrection (double value) const;
	double ReverseGammaCorrection (double value) const;
};

