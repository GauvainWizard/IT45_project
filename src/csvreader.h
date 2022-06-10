#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

std::vector<std::vector<std::string>> readCSV(std::string path);

#endif // CSVREADER_H