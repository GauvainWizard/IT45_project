#include "ae.h"
#include <iomanip>

/**
 * @brief Variables globales du fichier "global.h"
 */
Intervenants intervenants;
vector<Mission> missions;
double *distances;
double alphaC;
double betaC;
double gammaC;
double zetaC;
double kappaC;

using namespace std;

Ae::Ae(double tp_max, size_t tp, double tcroisement, double tmutation, size_t tc, string nom_dossier)
{
	temps_max = tp_max;
	taille_pop = tp;
	taux_croisement = tcroisement;
	taux_mutation = tmutation;
	taille_chromosome = tc;
	construction_intervenants(nom_dossier);
	construction_missions(nom_dossier);
	construction_distance(nom_dossier);
	construction_correlation(nom_dossier);
	pop = new population(taille_pop, taille_chromosome);
}

Ae::~Ae()
{
	delete pop;
}

chromosome *Ae::optimiser()
{
	size_t amelioration = 0;
	chromosome *fils1 = new chromosome(taille_chromosome);
	chromosome *fils2 = new chromosome(taille_chromosome);
	chromosome *pere1;
	chromosome *pere2;
	double best_fitness;

	// Evaluation des individus de la population initiale
	for (size_t ind = 0; ind < taille_pop; ++ind)
		pop->individus[ind]->evaluer();

	// On ordonne les indivudus selon leur fitness
	pop->ordonner();

	best_fitness = pop->individus[pop->ordre[0]]->critere1;
	// On affiche les statistiques de la population initiale
	cout << "Quelques statistiques sur la population initiale" << endl;
	pop->statistiques();
	bool pass = false;

	// Lancement du chrono après initialisation de la population intiale
	auto start = std::chrono::system_clock::now();

	// Nombre de générations réalisées
	size_t g = 0;

	// Boucle pendant la durée du temps d'exécution maximal
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() < temps_max * 1000)
	{
		// Selection de deux individus de la population courante
		pere1 = pop->selection_roulette();
		pere2 = pop->selection_roulette();
		pass = false;
		// On effectue un croisementavec une probabilite "taux_croisement"
		if (Random::aleatoire(1000) / 1000.0 < taux_croisement)
		{
			// Random croisement1X ou croisement2X
			croisement1X(*pere1, *pere2, *fils1, *fils2);
			pass = true;
		}
		if (Random::aleatoire(1000) / 1000.0 < taux_croisement)
		{
			// Random croisement1X ou croisement2X
			croisement2X(*pere1, *pere2, *fils1, *fils2);
			pass = true;
		}
		if (!pass)
		{
			fils1->copier(pere1);
			fils2->copier(pere2);
		}

		// On effectue la mutation d'un enfant avec une probabilite "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils1->echange_2_genes_consecutifs();

		// On effectue la mutation de l'autre enfant avec une probabilite "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils2->echange_2_genes_consecutifs();

		// On effectue la mutation d'un enfant avec une probabilite "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils1->echange_2_genes_quelconques();

		// On effectue la mutation de l'autre enfant avec une probabilite "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils2->echange_2_genes_quelconques();

		// On effectue la mutation d'un enfant avec une probabilite "taux_mutation"
		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils1->deplacement_1_gene();

		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils2->deplacement_1_gene();

		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils1->inversion_sequence_genes();

		if (Random::aleatoire(1000) / 1000.0 < taux_mutation)
			fils2->inversion_sequence_genes();

		// Evaluation des deux nouveaux individus generes
		if (fils1->evaluer())
			pop->remplacement_roulette(fils1);
		if (fils2->evaluer())
			pop->remplacement_roulette(fils2);
		// On reordonne la population selon la fitness
		pop->reordonner();
		// Si l'un des nouveaux indivudus-solutions est le meilleur jamais renconte
		if (pop->individus[pop->ordre[0]]->critere1 < best_fitness)
		{
			taux_mutation = taux_mutation;
			taux_croisement = taux_croisement;
			best_fitness = pop->individus[pop->ordre[0]]->critere1;
			cout << "Amelioration de la meilleure solution a la generation " << g << " : " << best_fitness << endl;
			chrono::duration<double> diff = std::chrono::system_clock::now() - start;
			cout << "Le temps est de " << std::setprecision(9) << diff.count() << " secondes" << endl;
			amelioration = g;
		}
		if (g > amelioration + 15000 && pop->individus[pop->ordre[0]]->penalite != 0)
		{
			for (size_t ind = taille_pop - 1; ind > (taille_pop - 0.75 * taille_pop); --ind)
			{
				pop->individus[pop->ordre[ind]]->copier(new chromosome(taille_chromosome));
				pop->individus[pop->ordre[ind]]->evaluer();
			}
			pop->reordonner();
			amelioration = g;
		}
		++g;
	}
	// Reordonner par rapport aux critere2 et critere3
	pop->reordonner_critere2();
	pop->reordonner_critere3();
	// On affiche les statistiques de la population finale
	cout << "Quelques statistiques sur la population finale" << endl;
	pop->statistiques();
	// On affiche la consanginite de la population finale
	pop->similitude();

	// Retourner le meilleur individu rencontre pendant la recherche
	return pop->individus[pop->ordre[0]];
}

void Ae::croisement1X(chromosome &parent1, chromosome &parent2,
					  chromosome &enfant1, chromosome &enfant2)
{

	size_t nb_genes = parent1.taille;
	vector<int> odre_parent1(nb_genes);
	vector<int> odre_parent2(nb_genes);

	for (size_t i = 0; i < nb_genes; ++i)
	{
		odre_parent1[parent1.gene[i]] = i;
		odre_parent2[parent2.gene[i]] = i;
	}

	// 1) l'operateur 1X choisit de maniere aleatoire le point de croisement
	int point = Random::aleatoire(nb_genes);

	// 2) l'operateur 1X recopie le debut du parent 1 au debut de l'enfant 1
	//    et le debut du parent 2 au debut de l'enfant 2.
	enfant1.copier(&parent1);
	enfant2.copier(&parent2);

	// 3) l'operateur 1X complete l'enfant 1 avec les genes manquant en les placant dans l'ordre du parent 2
	//    et l'enfant 2 avec les genes manquant en les placant dans l'ordre du parent 1.
	for (size_t k = point + 1; k < nb_genes; ++k)
	{
		for (size_t l = k + 1; l < nb_genes; ++l)
		{
			if (odre_parent2[enfant1.gene[k]] > odre_parent2[enfant1.gene[l]])
				enfant1.echange_2_genes(k, l);
			if (odre_parent1[enfant2.gene[k]] > odre_parent1[enfant2.gene[l]])
				enfant2.echange_2_genes(k, l);
		}
	}
}

void Ae::croisement2X(chromosome &parent1, chromosome &parent2,
					  chromosome &enfant_s1, chromosome &enfant_s2)
{
	size_t nb_genes = parent1.taille;
	vector<int> odre_parent1(nb_genes);
	vector<int> odre_parent2(nb_genes);

	for (size_t i = 0; i < nb_genes; ++i)
	{
		odre_parent1[parent1.gene[i]] = i;
		odre_parent2[parent2.gene[i]] = i;
	}

	// 1) l'operateur 2X choisit de maniere aleatoire les point de croisement
	size_t point1 = Random::aleatoire(nb_genes);
	size_t point2 = Random::aleatoire(nb_genes);

	// 2) l'operateur 2X recopie le debut du parent 1 au debut de l'enfant 1
	//    et le debut du parent 2 au debut de l'enfant 2.
	enfant_s1.copier(&parent1);
	enfant_s2.copier(&parent2);

	// 3) l'operateur 2X complete l'enfant 1 avec les genes manquant en les placant dans l'ordre du parent 2
	//    et l'enfant 2 avec les genes manquant en les placant dans l'ordre du parent 1.
	for (size_t k = point1 + 1; k < point2; ++k)
	{
		for (size_t l = k + 1; l < point2; ++l)
		{
			if (odre_parent2[enfant_s1.gene[k]] > odre_parent2[enfant_s1.gene[l]])
				enfant_s1.echange_2_genes(k, l);
			if (odre_parent1[enfant_s2.gene[k]] > odre_parent1[enfant_s2.gene[l]])
				enfant_s2.echange_2_genes(k, l);
		}
	}
}

void Ae::construction_correlation(const string nom_dossier)
{
	// Tableau pour convertir en heures
	double parseTemps[2] = {24, 35};
	// alpha
	alphaC = 100 / missions.size();
	// beta
	betaC = 100 / 45;
	// gamma
	gammaC = 100 / 10;

	// zeta
	// On boucle sur la liste des intervenants
	for (size_t i = 0; i < intervenants.getListe()->size(); ++i)
	{
		// On convertit le temps de travail de l'intervenant i en heures
		zetaC += parseTemps[intervenants.getListe()->at(i).getTemps()];
	}
	zetaC = (100) / (zetaC / intervenants.getListe()->size());

	// kappa
	// On boucle sur la liste des missions
	for (size_t i = 1; i < missions.size() + 1; ++i)
	{
		kappaC += distances[0 * (missions.size() + 1) + i] + distances[i * (missions.size() + 1) + 0];
	}
	kappaC = (100) / (kappaC / intervenants.getListe()->size());
}

void Ae::construction_distance(const string nom_dossier)
{
	// Concatenation du nom du dossier et du nom du fichier
	string path = nom_dossier + "/Distances.csv";

	// Allocation de la matrice des distances
	distances = new double[(taille_chromosome + 1) * (taille_chromosome + 1)];

	// Lecture du CSV intervenants
	vector<vector<string>> files = readCSV(path);

	// Construction de la matrice des distances
	for (size_t i = 0; i < taille_chromosome + 1; ++i)
	{
		for (size_t j = 0; j < taille_chromosome + 1; ++j)
		{
			distances[i * (taille_chromosome + 1) + j] = stod(files[i][j].c_str());
		}
	}
}

void Ae::construction_intervenants(const string nom_dossier)
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

	for (size_t i = 0; i < files.size(); ++i)
	{
		// On récupère la compétence de l'intervenant
		string competence = files[i][1];

		// On initialise la compétence de l'intervenant
		Competence competenceIntervenant;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; ++j)
			if (competence == competences[j])
				competenceIntervenant = static_cast<Competence>(j);

		// On récupère la spécialité de l'intervenant
		string specialite = files[i][2];

		// On initialise la spécialité de l'intervenant
		Specialite specialiteIntervenant;

		// On parcourt les spécialités possible et on affecte la spécialité correspondante
		for (size_t j = 0; j < 6; ++j)
			if (specialite == specialites[j])
				specialiteIntervenant = static_cast<Specialite>(j);

		// On récupère le temps de travail de l'intervenant
		string tempsTravail = files[i][3];

		// On initialise la compétence de l'intervenant
		Temps tempsIntervenant;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; ++j)
			if (tempsTravail == temps[j])
				tempsIntervenant = static_cast<Temps>(j);

		// crée un intervenant
		Intervenant intervenant(competenceIntervenant, specialiteIntervenant, tempsIntervenant, i);

		// ajoute l'intervenant à la liste à la fin
		intervenants.getListe()->push_back(intervenant);
	}
}

void Ae::construction_missions(const string nom_dossier)
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

	for (size_t i = 0; i < files.size(); ++i)
	{
		// On récupère le jour de la mission
		string jour = files[i][1];

		// On initialise le jour de la mission
		Jour jourMission = static_cast<Jour>(stoi(jour) - 1);

		// On récupère les horaires de travail de la mission

		string horaires[2];

		for (size_t j = 0; j < 2; ++j)
			horaires[j] = files[i][j + 2];

		// On initialise les horaires de travail de la mission
		int horairesMission[2];

		// On convertit les horaires de travail de la mission en int
		for (size_t j = 0; j < 2; ++j)
			horairesMission[j] = stoi(horaires[j]);

		// On récupère la compétence de la mission
		string competence = files[i][4];

		// On initialise la compétence de la mission
		Competence competenceMission;

		// On parcourt les compétences possible et on affecte la compétence correspondante
		for (size_t j = 0; j < 2; ++j)
			if (competence == competences[j])
				competenceMission = static_cast<Competence>(j);

		// On récupère la spécialité de la mission
		string specialite = files[i][5];

		// On initialise la spécialité de la mission
		Specialite specialiteMission;

		// On parcourt les spécialités possible et on affecte la spécialité correspondante
		for (size_t j = 0; j < 6; ++j)
			if (specialite == specialites[j])
				specialiteMission = static_cast<Specialite>(j);

		// Créer une mission
		Mission mission(competenceMission, specialiteMission, jourMission, horairesMission, i);

		// ajoute la mission à la liste à la fin
		missions.push_back(mission);
	}
}
