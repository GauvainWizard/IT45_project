#ifndef _AE_H
#define _AE_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include "population.h"
#include "chromosome.h"
#include "csvreader.h"
#include "enum.h"
#include "intervenant.h"
#include "mission.h"
#include "global.h"

using namespace std;
// La classe Ae d�finie les param�tres d'une ex�cution d'un algorithme
//    �volutionniste ainsi que la proc�dure principale de recherche
class Ae
{
public:
	// ATTRIBUTS
	int nbgenerations;		  // nombre de g�n�rations apr�s quoi la recherche est arr�t�e
	size_t taille_pop;		  // nombre d'individus dans la population
	double taux_croisement;	  // taux de croisement : valeur entre 0 et 1
	double taux_mutation;	  // taux de mutation : valeur entre 0 et 1
	size_t taille_chromosome; // nombre de g�nes dans le chromosome
	population *pop;		  // liste des individus de la population
	int **les_distances;	  // matrice des distances entre les villes

	// CONSTRUCTEURS
	Ae(int nbg, size_t tp, double tcroisement, double tmutation, size_t tc, string nom_dossier);
	// constructeur de l'objet Algaorithme evolutioniste
	~Ae(); // destructeur de l'objet Ae

	// METHODES

	/**
	 * @brief Construit la liste des intervenants
	 * @param nom_dossier : nom du dossier contenant les fichiers CSV
	 */
	void construction_intervenants(string nom_dossier);

	/**
	 * @brief Construit la liste des missions
	 * @param nom_dossier nom du dossier contenant les fichiers CSV
	 */
	void construction_missions(string nom_dossier);

	/**
	 * @brief Construit la liste des distances
	 * @param nom_dossier nom du dossier contenant les fichiers CSV
	 */
	void construction_distance(string nom_dossier);

	// constuie la matrice des distances
	chromosome *optimiser(); // fonction principale qui d�cit le d�roulement de l'algorithme �volusionniste

	// OPERATEURS DE CROISEMENT
	// op�rateur de croisement 1X de deux chromosomes
	void croisement1X(chromosome *parent1, chromosome *parent2,
					  chromosome *enfant1, chromosome *enfant2);
	// op�rateur de croisement 1X de deux chromosomes
	void croisement2X(chromosome *parent1, chromosome *parent2,
					  chromosome *enfant1, chromosome *enfant2);
	// op�rateur de croisement 2LOX de deux chromosomes
	void croisement2LOX(chromosome *parent1, chromosome *parent2,
						chromosome *enfant_s1, chromosome *enfant_s2);
};

#endif
