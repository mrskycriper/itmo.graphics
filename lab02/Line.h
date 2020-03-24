#pragma once

class Line
{
public:
	Line(double& xStart, double& yStart, double& xFinish, double& yFinish, const int& brightness, const double& thickness);
	bool verticalCheck();
	bool horizontalCheck();
	bool leftToRightCheck();
	bool topToBottomCheck();

	int brightness;
	double thickness;
	double xStart;
	double yStart;
	double xFinish;
	double yFinish;
};