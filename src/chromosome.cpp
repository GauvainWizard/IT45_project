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
	for (size_t i = 0; i < taille; i++)
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
	fitness = 0;
	double parseTemps[2] = {24, 35};  // tableau pour convertir en heures
	double moyenneD = 0;			  // moyenne des distances
	double moyenneWH = 0;			  // moyenne des heures non travaillées
	double moyenneOH = 0;			  // moyenne des heures supplémentaires
	double sOH = 0;					  // sigma OH (écart type des heures supplémentaires)
	double sWH = 0;					  // sigma WH (écart type des heures non travaillées)
	double sD = 0;					  // sigma D (écart type des distances)
	vector<double> heuresTravaillees; // heures de travail des employées
	heuresTravaillees.reserve(intervenants.getListe()->size());

	vector<double> heuresNonTravaillees; // heures de travail des employées
	heuresNonTravaillees.reserve(intervenants.getListe()->size());

	vector<double> heuresSup; // heures de travail des employées
	heuresSup.reserve(intervenants.getListe()->size());

	vector<double> distancesTravail; // distance des employées
	distancesTravail.reserve(intervenants.getListe()->size());

	// On crée la liste de missions
	vector<Mission> missionsJour;
	// On reserve la mémoire pour la liste
	missionsJour.reserve(missions.size());

	// On va calculer femployee
	// On boucle sur la liste des intervenants
	for (size_t i = 0; i < intervenants.getListe()->size(); i++)
	{
		heuresTravaillees[i] = distancesTravail[i] = heuresNonTravaillees[i] = heuresSup[i] = 0;
		// On boucle sur les jours
		for (size_t j = 0; j < 7; j++)
		{
			// on récupère la liste des missions de l'intervenant le jour de la mission
			missionsJour = intervenants.getListe()->at(i).getMissionsJour(static_cast<Jour>(j), gene);
			//  S'il y a au moins une mission
			if (missionsJour.size() > 0)
			{
				// distance entre SESSAD et la première mission
				distancesTravail[i] += distances[(0) * (missions.size() + 1) + missionsJour[0].getId() + 1];
				// distance entre la dernière mission et le SESSAD
				distancesTravail[i] += distances[(missionsJour[missionsJour.size() - 1].getId() + 1) * (missions.size() + 1) + 0];
				// on calcule le temps de trajet en minute entre la SESSAD et la première mission du jour sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
				heuresTravaillees[i] += distances[(0) * (missions.size() + 1) + missionsJour[0].getId() + 1] / (50 * 1000 / 60);
				// on calcule le temps de trajet en minute entre la dernière mission et la SESSAD sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
				heuresTravaillees[i] += distances[(missionsJour[missionsJour.size() - 1].getId() + 1) * (missions.size() + 1) + 0] / (50 * 1000 / 60);
			}
			// On boucle sur les missions du jour
			for (size_t k = 0; k < missionsJour.size(); k++)
			{
				if (k + 1 < missionsJour.size())
				{
					distancesTravail[i] += distances[(missionsJour[k].getId() + 1) * (missions.size() + 1) + missionsJour[k + 1].getId() + 1];
					// on calcule le temps de trajet en minute entre la mission actuelle et la prochaine mission du jour sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
					double tempsTrajet = distances[(missionsJour[k].getId() + 1) * (missions.size() + 1) + missionsJour[k + 1].getId() + 1] / (50 * 1000 / 60);
					heuresTravaillees[i] += tempsTrajet;

					// si les horaires de la mission sont superposés avec les horaires de la mission suivante que l'on regarde
					if ((missionsJour[k].getHoraires()[0] <= missionsJour[k + 1].getHoraires()[0] && missionsJour[k].getHoraires()[1] + tempsTrajet > missionsJour[k + 1].getHoraires()[0]) || (missionsJour[k].getHoraires()[0] < missionsJour[k + 1].getHoraires()[1] + tempsTrajet && missionsJour[k].getHoraires()[1] >= missionsJour[k + 1].getHoraires()[1]))
					{
						// on retourne faux
						return false;
					}
				}
				// On récupère le temps en minute que dure la mission
				heuresTravaillees[i] += missionsJour[k].getHoraires()[1] - missionsJour[k].getHoraires()[0];
			}
			missionsJour.clear();
		}
		// Convertir en heures les minutes de travail des employées
		heuresTravaillees[i] = heuresTravaillees[i] / 60;
		// si le temps de travail des employées est supérieur au temps de travail maximal autorisé
		if (heuresTravaillees[i] < parseTemps[intervenants.getListe()->at(i).getTemps()])
		{
			// on calcule les heures non travaillees
			heuresNonTravaillees[i] = parseTemps[intervenants.getListe()->at(i).getTemps()] - heuresTravaillees[i];
			// on ajoute les heures non travaillees au swh
			moyenneWH += heuresNonTravaillees[i];
			heuresSup[i] = 0;
		}
		else
		{
			// on calcule les heures sup
			heuresSup[i] = heuresTravaillees[i] - parseTemps[intervenants.getListe()->at(i).getTemps()];
			// on ajoute les heures sup au soh
			moyenneOH += heuresSup[i];
			heuresNonTravaillees[i] = 0;
		}
		moyenneD += distancesTravail[i];
	}
	moyenneOH /= intervenants.getListe()->size();
	moyenneWH /= intervenants.getListe()->size();
	moyenneD /= intervenants.getListe()->size();
	for (size_t i = 0; i < intervenants.getListe()->size(); i++)
	{
		sOH += (heuresSup[i] - moyenneOH) * (heuresSup[i] - moyenneOH);
		sWH += (heuresNonTravaillees[i] - moyenneWH) * (heuresNonTravaillees[i] - moyenneWH);
		sD += (distancesTravail[i] - moyenneD) * (distancesTravail[i] - moyenneD);
	}
	sWH /= intervenants.getListe()->size();
	sOH /= intervenants.getListe()->size();
	sD /= intervenants.getListe()->size();
	fitness = (zetaC * sqrt(sWH) + gammaC * sqrt(sOH) + kappaC * sqrt(sD)) / 3;
	return true;
}
// copie les genes d'un chromosome. la fitness n'est reprise
void chromosome::copier(chromosome *source)
{
	for (size_t i = 0; i < gene.size(); i++)
		gene[i] = source->gene[i];
}

// on �change les 2 g�nes
void chromosome::echange_2_genes(int gene1, int gene2)
{
	int inter = gene[gene1];
	gene[gene1] = gene[gene2];
	gene[gene2] = inter;
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
}

void chromosome::deplacement_1_gene()
{
}

void chromosome::inversion_sequence_genes()
{
}

// affichage des param�tre d'un chromosome
void chromosome::afficher()
{
	cout << gene[0];
	for (size_t i = 1; i < taille; i++)
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
	for (size_t i = 0; i < 7; i++)
	{
		// affiche le jour
		cout << jours[i] << " : " << endl;
		// on recupère la liste des index des intervenants qui travaillent le jour i
		index = intervenants.getIndexJour(static_cast<Jour>(i), gene);
		// on affiche la liste des intervenants qui travaillent le jour i
		for (size_t j = 0; j < index.size(); j++)
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
	for (size_t i = 1; i < gene.size(); i++)
		if (chro->gene[i] != this->gene[i])
			return false;
	return true;
}
