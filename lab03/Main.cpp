#include <iostream>
#include <string>
#include "Picture.hpp"

using namespace std;

int main (int argc, char** argv)
{
	Result ErrorCheck;
	if (argc != 7)
	{
		cerr << "Invalid arguments" << endl;
		return 1;
	}

	string InputName = argv[1], OutputName = argv[2];
	bool IsGradient;
	if (*argv[3] == *"1")
	{
		IsGradient = true;
	}
	else if (*argv[3] == *"0")
	{
		IsGradient = false;
	}
	else
	{
		cerr << "Invalid arguments" << endl;
		return 1;
	}
	int BitRate = stoi (argv[5]);
	DitheringType Type = (DitheringType) stoi (argv[4]);
	Picture NewPicture;
	NewPicture.SetGamma (stod (argv[6]));

	ErrorCheck = NewPicture.Read (InputName);
	if (ErrorHandler (ErrorCheck) == -1)
	{
		return 1;
	}

	if (IsGradient)
	{
		NewPicture.MakeGradient ();
	}

	NewPicture.MakeDither (Type, BitRate);

	ErrorCheck = NewPicture.Write(OutputName);
	if (ErrorHandler (ErrorCheck) == -1)
	{
		return 1;
	}

	return 0;
}
