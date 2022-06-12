#include "csvreader.h"
vector<vector<string>> readCSV(string path)
{
    vector<vector<string>> table;
    vector<string> row;
    string line, word;

    fstream file(path, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            table.push_back(row);
        }
    }
    else
    {
        cout << "Error opening file" << endl;
    }
    file.close();
    return table;
}