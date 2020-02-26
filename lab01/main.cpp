#include "pictureData.h"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cerr << "Error, use template: <input_file_name> <output_file_name> <operation_code>" << endl;
	}
	pictureData picture;
	int warning = 0;
	warning = picture.read(argv[1]);
	if (warning != 0)
	{
		return 1;
	}

	switch (stoi(argv[3]))
	{
		case INVERT:
		{
			picture.invert();
			warning = picture.write(argv[2]);
			if (warning != 0)
			{
				return 1;
			}
			break;
		}
		case FLIP_VERTICAL:
		{
			picture.flipVertical();
			warning = picture.write(argv[2]);
			if (warning != 0)
			{
				return 1;
			}
			break;
		}
		case FLIP_HORISONTAL:
		{
			picture.flipHorisontal();
			warning = picture.write(argv[2]);
			if (warning != 0)
			{
				return 1;
			}
			break;
		}
		case TURN_CLOCKWISE:
		{
			warning = picture.turnClockwise();
			if (warning != 0)
			{
				return 1;
			}
			warning = picture.write(argv[2]);
			if (warning != 0)
			{
				return 1;
			}
			break;
		}
		case TURN_COUNTERCLOCKWISE:
		{
			warning = picture.turnCounterclockwise();
			if (warning != 0)
			{
				return 1;
			}
			warning = picture.write(argv[2]);
			if (warning != 0)
			{
				return 1;
			}
			break;
		}
		default:
		{
			cerr << "Wrong operation, please use valid operation code" << endl;
			return 1;
		}
	}
	return 0;
}
