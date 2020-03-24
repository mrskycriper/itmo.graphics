#include <cmath>
#include "Line.h"

using namespace std;

Line::Line(double& xStartIN, double& yStartIN, double& xFinishIN, double& yFinishIN, const int& brightnessIN,
		   const double& thicknessIN)
{
	xStart = xStartIN;
	yStart = yStartIN;
	xFinish = xFinishIN;
	yFinish = yFinishIN;
	brightness = brightnessIN;
	thickness = thicknessIN;
}

bool Line::verticalCheck()
{
	return xStart == xFinish;
}

bool Line::horizontalCheck()
{
	return yStart == yFinish;
}

bool Line::leftToRightCheck()
{
	return abs(xStart - xFinish) >= abs(yStart - yFinish);
}

bool Line::topToBottomCheck()
{
	return abs(xStart - xFinish) < abs(yStart - yFinish);
}