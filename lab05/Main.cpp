#include <iostream>
#include "Picture.hpp"

int main(int argc, char** argv)
{
	if (argc < 4 || argc > 6)
	{
		std::cerr << "Invalid arguments" << std::endl;
		return 1;
	}

	std::string InputFileName = argv[1], OutputFileName = argv[2];
	ConversionType Type;
	int Offset;
	double Multiple;

	int tmp_type = std::stoi(argv[3]);
	switch (tmp_type)
	{
		case RGB_MANUAL:
		{
			Type = RGB_MANUAL;
			break;
		}
		case YCBCR601_MANUAL:
		{
			Type = YCBCR601_MANUAL;
			break;
		}
		case RGB_AUTO:
		{
			Type = RGB_AUTO;
			break;
		}
		case YCBCR601_AUTO:
		{
			Type = YCBCR601_AUTO;
			break;
		}
		case RGB_AUTO_CUT:
		{
			Type = RGB_AUTO_CUT;
			break;
		}
		case YCBCR601_AUTO_CUT:
		{
			Type = YCBCR601_AUTO_CUT;
			break;
		}
		default:
		{
			std::cerr << "Invalid conversion mode" << std::endl;
			return 1;
		}
	}

	if ((Type == RGB_MANUAL) || (Type == YCBCR601_MANUAL))
	{
		if (argc != 6)
		{
			std::cerr << "Invalid conversion parameters" << std::endl;
			return 1;
		}
		Offset = std::stoi(argv[4]);
		Multiple = std::stod(argv[5]);
	}

	Picture NewPicture;
	if ((Type == YCBCR601_MANUAL) || (Type == YCBCR601_AUTO) || (Type == YCBCR601_AUTO_CUT))
	{
		NewPicture.SetColorSpace(YCBCR601);
	}

	try
	{
		NewPicture.Read(InputFileName);
		if ((Type == RGB_MANUAL) || (Type == YCBCR601_MANUAL))
		{
			NewPicture.ManualConversion(Offset, Multiple);
		}
		else
		{
			NewPicture.AutoConversion(Type);
		}
		NewPicture.Write(OutputFileName);
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		return 1;
	}

	return 0;
}