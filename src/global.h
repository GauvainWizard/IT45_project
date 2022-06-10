#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "intervenants.h"
#include "mission.h"

using namespace std;

extern Intervenants intervenants; // liste des intervenants
extern vector<Mission> missions;  // liste des missions
extern double *distances;         // matrice des distances entre les missions

#endif