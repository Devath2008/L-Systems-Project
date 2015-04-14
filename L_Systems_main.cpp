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

//This function will take a string of
void pointSetter(vector<pair<coord, coord> > & thePoints, string & theString)
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
	if (iterations == 0)
		return start;


}

int main(int argc, char *argv[])
{
	string exrule = "X:YZ;Y:XZ;Z:Z";
	vector<pair<char, string> > output;
	ruleParser(exrule, output);
	if (!output.empty())
	{
		for (int i = 0; i < output.size(); i ++)
		{
			std::cout << output[i].first << " -> " << output[i].second << "\n";
		}
	}
	return 0;
}
