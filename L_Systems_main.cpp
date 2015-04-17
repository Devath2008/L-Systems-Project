// Alexander Lease
// L-Systems rendering simulation
//
// Created 04/13/2015
#include <gl\glew.h>
#include <vector>       //std::vector
#include <string>		//std::getline
#include <algorithm>    //std::sort
#include <sstream>      //std::stringstream
#include <iostream>		//std::cin,std::cout
#include <cmath>		//std::abs, std::round
#include <stack>		//std::stack
#include <gl\glut.h>
//This is the number of radians in one degree
#define degree 0.0174532925

using std::string;
using std::vector;
using std::pair;
typedef pair<float, float> coord;
vector<pair<coord, coord> > points;

//Centering Function. Finds the coordinates that are topmmost, bottommost, leftmost, and rightmost
//Uses these values to create an equation that will extend the widest axis to the sides of the window
//Should take into account if the window isn't square
void justifyPoints(vector<pair<coord, coord> > & thePoints, pair<coord,coord> newCorners)
{
	float bigx =	 -100000000000000;
	float bigy =	 -100000000000000;
	float smallx =	  100000000000000;
	float smally =	  100000000000000;
	for (int i = 0; i < thePoints.size(); i++)
	{
		//X coords in first and second
		if (thePoints[i].first.first < smallx)
			smallx = thePoints[i].first.first;
		if (thePoints[i].first.first > bigx)
			bigx = thePoints[i].first.first;
		if (thePoints[i].second.first < smallx)
			smallx = thePoints[i].second.first;
		if (thePoints[i].second.first > bigx)
			bigx = thePoints[i].second.first;
		
		//Y coords in first and second
		if (thePoints[i].first.second < smally)
			smally = thePoints[i].first.second;
		if (thePoints[i].first.second > bigy)
			bigy = thePoints[i].first.second;
		if (thePoints[i].second.second < smally)
			smally = thePoints[i].second.second;
		if (thePoints[i].second.second > bigy)
			bigy = thePoints[i].second.second;
	}
	
	//Find the magnitudes of the edges of the old box and the new box
	float xmag, ymag, boxmag;
	xmag = std::abs(bigx - smallx);
	ymag = std::abs(bigy - smally);
	boxmag = std::abs(newCorners.first.first - newCorners.second.first);
	
	//This is the size of the line length increment
	float ratio = boxmag / (std::max(xmag, ymag)+4);

	//Find the centers of the old box and the new box
	pair<float, float> oldCenter, newCenter, offset;
	oldCenter.first = xmag / 2 + smallx;
	oldCenter.second = ymag / 2 + smally;
	newCenter.first = boxmag / 2 + std::min(newCorners.first.first, newCorners.second.first);
	newCenter.first = boxmag / 2 + std::min(newCorners.first.second, newCorners.second.second);
	
	//This is the amount that the values need to be adjusted by after dividing by ratio
	offset.first = (oldCenter.first - newCenter.first) * ratio;
	offset.second = (oldCenter.second - newCenter.second) * ratio;
	
	//Apply transformation to put everything between the new corner coordinates (Screenspace)
	for (int i = 0; i < thePoints.size(); i++)
	{
		float x1, y1, x2, y2;
		x1 = thePoints[i].first.first * ratio - offset.first;
		y1 = thePoints[i].first.second * ratio - offset.second;
		x2 = thePoints[i].second.first * ratio - offset.first;
		y2 = thePoints[i].second.second * ratio - offset.second;
		thePoints[i].first = std::make_pair(x1, y1);
		thePoints[i].second = std::make_pair(x2, y2);
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
	}
	return 0;
}

//This function will take a string of characters and will create a list of coordinate pairs for line creating
//This will operate with predetermined meanings for the characters in the sequence
void pointSetter(vector<pair<coord, coord> > & thePoints, string & theString, float anglesize)
{
	float x = 0;
	float y = 0;
	coord pos = std::make_pair(x, y);
	std::stack<pair<coord,float> > storedPos;
	pair < coord, float > posAngle;
	pair < coord, coord > theLine;
	coord temp;
	float angle = 0;
	for (int i = 0; i < theString.size(); i++)
	{
		int action = actionValue(theString[i]);
		switch (action)
		{
		case 1:		//Draw Forward
			x = cos(angle) + pos.first;
			y = sin(angle) + pos.second;
			temp = std::make_pair(x, y);
			theLine = std::make_pair(pos,temp);
			pos = temp;
			thePoints.push_back(theLine);
			break;
		case 2:		//Move Forward
			x = cos(angle) + pos.first;
			y = sin(angle) + pos.second;
			pos = std::make_pair(x, y);
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

//3D OpenGl draw function for the map
void renderFunc()
{
	glShadeModel(GL_SMOOTH);
	//Sets background color
	glClearColor(0.0, 0.8, 0.4, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < points.size(); i++)
	{
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(points[i].first.first,points[i].first.second);
		glVertex2f(points[i].second.first,points[i].second.second);
		glEnd();
	}
	glutSwapBuffers();
}

//The initializations necessary to use OpenGL through Glut
void runGL()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("L-Systems Simulation");
	glutDisplayFunc(renderFunc);
	glutMainLoop();
}

//This function initializes values to a preset list of functions
void init(int preset, float & anglesize, string & exrule, string & start, int & iterations, string & description)
{
	switch (preset)
	{
		case 0:	//Dragon Curve
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = 90 * degree;
			iterations = 12;
			description = "Dragon Curve";
			break;
		case 1:	//Tree/Fern thingy
			exrule = "F:FF-[-F+F+F]+[+F-F-F];";
			start = "F";
			anglesize = 22 * degree;
			iterations = 5;
			description = "Vegetation 1";
			break;
		case 2:	//Sierpinsky Hexagonal
			exrule = "A:B-A-B;B:A+B+A;";
			start = "A";
			anglesize = 60 * degree;
			iterations = 6;
			description = "Sierpinsky Gasket 1";
			break;
		case 3:	//Sierpinsky Triangular
			exrule = "F:F-E+F+E-F;E:EE";
			start = "F-E-E";
			anglesize = 120 * degree;
			iterations = 6;
			description = "Sierpinsky Gasket 2";
			break;
		case 4:	//Fractal Plant (Wikipedia)
			exrule = "X:F-[[X]+X]+F[+FX]-X;F:FF;";
			start = "XF";
			anglesize = 25 * degree;
			iterations = 3;
			description = "Vegetation 2";
			break;
		case 5:	//Koch Curve
			exrule = "F:F+F-F-F+F;";
			start = "F";
			anglesize = 90 * degree;
			iterations = 4;
			description = "Koch Curve";
			break;
		case 6:	//Pythagoras Tree
			exrule = "B:BB;A:B[-A]+A;";
			start = "A";
			anglesize = 45 * degree;
			iterations = 6;
			description = "Pythagoras Tree";
			break;
		case 7:	//Snowflake
			exrule = "F:F-F++F-F;X=FF;";
			start = "F++F++F";
			anglesize = 60 * degree;
			iterations = 4;
			description = "Snowflake 1";
			break;
		case 8:	//PentaSnowflake
			exrule = "F:F-F++F+F-F-F;";
			start = "F-F-F-F-F";
			anglesize = 72 * degree;
			iterations = 4;
			description = "Snowflake 2";
			break;
		case 9:	//Hexagonal Gosper
			exrule = "X:X+YF++YF-FX--FXFX-YF+;Y:-FX+YFYF++YF+FX--FX-Y;F:F";
			start = "XF";
			anglesize = 60 * degree;
			iterations = 4;
			description = "Hexagonal Gosper";
			break;
		case 10://Rings
			exrule = "F:FF+F+F+F+F+F-F";
			start = "F+F+F+F";
			anglesize = 90 * degree;
			iterations = 3;
			description = "Rings";
			break;
		default: //Dragon Curve
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = 90 * degree;
			iterations = 12;
			description = "Dragon Curve";
			break;
	}
}

int main(int argc, char *argv[])
{
	//Initial conditions
	string exrule, start, description;
	float anglesize;
	int iterations, preset;
	preset = 0;
	
	std::cout << "Enter the preset to be used.\n (A number between 0 and 10)\n";
	std::cin >> preset;
	if (preset < 0)
		preset = 0;
	init(preset, anglesize, exrule, start, iterations, description);
	std::cout << "This preset is " << description << ".\n";
	int tempit = iterations;
	std::cout << "Enter the number of iterations. \n If zero, default of "<<tempit<<".\n";
	std::cin >> iterations;
	if (iterations <= 0)
		iterations = tempit;

	vector<pair<char, string> > ruleList;
	ruleParser(exrule, ruleList);
	if (!ruleList.empty())
	{
		for (int i = 0; i < ruleList.size(); i++)
		{
			std::cout << ruleList[i].first << " -> " << ruleList[i].second << "\n";
		}
	}
	string output = buildString(start, ruleList, iterations);

	//std::cout << output << "\n";
	pointSetter(points, output, anglesize);
	pair<coord, coord> glCorners;
	glCorners.first = std::make_pair(-1, -1);
	glCorners.second = std::make_pair(1, 1);
	justifyPoints(points, glCorners);
	//Glut Display Function
	glutInit(&argc, argv);
	runGL();

	return 0;
}
