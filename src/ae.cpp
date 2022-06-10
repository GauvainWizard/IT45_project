#include "ae.h"

vector<Intervenant> intervenants; // liste des intervenants
vector<Mission> missions;		  // liste des missions
double *distances;				  // matrice des distances entre les missions

using namespace std;

// initialisation des param�tres de l'AG et g�n�ration de la population initiale
Ae::Ae(int nbg, int tp, double tcroisement, double tmutation, int tc, string nom_dossier)
{
	nbgenerations = nbg;
	taille_pop = tp;
	taux_croisement = tcroisement;
	taux_mutation = tmutation;
	taille_chromosome = tc;
	construction_intervenants(nom_dossier);
	construction_missions(nom_dossier);
	construction_distance(nom_dossier);
	pop = new population(taille_pop, taille_chromosome);
}

// destructeur de l'objet Ae
Ae::~Ae()
{
	delete pop;
}

// proc�dure principale de la recherche
chromosome *Ae::optimiser()
{
	int amelioration = 0;
	chromosome *fils1 = new chromosome(taille_chromosome);
	chromosome *fils2 = new chromosome(taille_chromosome);
	chromosome *pere1;
	chromosome *pere2;
	int best_fitness;

	// �valuation des individus de la population initiale
	for (int ind = 0; ind < taille_pop; ind++)
		pop->individus[ind]->evaluer(les_distances);

	// on ordonne les indivudus selon leur fitness
	pop->ordonner();

	best_fitness = pop->individus[pop->ordre[0]]->fitness;
	//  on affiche les statistiques de la population initiale
	cout << "Quelques statistiques sur la population initiale" << endl;
	pop->statiatiques();

	// tant que le nombre de g�n�rations limite n'est pas atteint
	for (int g = 0; g < nbgenerations; g++)
	{
		// s�lection de deux individus de la population courante
		pere1 = pop->selection_roulette();
		pere2 = pop->selection_roulette();

		// On effectue un croisementavec une probabilit� "taux_croisement"
		if (Random::aleatoire(1000) / 1000.0 < taux_croisement)
		{
			croisement1X(pere1, pere2, fils1, fils2);
		}
		else
		{
			fils1->copier(pere1);
			fils2->copier(pere2);
		}

		// On effectue la mutation d'un enfant avec une probabilit� "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils1->echange_2_genes_consecutifs();

		// On effectue la mutation de l'autre enfant avec une probabilit� "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils2->echange_2_genes_consecutifs();

		// �valuation des deux nouveaux individus g�n�r�s
		fils1->evaluer(les_distances);
		fils2->evaluer(les_distances);

		// Insertion des nouveaux individus dans la population
		pop->remplacement_roulette(fils1);
		pop->remplacement_roulette(fils2);

		// On r�ordonne la population selon la fitness
		pop->reordonner();

		// Si l'un des nouveaux indivudus-solutions est le meilleur jamais rencont�
		if (pop->individus[pop->ordre[0]]->fitness < best_fitness)
		{
			best_fitness = pop->individus[pop->ordre[0]]->fitness;
			cout << "Amelioration de la meilleure solution a la generation " << g << " : " << best_fitness << endl;
			amelioration = g;
		}
	}
	//  on affiche les statistiques de la population finale
	cout << "Quelques statistiques sur la population finale" << endl;
	pop->statiatiques();
	//  on affiche la consanginit� de la population finale
	pop->similitude();

	// retourner le meilleur individu rencontr� pendant la recherche
	return pop->individus[pop->ordre[0]];
}

// op�rateur de croisement � un point : croisement 1X
// 1) l'op�rateur 1X choisit de mani�re al�atoire un point de croisement
// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
//                     et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
//    Le 1ier fils est le produit de la partie haute du premier parent et
//    de la partie basse du deuxi�me parent et inversement pour le 2�me fils
void Ae::croisement1X(chromosome *parent1, chromosome *parent2,
					  chromosome *enfant1, chromosome *enfant2)
{
	int nb_genes = parent1->taille;

	int *odre_parent1 = new int[nb_genes];
	int *odre_parent2 = new int[nb_genes];

	for (int i = 0; i < nb_genes; i++)
	{
		odre_parent1[parent1->genes[i]] = i;
		odre_parent2[parent2->genes[i]] = i;
	}

	// 1) l'op�rateur 1X choisit de mani�re al�atoire le point de croisement
	int point = Random::aleatoire(nb_genes);

	// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
	//                     et le d�but du parent 2 au d�but de l'enfant 2.
	enfant1->copier(parent1);
	enfant2->copier(parent2);

	// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
	//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
	for (int k = point + 1; k < nb_genes; k++)
	{
		for (int l = k + 1; l < nb_genes; l++)
		{
			if (odre_parent2[enfant1->genes[k]] > odre_parent2[enfant1->genes[l]])
				enfant1->echange_2_genes(k, l);
			if (odre_parent1[enfant2->genes[k]] > odre_parent1[enfant2->genes[l]])
				enfant2->echange_2_genes(k, l);
		}
	}
	delete[] odre_parent1;
	delete[] odre_parent2;
}

// op�rateur de croisement � deux points : croisement 2X
// 1) l'op�rateur 2X choisit de mani�re al�atoire 2 points de croisement
// 2) l'op�rateur 2X recopie le d�but du parent 1 au d�but de l'enfant 1
//                        et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 2X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
void Ae::croisement2X(chromosome *parent1, chromosome *parent2,
					  chromosome *enfant_s1, chromosome *enfant_s2)
{
}

void Ae::croisement2LOX(chromosome *parent1, chromosome *parent2,
						chromosome *enfant_s1, chromosome *enfant_s2)
{
}

void Ae::construction_distance(string nom_dossier)
{
	// Concatenation du nom du dossier et du nom du fichier
	string path = nom_dossier + "/Distances.csv";

	// allocation de la matrice des distances
	distances = new double[taille_chromosome * taille_chromosome];

	// lecture du CSV intervenants
	vector<vector<string>> files = readCSV(path);

	// construction de la matrice des distances
	for (int i = 0; i < taille_chromosome; i++)
	{
		for (int j = 0; j < taille_chromosome; j++)
		{
			distances[i * taille_chromosome + j] = stod(files[i][j].c_str());
		}
	}
}
void Ae::construction_intervenants(string nom_dossier)
{
	// Concatenation du nom du dossier et du nom du fichier
	string path = nom_dossier + "/Intervenants.csv";

	// Sans correspond à la spécialité SANS
	// Jardinage correspond à la spécialité JARDINAGE
	// Menuiserie correspond à la spécialité MENUISERIE
	// Electricite correspond à la spécialité ELECTRICITE
	// Mecanique correspond à la spécialité MECANIQUE
	// Musique correspond à la spécialité MUSIQUE

	string specialites[6] = {"Sans", "Jardinage", "Menuiserie", "Electricite", "Mecanique", "Musique"};

	// 24 correspond au temps PARTIEL
	// 35 correspond au temps PLEIN

	string temps[2] = {"24", "35"};

	// LSF correspond à la compétence SIGNES
	// LPC correspond à la compétence CODAGE

	string competences[2] = {"LSF", "LPC"};

	// lecture du CSV intervenants
	vector<vector<string>> files = readCSV(path);

	// on parcourt les Intervenants du fichier CSV

	for (size_t i = 0; i < files.size(); i++)
	{
		// On récupère la compétence de l'intervenant
		string competence = files[i][1];

		// On initialise la compétence de l'intervenant
		Competence competenceIntervenant;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; j++)
			if (competence == competences[j])
				competenceIntervenant = static_cast<Competence>(j);

		// On récupère la spécialité de l'intervenant
		string specialite = files[i][2];

		// On initialise la spécialité de l'intervenant
		Specialite specialiteIntervenant;

		// On parcourt les spécialités possible et on affecte la spécialité correspondante
		for (size_t j = 0; j < 6; j++)
			if (specialite == specialites[j])
				specialiteIntervenant = static_cast<Specialite>(j);

		// On récupère le temps de travail de l'intervenant
		string tempsTravail = files[i][3];

		// On initialise la compétence de l'intervenant
		Temps tempsIntervenant;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; j++)
			if (tempsTravail == temps[j])
				tempsIntervenant = static_cast<Temps>(j);

		// crée un intervenant
		Intervenant intervenant(competenceIntervenant, specialiteIntervenant, tempsIntervenant);

		// ajoute l'intervenant à la liste à la fin
		intervenants.push_back(intervenant);
	}
}

void Ae::construction_missions(string nom_dossier)
{
	// Concatenation du nom du dossier et du nom du fichier
	string path = nom_dossier + "/Missions.csv";

	// Sans correspond à la compétence SANS
	// Jardinage correspond à la compétence JARDINAGE
	// Menuiserie correspond à la compétence MENUISERIE
	// Electricite correspond à la compétence ELECTRICITE
	// Mecanique correspond à la compétence MECANIQUE
	// Musique correspond à la compétence MUSIQUE

	string specialites[6] = {"Sans", "Jardinage", "Menuiserie", "Electricite", "Mecanique", "Musique"};

	// 24 correspond au temps PARTIEL
	// 35 correspond au temps PLEIN

	string temps[2] = {"24", "35"};

	// LSF correspond à la compétence SIGNES
	// LPC correspond à la compétence CODAGE

	string competences[2] = {"LSF", "LPC"};

	// lecture du CSV missions
	vector<vector<string>> files = readCSV(path);

	// on parcourt les missions du fichier CSV

	for (size_t i = 0; i < files.size(); i++)
	{
		// On récupère le jour de la mission
		string jour = files[i][1];

		// On initialise le jour de la mission
		Jour jourMission = static_cast<Jour>(stoi(jour) - 1);

		// On récupère les horaires de travail de la mission

		string horaires[2];

		for (size_t j = 0; j < 2; j++)
			horaires[j] = files[i][j + 2];

		// On initialise les horaires de travail de la mission
		int horairesMission[2];

		// On convertit les horaires de travail de la mission en int
		for (size_t j = 0; j < 2; j++)
			horairesMission[j] = stoi(horaires[j]);

		// On récupère la compétence de la mission
		string competence = files[i][4];

		// On initialise la compétence de la mission
		Competence competenceMission;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; j++)
			if (competence == competences[j])
				competenceMission = static_cast<Competence>(j);

		// On récupère la spécialité de la mission
		string specialite = files[i][5];

		// On initialise la spécialité de la mission
		Specialite specialiteMission;

		// On parcourt les spécialités possible et on affecte la spécialité correspondante
		for (size_t j = 0; j < 6; j++)
			if (specialite == specialites[j])
				specialiteMission = static_cast<Specialite>(j);

		// Créer une mission
		Mission mission(competenceMission, specialiteMission, jourMission, horairesMission);

		// ajoute la mission à la liste à la fin
		missions.push_back(mission);
	}
}