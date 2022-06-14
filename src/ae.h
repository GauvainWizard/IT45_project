#ifndef _AE_H
#define _AE_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <chrono>
#include "population.h"
#include "chromosome.h"
#include "csvreader.h"
#include "enum.h"
#include "intervenant.h"
#include "mission.h"
#include "global.h"

using namespace std;

/**
 * @class Ae
 * @brief Définie les paramètres d'une exécution d'un algorithme évolutionniste ainsi que la procédure principale de recherche
 */
class Ae
{
public:
	/**
	 * @brief Temps d'exécution maximum de l'algorithme
	 */
	double temps_max;

	/**
	 * @brief Nombre d'individus dans la population
	 */
	size_t taille_pop;

	/**
	 * @brief Taux de croisement : valeur réelle entre 0 et 1
	 */
	double taux_croisement;

	/**
	 * @brief Taux de mutation : valeur réelle entre 0 et 1
	 */
	double taux_mutation;

	/**
	 * @brief Nombre de gènes dans le chromosome
	 */
	size_t taille_chromosome;

	/**
	 * @brief Liste des individus de la population
	 */
	population *pop;

	/**
	 * @brief Constructeur
	 * @param tp_max - temps d'exécution maximum de l'algorithme
	 * @param tp - Taille de la population
	 * @param tcroisement - Taux de croisement
	 * @param tmutation - Taux de mutation
	 * @param tc - Taille du chromosome
	 * @param nom_dossier - Dossier comprenant les instances d'initialisation
	 */
	Ae(double tp_max, size_t tp, double tcroisement, double tmutation, size_t tc, string nom_dossier);

	/**
	 * @brief Destructeur
	 */
	~Ae();

	/**
	 * @brief Construit la liste des intervenants
	 * @param nom_dossier : nom du dossier contenant les fichiers CSV
	 */
	void construction_intervenants(const string nom_dossier);

	/**
	 * @brief Construit la liste des missions
	 * @param nom_dossier nom du dossier contenant les fichiers CSV
	 */
	void construction_missions(const string nom_dossier);

	/**
	 * @brief Construit la liste des distances
	 * @param nom_dossier nom du dossier contenant les fichiers CSV
	 */
	void construction_distance(const string nom_dossier);

	/**
	 * @brief Construit les facteurs de correlation
	 * @param nom_dossier nom du dossier contenant les fichiers CSV
	 */
	void construction_correlation(const string nom_dossier);

	/**
	 * @brief Construit la matrice des distances, Déroule l'algorithme génétique
	 */
	chromosome *optimiser();

	/**
	 * @brief Opérateur de croisement 1X de deux chromosomes
	 * @param parent1 - Référence sur un chromosome générant deux chromosomes enfants
	 * @param parent2 - Référence sur un chromosome générant deux chromosomes enfants
	 * @param enfant1 - Référence sur un chromosome issus du croisement
	 * @param enfant2 - Référence sur un chromosome issus du croisement
	 */
	void croisement1X(chromosome &parent1, chromosome &parent2,
					  chromosome &enfant1, chromosome &enfant2);

	/**
	 * @brief Opérateur de croisement 2X de deux chromosomes
	 * @param parent1 - Référence sur un chromosome générant deux chromosomes enfants
	 * @param parent2 - Référence sur un chromosome générant deux chromosomes enfants
	 * @param enfant1 - Référence sur un chromosome issus du croisement
	 * @param enfant2 - Référence sur un chromosome issus du croisement
	 */
	void croisement2X(chromosome &parent1, chromosome &parent2,
					  chromosome &enfant1, chromosome &enfant2);
};

#endif
