#include "cdf.h"

#include <iostream>  // std::cout, std::endl, std::cin
#include <fstream>   // std::ifstream
#include <algorithm> // std::count
#include <typeinfo>  // debugging

using namespace std;

cdf::DataFrame::DataFrame ( string _fileName, char _delimiter )
{
	// Open CSV file
	ifstream csvFile(_fileName);
	// Check whether state of stream is not good
	if (!csvFile.good())
	{
		cout << cdf::ALERT_BEGIN << "Filename is invalid!" << cdf::ALERT_END << endl;
		return;
	}
	// Find out number of fields per line
	string headersLine;
	getline(csvFile, headersLine);
	int countDelimiters = count(headersLine.begin(), headersLine.end(), _delimiter);
	dfDimension[0] = countDelimiters+1;
	// Save headers
	string header = "";
	dfHeaders.reserve( countDelimiters+1 );
	for (auto i = headersLine.begin(); i < headersLine.end(); i++)
	{
		// EXAMPLE1, EXAMPLE2, example3
		if ((*i) == _delimiter)
		{
			dfHeaders.push_back(header);
			header = "";
		}
		// example1, example2, EXAMPLE3
		else if (i == headersLine.end()-1)
		{
			header += (*i);
			dfHeaders.push_back(header);
			header = "";
		}
		else
			header += (*i);
	}

	size_t dfDimensionY = 0;
	string &fileLine = headersLine;
	string &line = header;
	std::string::size_type sz;     // alias of size_t
	double value;
	while ( getline(csvFile, fileLine) )
	{
		dfData.resize( ++dfDimensionY );
		dfData[ dfDimensionY-1 ].reserve( dfDimension[0] );

		for (auto i = fileLine.begin(); i < fileLine.end(); i++)
		{
			// EXAMPLE1, EXAMPLE2, example3
			if ((*i) == _delimiter)
			{
				value = std::stod(line, &sz);
				dfData[ dfDimensionY-1 ].push_back( value );
				line = "";
			}
			// example1, example2, EXAMPLE3
			else if (i == headersLine.end()-1)
			{
				line += (*i);
				value = std::stod(line, &sz);
				dfData[ dfDimensionY-1 ].push_back( value );
				line = "";
			}
			else
				line += (*i);
		}
	}
	dfDimension[1] = dfDimensionY;

	debugging();
}


void cdf::DataFrame::debugging ()
{
	cout << "--X: " << dfDimension[0] << endl;
	cout << "--Y: " << dfDimension[1] << endl;
	// DEBUGGING
	cout << "--Headers: ";
	for (size_t i = 0; i < dfHeaders.size(); i++)
		cout << dfHeaders[i] << ";";
	cout << endl << "--Data: " << endl;
	for (size_t i = 0; i < dfDimension[1]; i++)
	{
		for (size_t j = 0; j < dfDimension[0]; j++)
			cout << dfData[i][j] << ";";
		cout << endl;
	}
}