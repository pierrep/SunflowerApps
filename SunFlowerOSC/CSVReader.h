//
// CSVReader.h
//


#ifndef __SunflowerDataService__CSVReader__
#define __SunflowerDataService__CSVReader__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;


class CSVReader
{

public:
    void open(string filename);
    void close();
    std::vector<std::string> getNextLineAndSplitIntoTokens();

private:
    std::ifstream file;


};


#endif /* defined(__SunflowerDataService__CSVReader__) */

