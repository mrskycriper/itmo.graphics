//
// Created by serge on 25.02.2020.
//

#include "pictureData.h"

pictureData::pictureData() :
		mode(P5),
		width(0),
		height(0),
		colorDepth(0),
		data(nullptr)
{}

int pictureData::read(const string& fileName)
{
	ifstream input;
	input.open(fileName, ios_base::in | ios_base::binary);
	if (!input.is_open())
	{
		cerr << "Error with opening " << fileName << endl;
		input.close();
		return 1;
	}
	string type;
	input >> type;
	if (!input.good())
	{
		cerr << "Error with reading " << fileName << endl;
		input.close();
		return 1;
	}
	if (type == "P5")
	{
		mode = P5;
	}
	else if (type == "P6")
	{
		mode = P6;
	}
	else
	{
		cerr << "Wrong file type or file was corrupted" << endl;
		input.close();
		return 1;
	}
	input >> width >> height >> colorDepth;
	if (!input.good())
	{
		cerr << "Error with reading " << fileName << endl;
		input.close();
		return 1;
	}

	if (!mode)
	{
		data = new unsigned char[height * width + 1];
		if (data == nullptr)
		{
			cerr << "Out of memory" << endl;
			input.close();
			return 1;
		}
		input.read((char*) data, height * width + 1);
		input.close();
		return 0;
	}
	else if (mode)
	{
		data = new unsigned char[height * width * 3 + 1];
		if (data == nullptr)
		{
			cerr << "Out of memory" << endl;
			input.close();
			return 1;
		}
		input.read((char*) data, height * width * 3 + 1);
		input.close();
		return 0;
	}
	return 1;
}

void pictureData::invert()
{
	if (!mode)
	{
		for (int i = 1; i < width * height + 1; i++)
		{
			data[i] = colorDepth - data[i];
		}
	}
	else if (mode)
	{
		for (int i = 1; i < width * height * 3 + 1; i++)
		{
			data[i] = colorDepth - data[i];
		}
	}
}

int pictureData::write(const string& fileName)
{
	ofstream output;
	output.open(fileName, ios_base::out | ios_base::binary);
	if (!output.is_open())
	{
		cerr << "Error with opening/creating " << fileName << endl;
		return 1;
	}
	if (!mode)
	{
		output << "P5";
	}
	else if (mode)
	{
		output << "P6";
	}
	output << '\n' << width << '\n' << height << '\n' << colorDepth;
	if (!output.good())
	{
		cerr << "Error with reading " << fileName << endl;
		output.close();
		return 1;
	}
	if (!mode)
	{
		output.write((char*) data, height * width + 1);
		output.close();
		return 0;
	}
	else if (mode)
	{
		output.write((char*) data, height * width * 3 + 1);
		output.close();
		return 0;
	}
	output.close();
	return 1;
}

void pictureData::flipVertical()
{
	if (!mode)
	{
		for (int i = 0; i < height / 2; i++)
		{
			for (int j = 0; j < width; j++)
			{
				swap(data[(i * width + j) + 1], data[((height - i - 1) * width + j) + 1]);
			}
		}
	}
	else if (mode)
	{
		for (int i = 0; i < height / 2; i++)
		{
			for (int j = 0; j < width * 3; j++)
			{
				swap(data[(i * width * 3 + j) + 1], data[((height - i - 1) * width * 3 + j) + 1]);
			}
		}
	}
}

void pictureData::flipHorisontal()
{
	if (!mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width / 2; j++)
			{
				swap(data[(i * width + j) + 1], data[(i * width + width - j - 1) + 1]);
			}
		}
	}
	else if (mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width / 2; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					swap(data[(i * width + j) * 3 + k + 1], data[(i * width + width - j - 1) * 3 + k + 1]);
				}
			}
		}
	}
}

int pictureData::turnClockwise()
{
	unsigned char* newData = nullptr;
	if (!mode)
	{
		newData = new unsigned char[height * width + 1];
	}
	else if (mode)
	{
		newData = new unsigned char[height * width * 3 + 1];
	}

	if (newData == nullptr)
	{
		cerr << "Out of memory" << endl;
		return 1;
	}
	newData[0] = '\n';
	if (!mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newData[(j * height + i) + 1] = data[((height - i - 1) * width + j) + 1];
			}
		}
	}
	else if (mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					newData[(j * height + i) * 3 + k + 1] = data[((height - i - 1) * width + j) * 3 + k + 1];
				}
			}
		}
	}
	swap(height, width);
	delete[] data;
	data = newData;
	return 0;
}

int pictureData::turnCounterclockwise()
{
	unsigned char* newData = nullptr;
	if (!mode)
	{
		newData = new unsigned char[height * width + 1];
	}
	else if (mode)
	{
		newData = new unsigned char[height * width * 3 + 1];
	}

	if (newData == nullptr)
	{
		cerr << "Out of memory" << endl;
		return 1;
	}
	newData[0] = '\n';
	if (!mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newData[(j * height + i) + 1] = data[(i * width + width - j - 1) + 1];
			}
		}
	}
	else if (mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					newData[(j * height + i) * 3 + k + 1] = data[(i * width + width - j - 1) * 3 + k + 1];
				}
			}
		}
	}
	swap(height, width);
	delete[] data;
	data = newData;
	return 0;
}

pictureData::~pictureData()
{
	delete[] data;
}