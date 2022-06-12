/**
 * @author CLAUSS Gauvain
 * @file intervenants.cpp
 * @date 10/06/2022
 * @brief Implémentation des méthodes de la classe intervenants
 */

// Inclusion du fichier d'interface de la classe intervenants
#include "intervenants.h"
#include "global.h"

Intervenants::Intervenants()
{
}

Intervenants::~Intervenants()
{
}

Intervenants::Intervenants(const Intervenants &orig)
{
    this->liste = orig.liste;
}

Intervenants &Intervenants::operator=(const Intervenants &orig)
{
    this->liste = orig.liste;

    return *this;
}

vector<Intervenant> *Intervenants::getListe()
{
    return &liste;
}

vector<int> Intervenants::getIndex(const Mission mission, const vector<int> gene)
{
    // On crée une liste d'index
    vector<int> listeindex;
    // On reserve la mémoire pour la liste
    listeindex.reserve(liste.size());

    // On boucle sur tous les intervenants
    for (size_t i = 0; i < liste.size(); ++i)
        // On vérifie si l'intervenant i est disponible pour la mission donnée et le gene
        if (liste[i].estDisponible(mission, gene))
            // Si c'est le cas, on l'ajoute à la liste
            listeindex.push_back((int)i);

    // On retourne la liste
    return listeindex;
}

vector<int> Intervenants::getIndexJour(const Jour jour, const vector<int> gene)
{
    // On crée une liste d'index
    vector<int> listeindex;
    // On reserve la mémoire pour la liste
    listeindex.reserve(liste.size());

    // On boucle sur tous les intervenants
    for (size_t i = 0; i < liste.size(); ++i)
    {
        // On vérifie si l'intervenant i travaille le jour donné
        if (liste[i].travailleLeJour(jour, gene))
        {
            // Si c'est le cas, on l'ajoute à la liste
            listeindex.push_back((int)i);
        }
    }

    // On retourne la liste
    return listeindex;
}