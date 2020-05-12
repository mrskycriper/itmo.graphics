#include <fstream>
#include <iostream>
#include <vector>
#include "Picture.hpp"

Picture::Picture() : height(0), width(0), color_space(RGB), data(nullptr), color(P5)
{

}

Picture::~Picture()
{
	delete[] data;
}

void Picture::Read(const std::string& InputFileName)
{
	std::ifstream input;
	input.open(InputFileName, std::ios_base::in | std::ios_base::binary);
	if (!input.is_open())
	{
		throw std::runtime_error("Error occupied with open " + InputFileName);
	}

	std::string tmp_type;
	input >> tmp_type;
	if (tmp_type == "P5")
	{
		color = P5;
	}
	else if (tmp_type == "P6")
	{
		color = P6;
	}
	else
	{
		throw std::runtime_error("Wrong file format or header was corrupted");
	}

	input >> width >> height;
	int color_depth;
	input >> color_depth;

	if (!input.good())
	{
		throw std::invalid_argument("Wrong file format or header was corrupted");
	}

	data = new Pixel[width * height];
	if (data == nullptr)
	{
		throw std::runtime_error("Out of memory error");
	}

	char buffer;
	char* buffer_pointer = &buffer;
	input.read(buffer_pointer, 1);
	for (int i = 0; i < width * height; i++)
	{
		if (color)
		{
			input.read(buffer_pointer, 1);
			data[i].first_subpixel = *buffer_pointer;
			input.read(buffer_pointer, 1);
			data[i].second_subpixel = *buffer_pointer;
			input.read(buffer_pointer, 1);
			data[i].third_subpixel = *buffer_pointer;
		}
		else
		{
			input.read(buffer_pointer, 1);
			data[i].first_subpixel = *buffer_pointer;
			data[i].second_subpixel = *buffer_pointer;
			data[i].third_subpixel = *buffer_pointer;
		}
	}

	if (!input.good())
	{
		throw std::runtime_error("Wrong file format or file was corrupted");
	}
	input.close();
}

void Picture::Write(const std::string& OutputFileName)
{
	std::ofstream output;
	output.open(OutputFileName, std::ios_base::out | std::ios_base::binary);
	if (!output.is_open())
	{
		throw std::runtime_error("Error occupied with open " + OutputFileName);
	}

	if (color)
	{
		output << "P6" << '\n';
	}
	else
	{
		output << "P5" << '\n';
	}

	output << width << ' ' << height << '\n' << 255 << '\n';

	char* buffer_pointer = reinterpret_cast <char*> (data);
	if (color)
	{
		output.write(buffer_pointer, width * height * 3);
	}
	else
	{
		for (int i = 0; i < width * height; ++i)
		{
			output.write(&buffer_pointer[i * 3], 1);
		}
	}

	if (output.fail())
	{
		throw std::runtime_error("Error occupied with write" + OutputFileName);
	}
	output.close();
}

void Picture::SetColorSpace(const ColorSpace& NewColorSpace) noexcept
{
	color_space = NewColorSpace;
}

void Picture::ManualConversion(const int& Offset, const double& Multiple) noexcept
{
	if (!color)
	{
		for (int i = 0; i < width * height; ++i)
		{
			int result = static_cast <int> ((data[i].first_subpixel - Offset) * Multiple);
			if (result > 255)
			{
				result = 255;
			}
			if (result < 0)
			{
				result = 0;
			}
			data[i].first_subpixel = result;
			data[i].second_subpixel = result;
			data[i].third_subpixel = result;
		}
	}
	else if (color_space == RGB)
	{
		for (int i = 0; i < width * height; ++i)
		{
			int result = static_cast <int> ((data[i].first_subpixel - Offset) * Multiple);
			if (result > 255)
			{
				result = 255;
			}
			if (result < 0)
			{
				result = 0;
			}
			data[i].first_subpixel = result;

			result = static_cast <int> ((data[i].second_subpixel - Offset) * Multiple);
			if (result > 255)
			{
				result = 255;
			}
			if (result < 0)
			{
				result = 0;
			}
			data[i].second_subpixel = result;

			result = static_cast <int> ((data[i].third_subpixel - Offset) * Multiple);
			if (result > 255)
			{
				result = 255;
			}
			if (result < 0)
			{
				result = 0;
			}
			data[i].third_subpixel = result;
		}
	}
	else
	{
		for (int i = 0; i < width * height; ++i)
		{
			int result = static_cast <int> ((data[i].first_subpixel - Offset) * Multiple);
			if (result > 255)
			{
				result = 255;
			}
			if (result < 0)
			{
				result = 0;
			}
			data[i].first_subpixel = result;
		}
	}

}

void Picture::AutoConversion(const ConversionType& Type) noexcept
{
	std::pair <int, int> buffer;
	buffer = cut_dark_and_bright(Type == RGB_AUTO || Type == YCBCR601_AUTO);

	int offset = buffer.second;
	double multiple = 255.0 / static_cast <double> (buffer.first - buffer.second);
	if (multiple < 0.0039) // 1/255 ~ 0.0039
	{
		multiple = 0.0039;
	}
	if (multiple > 255)
	{
		multiple = 255;
	}
	std::cout << '<' << offset << '>' << ' ' << '<' << multiple << '>' << std::endl;
	ManualConversion(offset, multiple);
}

std::pair <int, int> Picture::cut_dark_and_bright(const bool& avoid_cut) noexcept
{
	int brightest_level = 0, darkest_level = 0;
	std::vector <int> brightness_levels(256, 0);
	for (int i = 0; i < width * height; ++i)
	{
		if ((color_space == RGB) && (color))
		{
			++brightness_levels[data[i].first_subpixel];
			++brightness_levels[data[i].second_subpixel];
			++brightness_levels[data[i].third_subpixel];
		}
		else
		{
			++brightness_levels[data[i].first_subpixel];
		}
	}

	if (!avoid_cut)
	{
		int pixels_to_cut, cutted_pixels = 0;
		if ((color) && (color_space == RGB))
		{
			pixels_to_cut = static_cast <int> (width * height * 3 * 0.0039);
		}
		else
		{
			pixels_to_cut = static_cast <int> (width * height * 0.0039);
		}

		int i = 0, j = 255;
		while (cutted_pixels < pixels_to_cut)
		{
			while (brightness_levels[i] == 0) // cutting darkest
			{
				++i;
			}
			--brightness_levels[i];
			++cutted_pixels;

			while (brightness_levels[j] == 0) // cutting brightest
			{
				--j;
			}
			--brightness_levels[j];
			++cutted_pixels;
		}
	}

	for (int i = 0; i <= 255; ++i)
	{
		if (brightness_levels[i] > 0)
		{
			darkest_level = i;
			break;
		}
	}
	for (int j = 255; j >= 0; --j)
	{
		if (brightness_levels[j] > 0)
		{
			brightest_level = j;
			break;
		}
	}

	return {brightest_level, darkest_level};
}
