#include <iostream>
#include "Picture.hpp"

int main (int argc, char** argv)
{
	if (argc != 11)
	{
		std::cerr << "Invalid arguments" << std::endl;
		return 1;
	}
	ColorSpace Space;
	bool isMultiInput, isMultiOutput;
	std::string inputFile, outputFile, buffer;
	Picture NewPicture;

	for (int i = 1; i < argc; ++i)
	{
		buffer = argv[i];
		if (buffer == "-f")
		{
			++i;
			buffer = argv[i];
			if (buffer == "RGB")
			{
				NewPicture.SetColorSpace (RGB);
				continue;
			}
			if (buffer == "HSL")
			{
				NewPicture.SetColorSpace (HSL);
				continue;
			}
			if (buffer == "HSV")
			{
				NewPicture.SetColorSpace (HSV);
				continue;
			}
			if (buffer == "YCbCr.601")
			{
				NewPicture.SetColorSpace (YCbCr_601);
				continue;
			}
			if (buffer == "YCbCr.709")
			{
				NewPicture.SetColorSpace (YCbCr_709);
				continue;
			}
			if (buffer == "YCoCg")
			{
				NewPicture.SetColorSpace (YCoCg);
				continue;
			}
			if (buffer == "CMY")
			{
				NewPicture.SetColorSpace (CMY);
				continue;
			}
			std::cerr << "Invalid arguments" << std::endl;
			return 1;
		}
		if (buffer == "-t")
		{
			++i;
			buffer = argv[i];
			if (buffer == "RGB")
			{
				Space = RGB;
				continue;
			}
			if (buffer == "HSL")
			{
				Space = HSL;
				continue;
			}
			if (buffer == "HSV")
			{
				Space = HSV;
				continue;
			}
			if (buffer == "YCbCr.601")
			{
				Space = YCbCr_601;
				continue;
			}
			if (buffer == "YCbCr.709")
			{
				Space = YCbCr_709;
				continue;
			}
			if (buffer == "YCoCg")
			{
				Space = YCoCg;
				continue;
			}
			if (buffer == "CMY")
			{
				Space = CMY;
				continue;
			}
			std::cerr << "Invalid arguments" << std::endl;
			return 1;
		}
		if (buffer == "-i")
		{
			++i;
			buffer = argv[i];
			if (buffer == "1")
			{
				isMultiInput = false;
			}
			else if (buffer == "3")
			{
				isMultiInput = true;
			}
			else
			{
				std::cerr << "Invalid arguments" << std::endl;
				return 1;
			}
			++i;
			buffer = argv[i];
			inputFile = buffer;
			continue;
		}
		if (buffer == "-o")
		{
			++i;
			buffer = argv[i];
			if (buffer == "1")
			{
				isMultiOutput = false;
			}
			else if (buffer == "3")
			{
				isMultiOutput = true;
			}
			else
			{
				std::cerr << "Invalid arguments" << std::endl;
				return 1;
			}
			++i;
			buffer = argv[i];
			outputFile = buffer;
			continue;
		}
	}

	try
	{
		if (isMultiInput)
		{
			NewPicture.ReadPGMS (inputFile);
		}
		else
		{
			NewPicture.ReadPPM (inputFile);
		}
	}
	catch (const std::runtime_error& RE)
	{
		std::cerr << RE.what () << std::endl;
		return 1;
	}

	NewPicture.Convert (Space);

	try
	{
		if (isMultiOutput)
		{
			NewPicture.WritePGMS (outputFile);
		}
		else
		{
			NewPicture.WritePPM (outputFile);
		}
	}
	catch (const std::runtime_error& RE)
	{
		std::cerr << RE.what () << std::endl;
		return 1;
	}
	return 0;
}
