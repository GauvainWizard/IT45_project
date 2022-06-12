#ifndef INTERVENANT_H
#define INTERVENANT_H

#include "enum.h"
#include "mission.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * @class Intervenant
 * @brief Classe représentant un intervenant
 */
class Intervenant
{
private:
    /**
     * @brief Competence de l'intervenant
     */
    Competence competence;

    /**
     * @brief Specialite de l'intervenant
     */
    Specialite specialite;

    /**
     * @brief Temps de travail (partiel ou plein) de l'intervenant
     */
    Temps temps;

    /**
     * @brief Id de l'intervenant
     */
    int id;

public:
    /**
     * @brief Constructeur par défaut
     */
    Intervenant();

    /**
     * @brief Destructeur
     */
    virtual ~Intervenant();

    /**
     * @brief Constructeur par copie
     * @param orig - Intervenant à copier
     */
    Intervenant(const Intervenant &orig);

    /**
     * @brief Opérateur d'affectation
     * @param orig - Intervenant à copier
     * @return L'intervenant
     */
    Intervenant &operator=(const Intervenant &orig);

    /**
     * @brief Construit un intervenant à partir de ses caractéristiques (competence, specialite, temps)
     * @param competence - Competence de l'intervenant
     * @param specialite - Specialite de l'intervenant
     * @param temps - Temps de l'intervenant
     * @param id - Id de l'intervenant
     */
    Intervenant(const Competence competence, const Specialite specialite, const Temps temps, const int id);

    /**
     * @brief Affiche l'intervenant
     * @return None
     */
    void display() const;

    /**
     * @brief Recupère la competence de l'intervenant
     * @return Competence de l'intervenant
     */
    const Competence getCompetence() const;

    /**
     * @brief Recupère la specialite de l'intervenant
     * @return Specialite de l'intervenant
     */
    const Specialite getSpecialite() const;

    /**
     * @brief Recupère le temps de l'intervenant
     * @return Temps de l'intervenant
     */
    const Temps getTemps() const;

    /**
     * @brief Recupère l'id de l'intervenant
     * @return Id de l'intervenant
     */
    const int getId() const;

    /**
     * @brief Vérifie si l'intervenant est disponible pour la mission donnée
     * @param mission - Mission donnée pour laquelle on vérifie si l'intervenant est disponible
     * @param gene - Le gene qui correspond
     * @return True si l'intervenant est disponible, false sinon
     */
    bool estDisponible(const Mission mission, const vector<int> gene) const;

    /**
     * @brief Vérifie si l'intervenant travaille le jour donné
     * @param jour - Jour donné pour lequel on vérifie si l'intervenant travaille
     * @param gene - Le gene qui correspond
     * @return True si l'intervenant travaille le jour donné, false sinon
     */
    bool travailleLeJour(const Jour jour, const vector<int> gene) const;

    /**
     * @brief Récupère la liste des missions de l'intervenant pour le jour donné
     * @param jour - Jour pour lequel on récupère les missions
     * @param gene - Le gene qui correspond
     * @return Liste des missions de l'intervenant pour le jour donné
     */
    vector<Mission> getMissionsJour(const Jour jour, const vector<int> gene) const;

    /**
     * @brief Récupère la liste des missions de l'intervenant
     * @param gene - Le gene qui correspond
     * @return Liste des missions de l'intervenant
     */
    vector<Mission> getMissions(const vector<int> gene) const;
};

#endif