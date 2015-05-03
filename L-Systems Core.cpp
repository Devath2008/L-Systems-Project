// Alexander Lease
// L-Systems rendering simulation
//
// Created 04/13/2015
#include "stdafx.h"
#include "L-Systems Core.h"
//This is the number of radians in one degree

using std::string;
using std::vector;
using std::pair;
//vector<pair<point,point> > points;

//Centering Function. Finds the coordinates that are topmmost, bottommost, leftmost, and rightmost
//Uses these values to create an equation that will extend the widest axis to the sides of the window
//Should take into account if the window isn't square
void justifyPoints(vector<pair<point, point> > & thePoints, pair<point,point> newCorners)
{
	float bigx =	 -100000000000000;
	float bigy =	 -100000000000000;
	float smallx =	  100000000000000;
	float smally =	  100000000000000;
	for (int i = 0; i < thePoints.size(); i++)
	{
		//X coords in first and second
		if (thePoints[i].first.x < smallx)
			smallx = thePoints[i].first.x;
		if (thePoints[i].first.x > bigx)
			bigx = thePoints[i].first.x;
		if (thePoints[i].second.x < smallx)
			smallx = thePoints[i].second.x;
		if (thePoints[i].second.x > bigx)
			bigx = thePoints[i].second.x;
		
		//Y coords in first and second
		if (thePoints[i].first.y < smally)
			smally = thePoints[i].first.y;
		if (thePoints[i].first.y > bigy)
			bigy = thePoints[i].first.y;
		if (thePoints[i].second.y < smally)
			smally = thePoints[i].second.y;
		if (thePoints[i].second.y > bigy)
			bigy = thePoints[i].second.y;
	}
	
	//Find the magnitudes of the edges of the old box and the new box
	float xmag, ymag, boxmag;
	xmag = std::abs(bigx - smallx);
	ymag = std::abs(bigy - smally);
	boxmag = std::abs(newCorners.first.x - newCorners.second.x);
	
	float sidespace = 20.0;

	//This is the size of the line length increment
	float ratio = boxmag / (max(xmag, ymag)+sidespace);

	//Find the centers of the old box and the new box
	pair<float, float> oldCenter, newCenter, offset;
	oldCenter.first = (xmag + sidespace) / 2 + smallx;
	oldCenter.second = (ymag + sidespace) / 2 + smally;
	newCenter.first = boxmag / 2 + min(newCorners.first.x, newCorners.second.x);
	newCenter.second = boxmag / 2 + min(newCorners.first.y, newCorners.second.y);
	
	//This is the amount that the values need to be adjusted by after dividing by ratio
	offset.first = (oldCenter.first * ratio) - newCenter.first;
	offset.second = (oldCenter.second * ratio) - newCenter.second;
	
	//Apply transformation to put everything between the new corner coordinates (Screenspace)
	for (int i = 0; i < thePoints.size(); i++)
	{
		float x1, y1, x2, y2;
		x1 = thePoints[i].first.x * ratio - offset.first;
		y1 = thePoints[i].first.y * ratio - offset.second;
		x2 = thePoints[i].second.x * ratio - offset.first;
		y2 = thePoints[i].second.y * ratio - offset.second;
		thePoints[i].first.x = x1;
		thePoints[i].first.y = y1;
		thePoints[i].second.x = x2;
		thePoints[i].second.y = y2;
	}
}

//This function takes the character and returns a number corresponding to one action that can be taken
//Current number of actions: 7
//0 is used as a null output
int actionValue(char theChar)
{
	//A–F	Draw forward
	//G–L	Move forward
	//M–Z	Used only in rule construction
	//- +	Rotate left / right
	//[ ]	Push / Pop current position
	//0-9	Color Presets
	switch (theChar)
	{
		case 'A':
			return 1;
			break;
		case 'B':
			return 1;
			break;
		case 'C':
			return 1;
			break;
		case 'D':
			return 1;
			break;
		case 'E':
			return 1;
			break;
		case 'F':
			return 1;
			break;
		case 'G':
			return 2;
			break;
		case 'H':
			return 2;
			break;
		case 'I':
			return 2;
			break;
		case 'J':
			return 2;
			break;
		case 'K':
			return 2;
			break;
		case 'L':
			return 2;
			break;
		case 'M':
			return 3;
			break;
		case 'N':
			return 3;
			break;
		case 'O':
			return 3;
			break;
		case 'P':
			return 3;
			break;
		case 'Q':
			return 3;
			break;
		case 'R':
			return 3;
			break;
		case 'S':
			return 3;
			break;
		case 'T':
			return 3;
			break;
		case 'U':
			return 3;
			break;
		case 'V':
			return 3;
			break;
		case 'W':
			return 3;
			break;
		case 'X':
			return 3;
			break;
		case 'Y':
			return 3;
			break;
		case 'Z':
			return 3;
			break;
		case '+':
			return 4;
			break;
		case '-':
			return 5;
			break;
		case '[':
			return 6;
			break;
		case ']':
			return 7;
			break;
		case '1':
			return 8;
			break;
		case '2':
			return 8;
			break;
		case '3':
			return 8;
			break;
		case '4':
			return 8;
			break;
		case '5':
			return 8;
			break;
		case '6':
			return 8;
			break;
		case '7':
			return 8;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 8;
			break;
		case '0':
			return 8;
			break;
	}
	return 0;
}

//This function sets the color of the point indicated based on the index of the color that is handed to it
//The color presets are 1=Cyan, 2=Blue, 3=Red, 4=Purple, 5=Blue-Green, 6=White, 7=Dark Green, 8=Light Green, 9=Brown, 0=Orange
void setColor(point & thepoint, char colorindex)
{
	switch (colorindex)
	{
	case '1': //Cyan
		thepoint.r = 0.0;
		thepoint.g = 1.0;
		thepoint.b = 1.0;
		break;
	case '2': //Blue
		thepoint.r = 0.0;
		thepoint.g = 0.0;
		thepoint.b = 1.0;
		break;
	case '3': //Red
		thepoint.r = 1.0;
		thepoint.g = 0.0;
		thepoint.b = 0.0;
		break;
	case '4': //Purple 
		thepoint.r = 1.0;
		thepoint.g = 0.0;
		thepoint.b = 1.0;
		break;
	case '5': //Blue-Green
		thepoint.r = 0.0;
		thepoint.g = 1.0;
		thepoint.b = 0.4;
		break;
	case '6': //White
		thepoint.r = 1.0;
		thepoint.g = 1.0;
		thepoint.b = 1.0;
		break;
	case '7': //Dark Green
		thepoint.r = 0.1;
		thepoint.g = 0.8;
		thepoint.b = 0.3;
		break;
	case '8': //Light Green
		thepoint.r = 0.3;
		thepoint.g = 1.0;
		thepoint.b = 0.3;
		break;
	case '9': //Brown
		thepoint.r = 0.5;
		thepoint.g = 0.35;
		thepoint.b = 0.15;
		break;
	case '0': //Orange
		thepoint.r = 1.0;
		thepoint.g = 0.4;
		thepoint.b = 0.0;
		break;
	}
}

//This function will take a string of characters and will create a list of coordinate pairs for line creating
//This will operate with predetermined meanings for the characters in the sequence
void pointSetter(vector<pair<point, point> > & thePoints, string & theString, float anglesize)
{
	float x = 0;
	float y = 0;
	point pos(x, y, 0, 0, 0);
	std::stack<pair<point,float> > storedPos;
	pair < point, float > posAngle;
	pair < point, point > theLine;
	point temp;
	float angle = 0;
	for (int i = 0; i < theString.size(); i++)
	{
		int action = actionValue(theString[i]);
		switch (action)
		{
		case 1:		//Draw Forward
			x = cos(angle) + pos.x;
			y = sin(angle) + pos.y;
			temp.x = x;
			temp.y = y;
			temp.r = pos.r;
			temp.g = pos.g;
			temp.b = pos.b;
			theLine = std::make_pair(pos,temp);
			pos = temp;
			thePoints.push_back(theLine);
			break;
		case 2:		//Move Forward
			x = cos(angle) + pos.x;
			y = sin(angle) + pos.x;
			pos.x = x;
			pos.y = y;
			break;
		case 4:		//Rotate Right
			angle -= anglesize;
			break;
		case 5:		//Rotate Left
			angle += anglesize;
			break;
		case 6:		//Store Current Location and angle
			posAngle.first = pos;
			posAngle.second = angle;
			storedPos.push(posAngle);
			break;
		case 7:		//Restore Stored Location and angle
			posAngle = storedPos.top();
			storedPos.pop();
			pos = posAngle.first;
			angle = posAngle.second;
			break;
		case 8:		//Sets the color of this and future segments
			setColor(pos, theString[i]);
			break;
		default:	//Rule construction, Bad Value
			break;
		}
	}
}

//Checks for the value in the vector and returns the index of that value
int findinVector(vector<pair<char, string> > & theStuff, char & value)
{
	for (int i = 0; i < theStuff.size(); i++)
	{
		if (theStuff[i].first == value)
			return i;
	}
	return -1;
}

//This is a function that parses the rules string into a list of
//key-value pairs for use in buildString
//Assumes that the rules string is bbuilt using the format
//Variable ":" Replacement ";" Next rule, with no spaces in the formula
void ruleParser(string & rules, vector<pair<char, string> > &  ruleList)
{
	std::stringstream theinput(rules);
	string temp1, temp2;
	char thechar;
	pair<char, string> output;
	while (std::getline(theinput, temp1, ';'))
	{
		temp2 = temp1.substr(2, temp1.size() - 2);
		thechar = temp1[0];
		output = std::make_pair(thechar, temp2);
		ruleList.push_back(output);
	}
}

//The find function assumes there is only one rule for each character
//Rules are parsed into key-value pairs
string buildString(string & start, vector<pair<char, string> > & rules, int & iterations)
{
	string output, built;
	if (iterations == 0)
		return start;
	for (int i = 0; i < start.size(); i++)
	{
		int transform = findinVector(rules, start[i]);
		if (transform != -1)
			output += rules[transform].second;
		else
			output += start[i];
	}
	iterations--;
	built = buildString(output, rules, iterations);
	return built;
}

void gdiRenderFunc(vector<pair<point, point> > & thePoints, HDC hdc)
{
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF color;
	for (int i = 0; i < thePoints.size(); i++)
	{
		color = RGB(thePoints[i].first.r * 255, thePoints[i].first.r * 255, thePoints[i].first.r * 255);
		hLinePen = CreatePen(PS_SOLID, 1, color);
		hPenOld = (HPEN)SelectObject(hdc, hLinePen);
		MoveToEx(hdc,thePoints[i].first.x,thePoints[i].first.y, NULL);
		LineTo(hdc,thePoints[i].second.x,thePoints[i].second.y);
		SelectObject(hdc, hPenOld);
		DeleteObject(hLinePen);
	}
}

//This function initializes values to a preset list of functions
void init(int preset, float & anglesize, string & exrule, string & start, int & iterations, string & description)
{
	switch (preset)
	{
		case 0:	//Dragon Curve
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = 90;
			iterations = 12;
			description = "Dragon Curve";
			break;
		case 1:	//Tree/Fern thingy
			exrule = "F:9FF-[7-F+F+F]+[8+F-F-F];";
			start = "F";
			anglesize = 22;
			iterations = 5;
			description = "Vegetation 1";
			break;
		case 2:	//Sierpinsky Hexagonal
			exrule = "A:B-A-B;B:A+B+A;";
			start = "A";
			anglesize = 60;
			iterations = 6;
			description = "Sierpinsky Gasket 1";
			break;
		case 3:	//Sierpinsky Triangular
			exrule = "F:F-E+F+E-F;E:EE";
			start = "F-E-E";
			anglesize = 120;
			iterations = 6;
			description = "Sierpinsky Gasket 2";
			break;
		case 4:	//Fractal Plant (Wikipedia)
			exrule = "X:F-[[X]+X]+F[+FX]-X;F:FF;";
			start = "XF";
			anglesize = 25;
			iterations = 3;
			description = "Vegetation 2";
			break;
		case 5:	//Koch Curve
			exrule = "F:F+F-F-F+F;";
			start = "F";
			anglesize = 90;
			iterations = 4;
			description = "Koch Curve";
			break;
		case 6:	//Pythagoras Tree
			exrule = "B:BB;A:B[-A]+A;";
			start = "A";
			anglesize = 45;
			iterations = 6;
			description = "Pythagoras Tree";
			break;
		case 7:	//Snowflake
			exrule = "F:F-F++F-F;X=FF;";
			start = "F++F++F";
			anglesize = 60;
			iterations = 4;
			description = "Snowflake 1";
			break;
		case 8:	//PentaSnowflake
			exrule = "F:F-F++F+F-F-F;";
			start = "F-F-F-F-F";
			anglesize = 72;
			iterations = 4;
			description = "Snowflake 2";
			break;
		case 9:	//Hexagonal Gosper
			exrule = "X:X+YF++YF-FX--FXFX-YF+;Y:-FX+YFYF++YF+FX--FX-Y;F:F";
			start = "XF";
			anglesize = 60;
			iterations = 4;
			description = "Hexagonal Gosper";
			break;
		case 10://Rings
			exrule = "F:FF+F+F+F+F+F-F";
			start = "F+F+F+F";
			anglesize = 90;
			iterations = 3;
			description = "Rings";
			break;
		default: //Dragon Curve
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = 90;
			iterations = 12;
			description = "Dragon Curve";
			break;
	}
}

