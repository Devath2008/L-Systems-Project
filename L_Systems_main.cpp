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
#define pi 3.14159265
#define degree 0.0174532925

using std::string;
using std::vector;
using std::pair;
typedef pair<float, float> coord;
vector<pair<coord, coord> > points;

//Finds the index of a coordinate in a vector
int findinVector(vector<float> & theStuff, float & value)
{
	for (int i = 0; i < theStuff.size(); i++)
	{
		if (theStuff[i] == value)
			return i;
	}
	return -1;
}

//Centering Function. Finds the coordinates that are topmmost, bottommost, leftmost, and rightmost
//Uses these values to create an equation that will extend the widest axis to the sides of the window
//Should take into account if the window isn't square
void justifyPoints(vector<pair<coord, coord> > & thePoints, pair<coord,coord> newCorners)
{
	vector<float> theX;
	vector<float> theY;
	for (int i = 0; i < thePoints.size(); i++)
	{
		//X coords in first and second
		if (findinVector(theX, thePoints[i].first.first) == -1)
			theX.push_back(thePoints[i].first.first);
		if (findinVector(theX, thePoints[i].first.second) == -1)
			theX.push_back(thePoints[i].first.second);

		//Y coords in first and second
		if (findinVector(theY, thePoints[i].first.second) == -1)
			theY.push_back(thePoints[i].first.second);
		if (findinVector(theY, thePoints[i].second.second) == -1)
			theY.push_back(thePoints[i].second.second);
	}
	//Sort x coordinates and y coordinates to find the biggest and smallest of both
	std::sort(theX.begin(), theX.end());
	std::sort(theY.begin(), theY.end());
	
	//Find the magnitudes of the edges of the old box and the new box
	float xmag, ymag, boxmag;
	xmag = std::abs(theX[0] - theX[theX.size() - 1]);
	ymag = std::abs(theY[0] - theY[theY.size() - 1]);
	boxmag = std::abs(newCorners.first.first - newCorners.second.first);
	
	//This is the size of the line length increment
	float ratio = boxmag / (std::max(xmag, ymag)+4);

	//Find the centers of the old box and the new box
	pair<float, float> oldCenter, newCenter, offset;
	oldCenter.first = xmag / 2 + theX[0];
	oldCenter.second = ymag / 2 + theY[0];
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
	std::stack<coord> storedPos;
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
			storedPos.push(pos);
			break;
		case 7:		//Restore Stored Location and angle
			pos = storedPos.top();
			storedPos.pop();
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

void init(int preset, float & anglesize, string & exrule, string & start)
{
	switch (preset)
	{
		case 0:
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = pi / 2;
			break;
		case 1:
			exrule = "F:FF-[-F+F+F]+[+F-F-F];";
			start = "F";
			anglesize = 22 * degree;
			break;
		case 2:
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = pi / 2;
			break;
		case 3:
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = pi / 2;
			break;
		default:
			exrule = "X:X+YF+;Y:-FX-Y;";
			start = "FX";
			anglesize = pi / 2;
			break;
	}
}



int main(int argc, char *argv[])
{
	//Initial conditions
	string exrule, start;
	float anglesize;
	init(1, anglesize, exrule, start);

	vector<pair<char, string> > ruleList;
	ruleParser(exrule, ruleList);
	if (!ruleList.empty())
	{
		for (int i = 0; i < ruleList.size(); i++)
		{
			std::cout << ruleList[i].first << " -> " << ruleList[i].second << "\n";
		}
	}
	int iterations = 5;
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
