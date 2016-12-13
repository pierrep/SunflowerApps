//
// CSVReader.cpp
//

#include "CSVReader.h"

#include <istream>
#include <fstream>      // std::ifstream
#include <sstream>

using namespace std;


void CSVReader::open(string filename)
{
    file.open(filename, std::ifstream::in);

}

void CSVReader::close()
{
    file.close();
}


std::vector<std::string> CSVReader::getNextLineAndSplitIntoTokens()
{
    if(file.is_open())
    {
        std::vector<std::string>   result;
        std::string                line;
        std::getline(file,line);

        std::stringstream          lineStream(line);
        std::string                cell;

        while(std::getline(lineStream,cell, ','))
        {
            result.push_back(cell);
        }

        return result;
    }
    else {cout << "File not open" << endl;}

    return std::vector<string>();
}
