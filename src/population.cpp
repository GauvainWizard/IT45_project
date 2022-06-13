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
		moyenne += individus[i]->critere1;
		ecart_type += individus[i]->critere1 * individus[i]->critere1;
	}
	moyenne = moyenne / taille_pop;
	ecart_type = sqrt(ecart_type / taille_pop - moyenne * moyenne);

	cout << "fitness : (moyenne, ecart_type) -> ("
		 << moyenne << " , " << ecart_type << ")" << endl;
	cout << "fitness : [meilleure, mediane, pire] -> ["
		 << individus[ordre[0]]->critere1 << " , "
		 << individus[ordre[(int)(taille_pop / 2)]]->critere1 << " , "
		 << individus[ordre[taille_pop - 1]]->critere1 << "]" << endl;
}

// Similitude de la population
void population::similitude()
{
	size_t nb_ind_id_1, nb_ind_id_2, nb_ind_id_3;
	nb_ind_id_1 = nb_chromosomes_similaires(individus[ordre[0]]);
	cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[0]]->critere1 << " : " << nb_ind_id_1 << " / " << taille_pop << endl;
	if (nb_ind_id_1 < taille_pop)
	{
		nb_ind_id_2 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1]]);
		cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1]]->critere1 << " : " << nb_ind_id_2 << " / " << taille_pop << endl;
		if (nb_ind_id_1 + nb_ind_id_2 < taille_pop)
		{
			nb_ind_id_3 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1 + nb_ind_id_2]]);
			cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1 + nb_ind_id_2]]->critere1 << " : " << nb_ind_id_3 << " / " << taille_pop << endl;
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
			if (individus[ordre[i]]->critere1 > individus[ordre[j]]->critere1)
				swap(ordre[i], ordre[j]);
}

// R�-ordonne le classement des individus de la population par ordre croissant de fitness
//   apr�s un petit changement
void population::reordonner()
{
	// tri par ordre croissant de fitness
	// for (size_t i = 0; i < taille_pop - 1; ++i)
	// 	for (size_t j = i + 1; j < taille_pop; ++j)
	// 		if (individus[ordre[i]]->critere1 > individus[ordre[j]]->critere1)
	// 			swap(ordre[i], ordre[j]);

	// tri par insertition (plus rapide dans ce cas) pour ré-ordonner les individus de la population
	int inter;
	for (size_t i = 1; i < taille_pop; ++i)
	{
		inter = ordre[i];
		int j = i;
		while (j > 0 && individus[ordre[j - 1]]->critere1 > individus[inter]->critere1)
		{
			ordre[j] = ordre[j - 1];
			--j;
		}
		ordre[j] = inter;
	}
}

void population::reordonner_critere2()
{
	// tri par ordre croissant de critere2
	double best_fitness = individus[ordre[0]]->critere1;
	size_t taille = 0;
	while (best_fitness == individus[ordre[taille]]->critere1)
		++taille;
	for (size_t i = 0; i < taille - 1; ++i)
		for (size_t j = i + 1; j < taille; ++j)
			if (individus[ordre[i]]->critere2 > individus[ordre[j]]->critere2)
				swap(ordre[i], ordre[j]);
}

void population::reordonner_critere3()
{
	// tri par ordre croissant de critere3
	double best_fitness = individus[ordre[0]]->critere1;
	double critere2 = individus[ordre[0]]->critere2;
	size_t taille = 0;
	while (best_fitness == individus[ordre[taille]]->critere1 && critere2 == individus[ordre[taille]]->critere2)
		++taille;
	for (size_t i = 0; i < taille - 1; ++i)
		for (size_t j = i + 1; j < taille; ++j)
			if (individus[ordre[i]]->critere3 > individus[ordre[j]]->critere3)
			{
				cout << "L'individu " << individus[ordre[i]]->critere1 << " " << individus[ordre[i]]->critere2 << " " << individus[ordre[i]]->critere3 << " est plus petit que " << individus[ordre[j]]->critere1 << " " << individus[ordre[j]]->critere2 << " " << individus[ordre[j]]->critere3 << endl;
				swap(ordre[i], ordre[j]);
			}
}

// SELECTION PAR ROULETTE BIAISEE
// op�rateur de s�lection bas� sur la fonction fitness
chromosome *population::selection_roulette()
{
	int somme_fitness = individus[0]->critere1;
	int fitness_max = individus[0]->critere1;
	int somme_portion;

	for (size_t i = 1; i < taille_pop; ++i)
	{
		somme_fitness += individus[i]->critere1;
		if (fitness_max < individus[i]->critere1)
			fitness_max = individus[i]->critere1;
	}
	somme_portion = fitness_max * taille_pop - somme_fitness;

	double variable_alea = Random::aleatoire(1000) / 1000.0;

	size_t ind = 0;
	double portion = (fitness_max - individus[0]->critere1) * 1. / somme_portion;
	while ((ind < taille_pop - 1) && (variable_alea >= portion))
	{
		ind++;
		portion += (fitness_max - individus[ind]->critere1) * 1. / somme_portion;
	}
	return individus[ind];
}

// op�rateur de remplacement bas� sur la roulette biais�e d'un individu de la population
//   par un nouveau individu donn� en argument
void population::remplacement_roulette(chromosome *individu)
{
	int somme_fitness = individus[0]->critere1;
	int fitness_max = individus[0]->critere1;
	int somme_portion;

	for (size_t i = 1; i < taille_pop; ++i)
	{
		somme_fitness += individus[i]->critere1;
		if (fitness_max < individus[i]->critere1)
			fitness_max = individus[i]->critere1;
	}
	somme_portion = fitness_max * taille_pop - somme_fitness;
	size_t ind = (size_t)ordre[0];
	while ((size_t)ordre[0] == ind)
	{
		double variable_alea = Random::aleatoire(1000) / 1000.0;
		ind = 0;
		double portion = (fitness_max - individus[0]->critere1) * 1. / somme_portion;
		while ((ind < taille_pop - 1) && (variable_alea >= portion))
		{
			ind++;
			portion += (fitness_max - individus[ind]->critere1) * 1. / somme_portion;
		}
	}
	individus[ind]->copier(individu);
	individus[ind]->critere1 = individu->critere1;
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
