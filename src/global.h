#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "intervenants.h"
#include "mission.h"

extern Intervenants intervenants;     // liste des intervenants
extern std::vector<Mission> missions; // liste des missions
extern double *distances;             // matrice des distances entre les missions
extern double alphaC;                 // paramètre de la fonction des fonctions d'objectif (100 / nombre total de missions)
extern double betaC;                  // paramètre de la fonction des fonctions d'objectif (100 / nombre total d'heures qu'un intervenant peut travailler par semaine)
extern double gammaC;                 // paramètre de la fonction des fonctions d'objectif (100 / nombre total d'heures supplémentaires tolérées)
extern double zetaC;                  // paramètre de la fonction des fonctions d'objectif (100 / moyenne des heures du quota du travail des employés)
extern double kappaC;                 // paramètre de la fonction des fonctions d'objectif (100 / moyenne de toutes les distances)

#endif