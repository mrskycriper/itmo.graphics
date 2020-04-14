#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "Picture.hpp"

using namespace std;

const double Ordered[8][8] = {
		{0.0 / 64.0,  48.0 / 64.0, 12.0 / 64.0, 60.0 / 64.0, 3.0 / 64.0,  51.0 / 64.0, 15.0 / 64.0, 63.0 / 64.0},
		{32.0 / 64.0, 16.0 / 64.0, 44.0 / 64.0, 28.0 / 64.0, 35.0 / 64.0, 19.0 / 64.0, 47.0 / 64.0, 31.0 / 64.0},
		{8.0 / 64.0,  56.0 / 64.0, 4.0 / 64.0,  52.0 / 64.0, 11.0 / 64.0, 59.0 / 64.0, 7.0 / 64.0,  55.0 / 64.0},
		{40.0 / 64.0, 24.0 / 64.0, 36.0 / 64.0, 20.0 / 64.0, 43.0 / 64.0, 27.0 / 64.0, 39.0 / 64.0, 23.0 / 64.0},
		{2.0 / 64.0,  50.0 / 64.0, 14.0 / 64.0, 62.0 / 64.0, 1.0 / 64.0,  49.0 / 64.0, 13.0 / 64.0, 61.0 / 64.0},
		{34.0 / 64.0, 18.0 / 64.0, 46.0 / 64.0, 30.0 / 64.0, 33.0 / 64.0, 17.0 / 64.0, 45.0 / 64.0, 29.0 / 64.0},
		{10.0 / 64.0, 58.0 / 64.0, 6.0 / 64.0,  54.0 / 64.0, 9.0 / 64.0,  57.0 / 64.0, 5.0 / 64.0,  53.0 / 64.0},
		{42.0 / 64.0, 26.0 / 64.0, 38.0 / 64.0, 22.0 / 64.0, 41.0 / 64.0, 25.0 / 64.0, 37.0 / 64.0, 21.0 / 64.0},
};
const double JarvisJudiceNinke[3][5] = {{0, 0, 0, 7, 5},
										{3, 5, 7, 5, 3},
										{1, 3, 5, 3, 1}};
const double Sierra3[3][5] = {{0, 0, 0, 5, 3},
							  {2, 4, 5, 4, 2},
							  {0, 2, 3, 2, 0}};
const double Atkinson[3][5] = {{0, 0, 0, 1, 1},
							   {0, 1, 1, 1, 0},
							   {0, 0, 1, 0, 0}};
const double Halftone[4][4] = {
		{13.0 / 16.0, 11.0 / 16.0, 4.0 / 16.0,  8.0 / 16.0},
		{6.0 / 16.0,  0,           3.0 / 16.0,  15.0 / 16.0},
		{14.0 / 16.0, 1.0 / 16.0,  2.0 / 16.0,  7.0 / 16.0},
		{9.0 / 16.0,  5.0 / 16.0,  10.0 / 16.0, 12.0 / 16.0},
};

int ErrorHandler (Result error)
{
	switch (error)
	{
		case OK:
			return 0;
		case FILE_ERROR:
			std::cerr << "Error occupied while reading/writing image file" << std::endl;
			return 1;
		case OUT_OF_MEMORY:
			std::cerr << "Not enough memory on a disk" << std::endl;
			return 1;
		case WRONG_FORMAT:
			std::cerr << "Wrong file format or file corrupted" << std::endl;
			return 1;
	}
}

Picture::Picture () : mode (P5), width (0), height (0), colorDepth (0), data (nullptr), Gamma (-1.0)
{
}

Result Picture::Read (const std::string& fileName)
{
	std::ifstream input;
	input.open (fileName, std::ios_base::in | std::ios_base::binary);
	if (!input.is_open ())
	{
		return FILE_ERROR;
	}
	std::string tmpMode;
	input >> tmpMode >> width >> height >> colorDepth;
	if (tmpMode != "P5")
	{
		return WRONG_FORMAT;
	}

	if (!mode)
	{
		data = new unsigned char[height * width];
		if (data == nullptr)
		{
			return OUT_OF_MEMORY;
		}
		char* tmp;
		input.read (tmp, 1);
		input.read ((char*) data, height * width);
		input.close ();
		return OK;
	}
	else
	{
		return FILE_ERROR;
	}
}

Result Picture::Write (const std::string& fileName)
{
	std::ofstream output;
	output.open (fileName, std::ios_base::out | std::ios_base::binary);
	if (!output.is_open ())
	{
		return FILE_ERROR;
	}
	std::string tmpMode;
	if (!mode)
	{
		tmpMode = "P5";
	}

	output << tmpMode << '\n' << width << '\n' << height << '\n' << colorDepth << '\n';
	if (!mode)
	{
		output.write ((char*) data, height * width);
		output.close ();
		return OK;
	}
	output.close ();
	return FILE_ERROR;
}

void Picture::SetGamma (const double& gamma)
{
	Gamma = gamma;
}

void Picture::MakeGradient ()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			data[i * width + j] = (unsigned char) GammaCorrection (((double) j / width) * 255.0);
		}
	}
}

double Picture::GammaCorrection (double value) const
{
	value = value / 255;
	if (Gamma == 0) // sRGB gamma correction logic
	{
		if (value < 0.0031308)
		{
			return value * 12.92 * 255;
		}
		else
		{
			return 255 * ((211.0 * std::pow (value, 0.4166) - 11.0) / 200.0);
		}
	}
	else
	{
		return 255 * std::pow (value, Gamma);
	}
}

double Picture::ReverseGammaCorrection (double value) const
{
	value = value / 255;
	if (Gamma == 0) // reverse sRGB gamma correction logic
	{
		if (value < 0.04045)
		{
			return 255 * value * 12.92;
		}
		else
		{
			return 255 * (std::pow ((200.0 * value + 11.0) / 211.0, 2.4));
		}
	}
	else
	{
		return 255 * std::pow (value, 1 / Gamma);
	}
}

void Picture::MakeDither (const DitheringType& Type, const int& BitRate)
{
	int value = std::pow (2, BitRate);
	switch (Type)
	{
		case NO_DITHERING:
		{
			for (int i = 0; i < width * height; ++i)
			{

				double buffer = ReverseGammaCorrection ((double) data[i]) / 255.0;
				buffer *= value - 1;
				buffer = std::round (buffer);
				data[i] = std::round ((int) (GammaCorrection (buffer * (255.0 / (value - 1.0)))));
			}
			break;
		}
		case ORDERED_8x8:
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) +
									 (255.0 / (BitRate)) * (Ordered[i % 8][j % 8] - 0.5)) / 255.0;
					if (buffer < 0)
					{
						buffer = 0;
					}
					buffer *= value - 1;
					buffer = std::round (buffer);
					data[i * width + j] = std::round ((int) (GammaCorrection (buffer * (255.0 / (value - 1)))));
				}
			}
			break;
		}
		case RANDOM:
		{
			std::srand (1);
			for (int i = 0; i < width * height; ++i)
			{
				double buffer =
						(ReverseGammaCorrection (data[i]) +
						 (255.0 / (BitRate)) * ((double) std::rand () / 32767.0 - 0.75)) / 255.0;
				if (buffer < 0)
				{
					buffer = 0;
				}
				buffer *= value;
				buffer = std::round (buffer);
				data[i] = std::round ((int) (GammaCorrection (buffer * (255.0 / (value - 1)))));
			}
			break;
		}
		case FLOYD_STEINBERG:
		{
			std::vector <int> error (height * width, 0);
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) + error[i * width + j]) / 255.0;
					buffer *= (value - 1);
					buffer = std::round (buffer);
					buffer *= 255.0 / (value - 1);
					int CurrentErrorValue = data[i * width + j] + error[i * width + j] - (int) buffer;
					data[i * width + j] = (int) buffer;
					if (j + 1 < width)
					{
						error[i * width + j + 1] += CurrentErrorValue * (7.0 / 16.0);
					}
					if (i + 1 < height)
					{
						if (j + 1 < width)
						{
							error[(i + 1) * width + j + 1] += CurrentErrorValue * (1.0 / 16.0);
						}
						error[(i + 1) * width + j] += CurrentErrorValue * (5.0 / 16.0);
						if ((i - 1 > 0) && (j - 1 > 0))
						{
							error[(i - 1) * width + j - 1] += CurrentErrorValue * (3.0 / 16.0);
						}
					}
				}
			}
			break;
		}
		case JARVIS_JUDICE_NINKE:
		{
			std::vector <int> error (height * width, 0);
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) + error[i * width + j]) / 255.0;
					buffer *= (value - 1);
					buffer = std::round (buffer);
					buffer *= 255.0 / (value - 1);
					int CurrentErrorValue = data[i * width + j] + error[i * width + j] - (int) buffer;
					data[i * width + j] = (int) buffer;
					for (int k = 0; k <= 2; ++k)
					{
						for (int l = -2; l <= 2; ++l)
						{
							if (i + k < height)
							{
								if ((k == 0) && (l > 0))
								{
									if (j + l < width)
									{
										error[(i + k) * width + j + l] +=
												CurrentErrorValue * JarvisJudiceNinke[k][2 + l] / 48.0;
									}
								}
								else
								{
									if ((j + l < width) && (j + l > 0))
									{
										error[(i + k) * width + j + l] +=
												CurrentErrorValue * JarvisJudiceNinke[k][2 + l] / 48.0;
									}
								}
							}
						}
					}
				}
			}
			break;
		}
		case SIERRA_3:
		{
			std::vector <int> error (height * width, 0);
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) + error[i * width + j]) / 255.0;
					buffer *= (value - 1);
					buffer = std::round (buffer);
					buffer *= 255.0 / (value - 1);
					int CurrentErrorValue = data[i * width + j] + error[i * width + j] - (int) buffer;
					data[i * width + j] = (int) buffer;
					for (int k = 0; k <= 2; ++k)
					{
						for (int l = -2; l <= 2; ++l)
						{
							if (i + k < height)
							{
								if ((k == 0) && (l > 0))
								{
									if (j + l < width)
									{
										error[(i + k) * width + j + l] += CurrentErrorValue * Sierra3[k][2 + l] / 32.0;
									}
								}
								else
								{
									if ((j + l < width) && (j + l > 0))
									{
										error[(i + k) * width + j + l] += CurrentErrorValue * Sierra3[k][2 + l] / 32.0;
									}
								}
							}
						}
					}
				}
			}
			break;
		}
		case ATKINSON:
		{
			std::vector <int> error (height * width, 0);
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) + error[i * width + j]) / 255.0;
					buffer *= (value - 1);
					buffer = std::round (buffer);
					buffer *= 255.0 / (value - 1);
					int CurrentErrorValue = data[i * width + j] + error[i * width + j] - (int) buffer;
					data[i * width + j] = (int) buffer;
					for (int k = 0; k <= 2; ++k)
					{
						for (int l = -2; l <= 2; ++l)
						{
							if (i + k < height)
							{
								if ((k == 0) && (l > 0))
								{
									if (j + l < width)
									{
										error[(i + k) * width + j + l] += CurrentErrorValue * Atkinson[k][2 + l] / 8.0;
									}
								}
								else
								{
									if ((j + l < width) && (j + l > 0))
									{
										error[(i + k) * width + j + l] += CurrentErrorValue * Atkinson[k][2 + l] / 8.0;
									}
								}
							}
						}
					}
				}
			}
			break;
		}
		case HALFTONE_4x4:
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					double buffer = (ReverseGammaCorrection (data[i * width + j]) +
									 (255.0 / BitRate) * (Halftone[i % 4][j % 4] - 0.75)) / 255.0;
					if (buffer < 0)
					{
						buffer = 0;
					}
					buffer *= value;
					buffer = std::round (buffer);
					data[i * width + j] = (unsigned char) std::round (
							GammaCorrection (buffer * (255.0 / (value - 1))));
				}
			}
			break;
		}

	}

}


