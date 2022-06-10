/**
 * @author CLAUSS Gauvain
 * @file intervenants.cpp
 * @date 10/06/2022
 * @brief Implémentation des méthodes de la classe intervenants
 */

// Inclusion du fichier d'interface de la classe intervenants
#include "intervenants.h"

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

    vector<int> listeindex;
    listeindex.reserve(liste.size());

    for (size_t i = 0; i < liste.size(); i++)
    {
        if (liste[i].estDisponible(mission, gene))
        {
            listeindex.push_back((int)i);
        }
    }

    return listeindex;
}