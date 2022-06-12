#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * @brief Lis un fichier CSV et renvoie un tableau de string
 * @param path - Chemin du fichier CSV
 * @return vector<vector<string>> contenant les données du fichier
 */
std::vector<std::vector<std::string>> readCSV(std::string path);

#endif // CSVREADER_H