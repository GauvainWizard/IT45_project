#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "intervenants.h"
#include "mission.h"

/**
 * @brief Liste des intervenants
 */
extern Intervenants intervenants;

/**
 * @brief Liste des missions
 */
extern std::vector<Mission> missions;

/**
 * @brief Matrice des distances entre les missions
 */
extern double *distances;

/**
 * @brief Paramètre de la fonction des fonctions d'objectif (100 / nombre total de missions)
 */
extern double alphaC;

/**
 * @brief Paramètre de la fonction des fonctions d'objectif (100 / nombre total d'heures qu'un intervenant peut travailler par semaine)
 */
extern double betaC;

/**
 * @brief Paramètre de la fonction des fonctions d'objectif (100 / nombre total d'heures supplémentaires tolérées)
 */
extern double gammaC;

/**
 * @brief Paramètre de la fonction des fonctions d'objectif (100 / moyenne des heures du quota du travail des employés)
 */
extern double zetaC;

/**
 * @brief Paramètre de la fonction des fonctions d'objectif (100 / moyenne de toutes les distances)
 */
extern double kappaC;

#endif