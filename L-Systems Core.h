/*
*  L-Systems Core.h
*  Created on: April 30, 2015
*  Author: Alexander Lease
*  Header for L-Systems Simulation
*/
#ifndef LSYSTEMS_H_
#define LSYSTEMS_H_

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
#ifndef degree
#define degree 0.0174532925
#endif	//degree
using std::string;
using std::vector;
using std::pair;

struct point
{
	float x, y;
	float r, g, b;

	point(float nx, float ny, float nr, float ng, float nb)
		:x(nx), y(ny), r(nr), g(ng), b(nb)
	{}

	point()
		:x(0.0), y(0.0), r(0.0), g(0.0), b(0.0)
	{}
};

//Centering Function. Finds the coordinates that are topmmost, bottommost, leftmost, and rightmost
//Uses these values to create an equation that will extend the widest axis to the sides of the window
//Should take into account if the window isn't square
void justifyPoints(vector<pair<point, point> > & thePoints, pair<point, point> newCorners);

//This function takes the character and returns a number corresponding to one action that can be taken
//Current number of actions: 7
//0 is used as a null output
int actionValue(char theChar);

//This function sets the color of the point indicated based on the index of the color that is handed to it
//The color presets are 1=Cyan, 2=Blue, 3=Red, 4=Purple, 5=Blue-Green, 6=White, 7=Dark Green, 8=Light Green, 9=Brown, 0=Orange
void setColor(point & thepoint, char colorindex);

//This function will take a string of characters and will create a list of coordinate pairs for line creating
//This will operate with predetermined meanings for the characters in the sequence
void pointSetter(vector<pair<point, point> > & thePoints, string & theString, float anglesize);

//Checks for the value in the vector and returns the index of that value
int findinVector(vector<pair<char, string> > & theStuff, char & value);

//This is a function that parses the rules string into a list of
//key-value pairs for use in buildString
//Assumes that the rules string is bbuilt using the format
//Variable ":" Replacement ";" Next rule, with no spaces in the formula
void ruleParser(string & rules, vector<pair<char, string> > &  ruleList);

//The find function assumes there is only one rule for each character
//Rules are parsed into key-value pairs
string buildString(string & start, vector<pair<char, string> > & rules, int & iterations);

//Used to draw in the Windows API
void gdiRenderFunc(vector<pair<point, point> > & thePoints, HDC hdc);

//This function initializes values to a preset list of functions
void init(int preset, float & anglesize, string & exrule, string & start, int & iterations, string & description);

#endif /* LSYSTEMS_H_ */
