// Alexander Lease
// L-Systems rendering simulation
//
// Created 04/13/2015
#include <vector>       //std::vector
#include <string>		//std::getline
#include <sstream>      //std::stringstream
#include <iostream>		//std::cin,std::cout
using std::string;
using std::vector;
using std::pair;
typedef pair<int, int> coord;

//This function will take a string of characters and will create a list of coordinate pairs for line creating
//This will operate with predetermined meanings for the characters in the sequence
void pointSetter(vector<pair<coord, coord> > & thePoints, string & theString, int anglesize)
{

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
		output += rules[transform].second;
	}
	iterations--;
	built = buildString(output, rules, iterations);
	return built;
}

int main(int argc, char *argv[])
{
	string exrule = "X:YY;Y:ZXY;Z:Z";
	string start = "X";
	vector<pair<char, string> > ruleList;
	ruleParser(exrule, ruleList);
	if (!ruleList.empty())
	{
		for (int i = 0; i < ruleList.size(); i++)
		{
			std::cout << ruleList[i].first << " -> " << ruleList[i].second << "\n";
		}
	}
	int iterations = 2;
	string output = buildString(start, ruleList, iterations);
	std::cout << output << "\n";
	return 0;
}
