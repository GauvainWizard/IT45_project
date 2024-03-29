#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "ae.h"
#include "chromosome.h"
#include "random.h"

using namespace std;

int main(int argc, char **argv)
// argc : nombre de parametres
// argv : tableau contenant les parametres
// Soit l'executable 'algo_evo' ne prend pas d'arguments soit il prend 6 arguments :
//    1. nombre de generation (entier > 0)
//    2. taille de la population (entier > 0)
//    3. taux de croisement (0 <= reel <= 1)
//    4. taux de mutation (0 <= reel <= 1)
//    5. nombre de villes (=taille d'un chromosome)
//    6. nom du fichier indiquant les distances entre villes
{
	// Initialise le générateur de nombre aléatoire
	Random::randomize();

	double temps_max = 20;
	size_t taille_population = 25;
	float taux_croisement = 0.75;
	float taux_mutation = 0.3;
	size_t taille_chromosome = 100;
	string folderInstance = "instances/100-10";

	if (argc == 7)
	{
		temps_max = atoi(argv[1]);
		taille_population = atoi(argv[2]);
		taux_croisement = atof(argv[3]);
		taux_mutation = atof(argv[4]);
		taille_chromosome = atoi(argv[5]);
		folderInstance = argv[6];
	}
	else if (argc == 1)
		cout << "Parametres par default" << endl;
	else
	{
		cout << "Nombre d'arguments n'est pas correct." << endl;
		cout << "Soit l'executable 'algo_evo' ne prend pas d'arguments soit il prend 6 arguments : " << endl;
		cout << "   1. nombre de g�n�ration (entier > 0)" << endl;
		cout << "   2. taille de la population (entier > 0)" << endl;
		cout << "   3. taux de croisement (0 <= reel <= 1)" << endl;
		cout << "   4. taux de mutation (0 <= reel <= 1)" << endl;
		cout << "   5. nombre de villes (=taille d'un chromosome)" << endl;
		cout << "   6. nom du dossier indiquant l'instance" << endl;
		exit(EXIT_FAILURE);
	}

	// On parcourt les missions du fichier CSV
	auto start = std::chrono::high_resolution_clock::now();
	// Initialise l'algorithme �volutionniste
	Ae algo(temps_max, taille_population, taux_croisement, taux_mutation, taille_chromosome, folderInstance);
	// arguments du constructeur de l'objet Ae
	//    1. nombre de g�n�ration (entier > 0)
	//    2. taille de la population (entier > 0)
	//    3. taux de croisement (0 <= reel <= 1)
	//    4. taux de mutation (0 <= reel <= 1)
	//    5. nombre de villes (=taille d'un chromosome)
	//    6. nom du dossier indiquant l'instance

	// Lancer l'algorithme évolutionniste
	chromosome *best = algo.optimiser();

	// Lancer chronomètre
	auto end = chrono::high_resolution_clock::now();

	chrono::duration<double> diff = end - start;
	cout << "Temps d'execution : " << std::setprecision(9) << diff.count() << "s" << endl;

	// Affiche la fitness du meilleur individu trouvé
	cout << "La meilleure solution trouvee est : ";

	best->afficher();
}
