#include "chromosome.h"

using namespace std;

// initialisation des param�tres d'un chromosome
chromosome::chromosome(size_t tc)
{
	// taille du chromosome
	taille = tc;
	// On crée une liste d'index
	vector<int> index;
	// On reserve la mémoire pour la liste
	index.reserve(intervenants.getListe()->size());
	// On réserve la mémoire pour le gene
	gene.reserve(taille);
	// On boucle sur la taille du chromosome
	for (size_t i = 0; i < taille; ++i)
	{
		// on recupère la liste des index des intervenants qui ont la même compétence que la mission et qui sont disponibles
		index = intervenants.getIndex(missions[i], gene);
		// on tire aléatoirement un index dans cette liste et on ajoute l'index dans le chromosome
		gene.push_back(index[Random::aleatoire(index.size())]);
	}
	// on affiche le chromosome
	// cout << "Chromosome : ";
	// afficher();
}

// destruction de l'objet 'chromosome'
chromosome::~chromosome()
{
}

// �valuation d'une solution : fonction qui calcule la fitness d'une solution
bool chromosome::evaluer()
{
	critere1 = 0;
	double parseTempsSemaine[2] = {24, 35};						   // tableau pour convertir en heures le temps partiel et temps plein par semaine
	double parseTempsJour[2] = {6, 8};							   // tableau pour convertir en heures le temps partiel et temps plein par jour
	double moyenneD = 0;										   // moyenne des distances
	double moyenneWH = 0;										   // moyenne des heures non travaillées
	double moyenneOH = 0;										   // moyenne des heures supplémentaires
	double sOH = 0;												   // sigma OH (écart type des heures supplémentaires)
	double sWH = 0;												   // sigma WH (écart type des heures non travaillées)
	double sD = 0;												   // sigma D (écart type des distances)
	double sumWOH = 0;											   // somme des heures non travaillées et des heures supplémentaires
	penalite = 0;												   // somme des pénalités
	double maxD = 0;											   // distance maximum parcourure
	const size_t nbMissions = missions.size();					   // nombre de missions
	const size_t nbIntervenants = intervenants.getListe()->size(); // nombre d'intervenants
	vector<double> heuresTravaillees;							   // heures de travail des employées
	heuresTravaillees.reserve(nbIntervenants);

	vector<double> heuresNonTravaillees; // heures de travail des employées
	heuresNonTravaillees.reserve(nbIntervenants);

	vector<double> heuresSup; // heures de travail des employées
	heuresSup.reserve(nbIntervenants);

	vector<double> distancesTravail; // distance des employées
	distancesTravail.reserve(nbIntervenants);

	// On crée la liste de missions
	vector<Mission> missionsJour;
	// On reserve la mémoire pour la liste
	missionsJour.reserve(nbMissions);

	// On va calculer femployee
	// On boucle sur la liste des intervenants
	for (size_t i = 0; i < nbIntervenants; ++i)
	{
		heuresTravaillees[i] = distancesTravail[i] = heuresNonTravaillees[i] = heuresSup[i] = 0; // on initialise les heures de travail, distance, heures supplémentaires et heures non travaillées
		double heuresTravailleesJour, heuresSupJour;											 // heures de travail du jour
		// double heuresSupJour = heuresSup[i]; // heures de travail du jour
		//  On boucle sur les jours
		for (size_t j = 0; j < 7; ++j)
		{
			heuresTravailleesJour = 0; // heures de travail du jour
			// on récupère la liste des missions de l'intervenant le jour de la mission
			missionsJour = intervenants.getListe()->at(i).getMissionsJour(static_cast<Jour>(j), gene);
			double nbMissionsJour = missionsJour.size(); // nombre de missions du jour
			//  S'il y a au moins une mission
			if (nbMissionsJour > 0)
			{
				// distance entre SESSAD et la première mission
				double tempsTrajetSESSAD1 = distances[(0) * (nbMissions + 1) + missionsJour[0].getId() + 1];
				// distance entre la dernière mission et le SESSAD
				double tempsTrajetSESSADD = distances[(missionsJour[nbMissionsJour - 1].getId() + 1) * (nbMissions + 1) + 0];
				// distance entre SESSAD et la première mission
				distancesTravail[i] += tempsTrajetSESSAD1;
				// distance entre la dernière mission et le SESSAD
				distancesTravail[i] += tempsTrajetSESSADD;
				// on calcule le temps de trajet en minute entre la SESSAD et la première mission du jour sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
				tempsTrajetSESSAD1 = tempsTrajetSESSAD1 / (50 * 1000 / 60);
				heuresTravailleesJour += tempsTrajetSESSAD1;
				// on calcule le temps de trajet en minute entre la dernière mission et la SESSAD sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
				tempsTrajetSESSADD = tempsTrajetSESSADD / (50 * 1000 / 60);
				heuresTravailleesJour += tempsTrajetSESSADD;
				// Si on a travaille sur une amplitude supérieure à 12h
				if ((missionsJour[nbMissionsJour - 1].getHoraires()[1] + tempsTrajetSESSADD) - (missionsJour[0].getHoraires()[0] - tempsTrajetSESSAD1) > 12 * 60)
					penalite += 3;
			}
			// On boucle sur les missions du jour
			for (size_t k = 0; k < nbMissionsJour; ++k)
			{
				if (k + 1 < nbMissionsJour)
				{
					distancesTravail[i] += distances[(missionsJour[k].getId() + 1) * (missions.size() + 1) + missionsJour[k + 1].getId() + 1];
					// on calcule le temps de trajet en minute entre la mission actuelle et la prochaine mission du jour sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
					double tempsTrajet = distances[(missionsJour[k].getId() + 1) * (missions.size() + 1) + missionsJour[k + 1].getId() + 1] / (50 * 1000 / 60);
					heuresTravailleesJour += tempsTrajet;

					// si les horaires de la mission sont superposés avec les horaires de la mission suivante que l'on regarde
					if ((missionsJour[k].getHoraires()[0] <= missionsJour[k + 1].getHoraires()[0] && missionsJour[k].getHoraires()[1] + tempsTrajet > missionsJour[k + 1].getHoraires()[0]) || (missionsJour[k].getHoraires()[0] < missionsJour[k + 1].getHoraires()[1] + tempsTrajet && missionsJour[k].getHoraires()[1] >= missionsJour[k + 1].getHoraires()[1]))
						// on retourne faux
						return false;
					// Vérifier qu'on a bien une pause d'au moins 1h entre 12h et 14h
					double tempsTravail1214 = 0;
					// Si on a une mission qui débute entre 12h et 14h
					if (missionsJour[k].getHoraires()[0] > 12 * 60 && missionsJour[k].getHoraires()[0] < 14 * 60)
						tempsTravail1214 += (14 * 60 - missionsJour[k].getHoraires()[0]);
					// Si on a une mission qui finit entre 12h et 14h
					if (missionsJour[k].getHoraires()[1] + tempsTrajet > 12 * 60 && missionsJour[k].getHoraires()[1] + tempsTrajet < 14 * 60)
						tempsTravail1214 += (missionsJour[k].getHoraires()[1] + tempsTrajet - 12 * 60);
					// Si on a une mission qui débute avant 12h et finit après 14h
					if (missionsJour[k].getHoraires()[0] < 12 * 60 && missionsJour[k].getHoraires()[1] + tempsTrajet > 14 * 60)
						tempsTravail1214 += (2 * 60);
					// Si le tempsTravail1214 est supérieur à 1h
					if (tempsTravail1214 > 60)
						penalite += 2;
				}
				if (missionsJour[k].getSpecialite() != intervenants.getListe()->at(gene[i]).getSpecialite())
					++critere2;
				// On récupère le temps en minute que dure la mission
				heuresTravailleesJour += missionsJour[k].getHoraires()[1] - missionsJour[k].getHoraires()[0];
			}
			// Si on a trop travaillé dans la journée par rapport au temps réglementaire
			// heuresTravailleesJour = heuresTravaillees[i] - heuresTravailleesJour;
			heuresSupJour = heuresTravailleesJour - parseTempsJour[intervenants.getListe()->at(i).getTemps()] * 60;
			if ((heuresTravailleesJour) > parseTempsJour[intervenants.getListe()->at(i).getTemps()] * 60)
				penalite += 3; // on ajoute une pénalite de 6
			// // // Si on a dépassé les 2h d'heures supplémentaires dans la journée
			if ((heuresSupJour) > 2 * 60)
				penalite += 3; // on ajoute une pénalite de 12
			heuresTravaillees[i] += heuresTravailleesJour;
			missionsJour.clear();
		}
		// Convertir en heures les minutes de travail des employées
		heuresTravaillees[i] = heuresTravaillees[i] / 60;
		// si le temps de travail des employées est supérieur au temps de travail maximal autorisé
		if (heuresTravaillees[i] < parseTempsSemaine[intervenants.getListe()->at(i).getTemps()])
		{
			// on calcule les heures non travaillees
			heuresNonTravaillees[i] = parseTempsSemaine[intervenants.getListe()->at(i).getTemps()] - heuresTravaillees[i];
			// on ajoute les heures non travaillees au swh
			moyenneWH += heuresNonTravaillees[i];
			heuresSup[i] = 0;
		}
		else
		{
			// on calcule les heures sup
			heuresSup[i] = heuresTravaillees[i] - parseTempsSemaine[intervenants.getListe()->at(i).getTemps()];
			// on ajoute les heures sup au soh
			moyenneOH += heuresSup[i];
			heuresNonTravaillees[i] = 0;
		}
		// si la distance maximum est dépassée
		if (maxD < distancesTravail[i])
			maxD = distancesTravail[i]; // on met à jour la distance maximum
		// si les heures supp sont supérieurs à 10h
		if (heuresSup[i] > 10)
			penalite += 1; // on ajoute une pénalite de 5
		moyenneD += distancesTravail[i];
	}
	moyenneOH /= nbIntervenants;
	moyenneWH /= nbIntervenants;
	moyenneD /= nbIntervenants;
	for (size_t i = 0; i < nbIntervenants; ++i)
	{
		sumWOH += heuresNonTravaillees[i] + heuresSup[i];
		sOH += (heuresSup[i] - moyenneOH) * (heuresSup[i] - moyenneOH);
		sWH += (heuresNonTravaillees[i] - moyenneWH) * (heuresNonTravaillees[i] - moyenneWH);
		sD += (distancesTravail[i] - moyenneD) * (distancesTravail[i] - moyenneD);
	}
	sWH /= nbIntervenants;
	sOH /= nbIntervenants;
	sD /= nbIntervenants;
	critere1 = (zetaC * sqrt(sWH) + gammaC * sqrt(sOH) + kappaC * sqrt(sD)) / 3;
	critere1 += penalite;
	critere2 *= alphaC;
	critere3 = (betaC * sumWOH + kappaC * moyenneD + kappaC * maxD) / 3;
	return true;
}

void chromosome::calcul_critere3()
{
}

// copie les genes d'un chromosome. la fitness n'est reprise
void chromosome::copier(chromosome *source)
{
	for (size_t i = 0; i < gene.size(); ++i)
		gene[i] = source->gene[i];
}

// on �change les 2 g�nes
void chromosome::echange_2_genes(int gene1, int gene2)
{
	swap(gene[gene1], gene[gene2]);
}

void chromosome::echange_2_genes_consecutifs()
{
	// on s�l�ctionne un g�ne al�atoirement entre premier et l'avant dernier.
	// Rappel : Random::aleatoire(taille-1) retourne un entier al�atoire compris entre 0 et taille-2
	int i = Random::aleatoire(taille - 1);

	// on �change le g�ne s�l�ctionn� al�atoirement avec le g�ne le succ�dant
	echange_2_genes(i, i + 1);
}

void chromosome::echange_2_genes_quelconques()
{
	int i = Random::aleatoire(taille);
	int j = Random::aleatoire(taille);

	// on �change le g�ne s�l�ctionn� al�atoirement avec le g�ne le succ�dant
	echange_2_genes(i, j);
}

void chromosome::deplacement_1_gene()
{
	// insertion d'un gene dans une position aléatoire et décaler les autres
	size_t i = Random::aleatoire(taille - 1);
	size_t j = Random::aleatoire(taille);
	// décaler les autres genes
	for (size_t k = taille - 1; k > i; --k)
		gene[k] = gene[k - 1];
	gene[i] = gene[j];
}

void chromosome::inversion_sequence_genes()
{
	int i = Random::aleatoire(taille - 1);
	int j = Random::aleatoire(taille);
	while (i >= j)
		j = Random::aleatoire(taille);
	// inverser toutes les genes entre i et j
	for (int k = i; k < j; ++k)
		echange_2_genes(k, j - (k - i));
}

// affichage des param�tre d'un chromosome
void chromosome::afficher()
{
	cout << gene[0];
	for (size_t i = 1; i < taille; ++i)
		cout << "-" << gene[i];
	cout << endl;
	// cout << " => fitness = " << fitness << endl;

	string jours[7] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};

	// On crée une liste d'index
	vector<int> index;
	// On reserve la mémoire pour la liste
	index.reserve(intervenants.getListe()->size());
	// On crée une liste de missions
	vector<Mission> missionsRetour;
	// On reserve la mémoire pour la liste
	missionsRetour.reserve(gene.size());
	for (size_t i = 0; i < 7; ++i)
	{
		// affiche le jour
		cout << jours[i] << " : " << endl;
		// on recupère la liste des index des intervenants qui travaillent le jour i
		index = intervenants.getIndexJour(static_cast<Jour>(i), gene);
		// on affiche la liste des intervenants qui travaillent le jour i
		for (size_t j = 0; j < index.size(); ++j)
		{
			// on récupère les horaires de travail de l'intervenant
			missionsRetour = intervenants.getListe()->at(index[j]).getMissionsJour(static_cast<Jour>(i), gene);

			cout << intervenants.getListe()->at(index[j]).getId() << " : ";
			for (size_t k = 0; k < missionsRetour.size(); k++)
				cout << missionsRetour[k].getId() << " : " << missionsRetour[k].getHoraires()[0] << " - " << missionsRetour[k].getHoraires()[1] << " ";
			cout << endl;
		}
		cout << endl;
	}
}

bool chromosome::identique(chromosome *chro)
{
	for (size_t i = 1; i < gene.size(); ++i)
		if (chro->gene[i] != this->gene[i])
			return false;
	return true;
}
