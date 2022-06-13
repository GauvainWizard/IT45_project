#include "population.h"

using namespace std;

// initialisation d'une population de solutions
population::population(size_t tp, size_t tc)
{
	taille_pop = tp;
	individus = new chromosome *[taille_pop];
	for (size_t i = 0; i < taille_pop; ++i)
		individus[i] = new chromosome(tc);
	ordre = new int[taille_pop];
}

// destruction de l'objet "population"
population::~population()
{
	for (size_t i = 0; i < taille_pop; ++i)
		delete individus[i];
	delete individus;
	delete ordre;
}

// statistiques sur la population
void population::statistiques()
{
	double moyenne = 0;
	double ecart_type = 0;

	for (size_t i = 0; i < taille_pop; ++i)
	{
		moyenne += individus[i]->fitness;
		ecart_type += individus[i]->fitness * individus[i]->fitness;
	}
	moyenne = moyenne / taille_pop;
	ecart_type = sqrt(ecart_type / taille_pop - moyenne * moyenne);

	cout << "fitness : (moyenne, ecart_type) -> ("
		 << moyenne << " , " << ecart_type << ")" << endl;
	cout << "fitness : [meilleure, mediane, pire] -> ["
		 << individus[ordre[0]]->fitness << " , "
		 << individus[ordre[(int)(taille_pop / 2)]]->fitness << " , "
		 << individus[ordre[taille_pop - 1]]->fitness << "]" << endl;
}

// Similitude de la population
void population::similitude()
{
	size_t nb_ind_id_1, nb_ind_id_2, nb_ind_id_3;
	nb_ind_id_1 = nb_chromosomes_similaires(individus[ordre[0]]);
	cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[0]]->fitness << " : " << nb_ind_id_1 << " / " << taille_pop << endl;
	if (nb_ind_id_1 < taille_pop)
	{
		nb_ind_id_2 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1]]);
		cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1]]->fitness << " : " << nb_ind_id_2 << " / " << taille_pop << endl;
		if (nb_ind_id_1 + nb_ind_id_2 < taille_pop)
		{
			nb_ind_id_3 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1 + nb_ind_id_2]]);
			cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1 + nb_ind_id_2]]->fitness << " : " << nb_ind_id_3 << " / " << taille_pop << endl;
		}
	}
}

// compte le nombre de chromosomes similaires
size_t population::nb_chromosomes_similaires(chromosome *chro)
{
	size_t nb = 0;
	for (size_t i = 0; i < taille_pop; ++i)
		if (chro->identique(individus[i]))
			++nb;
	return nb;
}

// Oronne les individus de la population par ordre croissant de fitness
void population::ordonner()
{
	for (size_t i = 0; i < taille_pop; ++i)
		ordre[i] = i;

	// tri par ordre croissant de fitness
	for (size_t i = 0; i < taille_pop - 1; ++i)
		for (size_t j = i + 1; j < taille_pop; ++j)
			if (individus[ordre[i]]->fitness > individus[ordre[j]]->fitness)
				swap(ordre[i], ordre[j]);
}

// R�-ordonne le classement des individus de la population par ordre croissant de fitness
//   apr�s un petit changement
void population::reordonner()
{
	// tri par ordre croissant de fitness
	// for (size_t i = 0; i < taille_pop - 1; ++i)
	// 	for (size_t j = i + 1; j < taille_pop; ++j)
	// 		if (individus[ordre[i]]->fitness > individus[ordre[j]]->fitness)
	// 			swap(ordre[i], ordre[j]);

	// tri par insertition (plus rapide dans ce cas) pour ré-ordonner les individus de la population
	int inter;
	for (size_t i = 1; i < taille_pop; ++i)
	{
		inter = ordre[i];
		int j = i;
		while (j > 0 && individus[ordre[j - 1]]->fitness > individus[inter]->fitness)
		{
			ordre[j] = ordre[j - 1];
			--j;
		}
		ordre[j] = inter;
	}
}

// SELECTION PAR ROULETTE BIAISEE
// op�rateur de s�lection bas� sur la fonction fitness
chromosome *population::selection_roulette()
{
	int somme_fitness = individus[0]->fitness;
	int fitness_max = individus[0]->fitness;
	int somme_portion;

	for (size_t i = 1; i < taille_pop; ++i)
	{
		somme_fitness += individus[i]->fitness;
		if (fitness_max < individus[i]->fitness)
			fitness_max = individus[i]->fitness;
	}
	somme_portion = fitness_max * taille_pop - somme_fitness;

	double variable_alea = Random::aleatoire(1000) / 1000.0;

	size_t ind = 0;
	double portion = (fitness_max - individus[0]->fitness) * 1. / somme_portion;
	while ((ind < taille_pop - 1) && (variable_alea >= portion))
	{
		ind++;
		portion += (fitness_max - individus[ind]->fitness) * 1. / somme_portion;
	}
	return individus[ind];
}

// op�rateur de remplacement bas� sur la roulette biais�e d'un individu de la population
//   par un nouveau individu donn� en argument
void population::remplacement_roulette(chromosome *individu)
{
	int somme_fitness = individus[0]->fitness;
	int fitness_max = individus[0]->fitness;
	int somme_portion;

	for (size_t i = 1; i < taille_pop; ++i)
	{
		somme_fitness += individus[i]->fitness;
		if (fitness_max < individus[i]->fitness)
			fitness_max = individus[i]->fitness;
	}
	somme_portion = fitness_max * taille_pop - somme_fitness;
	size_t ind = (size_t)ordre[0];
	while ((size_t)ordre[0] == ind)
	{
		double variable_alea = Random::aleatoire(1000) / 1000.0;
		ind = 0;
		double portion = (fitness_max - individus[0]->fitness) * 1. / somme_portion;
		while ((ind < taille_pop - 1) && (variable_alea >= portion))
		{
			ind++;
			portion += (fitness_max - individus[ind]->fitness) * 1. / somme_portion;
		}
	}
	individus[ind]->copier(individu);
	individus[ind]->fitness = individu->fitness;
}

// affichage de la population, de son rang et de sa fitness
void population::afficher()
{
	cout << "Poputalion de " << taille_pop << " individus :" << endl;
	for (size_t i = 0; i < taille_pop; ++i)
	{
		cout << "individu " << i << ", rang : " << ordre[i] << " ";
		individus[i]->afficher();
	}
}
