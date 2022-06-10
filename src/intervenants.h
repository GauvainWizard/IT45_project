#ifndef INTERVENANTS_H
#define INTERVENANTS_H

#include "enum.h"
#include "mission.h"
#include <iostream>
#include "intervenant.h"

using namespace std;

/**
 * @class Intervenants
 * @brief Classe représentant une liste d'intervenant
 */
class Intervenants
{
private:
    /**
     * @brief Liste des intervenants
     */
    vector<Intervenant> liste;

public:
    /**
     * @brief Constructeur par défaut
     */
    Intervenants();

    /**
     * @brief Destructeur
     */
    virtual ~Intervenants();

    /**
     * @brief Constructeur par copie
     * @param orig - Intervenant à copier
     */
    Intervenants(const Intervenants &orig);

    /**
     * @brief Opérateur d'affectation
     * @param orig - Intervenant à copier
     * @return L'intervenant
     */
    Intervenants &operator=(const Intervenants &orig);

    /**
     * @brief Recupère la liste des intervenants
     * @return La liste des intervenants
     */
    vector<Intervenant> *getListe();

    /**
     * @brief Recupère la liste des index des intervenants qui correspondent à la mission donnée
     * @param mission - Mission donnée pour laquelle on recherche les intervenants
     * @param gene - Le gene actuel
     * @return La liste des index des intervenants correspondants
     */
    vector<int> getIndex(const Mission mission, const vector<int> gene);
};

#endif