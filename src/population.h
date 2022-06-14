#ifndef POPULATION_H
#define POPULATION_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "chromosome.h"
#include "global.h"

/**
 * @class population
 * @brief Classe représentant une population de chromosomes
 */
class population
{
public:
    /**
     * @brief Liste des individus de la population
     */
    chromosome **individus;

    /**
     * @brief Nombre d'individus de la population
     */
    size_t taille_pop;

    /**
     * @brief Tableau donnant l'ordre des individus dans la population
     * du meilleur au plus mauvais en fonction de la fitness
     */
    int *ordre;

    /**
     * @brief Constructeur
     * @param tp - Taille de la population
     * @param tc - Taille du chromosome
     */
    population(size_t tp, size_t tc);

    /**
     * @brief Destructeur
     */
    ~population();

    /**
     * @brief Affichage des statistiques sur la population
     */
    void statistiques();

    /**
     * @brief Compte le nombre de chromosomes similaires à 'chro'
     * @param chro - pointeur sur un chromosome comparé à la population
     * @return nombre de chromosomes similaires issus de la population
     */
    size_t nb_chromosomes_similaires(chromosome *chro);

    /**
     * @brief Affichage du nombre de chromosomes issus de la population ayant les trois meilleures fitness
     */
    void similitude();

    /**
     * @brief Selection par roulette biaisée d'un individu de la population
     */
    chromosome *selection_roulette();

    /**
     * @brief Rempacement par roulette biaisée d'un individu de la population par un chromosome donné
     * @param individu - chromosome ajouté dans la population
     */
    void remplacement_roulette(chromosome *individu);

    /**
     * @brief Ordonne les individus de la population par ordre croissant de fitness
     */
    void ordonner();

    /**
     * @brief Réordonne le classement des individus de la population par ordre croissant de fitness après un petit changement
     */
    void reordonner();

    /**
     * @brief Réordonne le classement des individus de la population par ordre croissant de fitness avec prise en compte du critère 2
     */
    void reordonner_critere2();

    /**
     * @brief Réordonne le classement des individus de la population par ordre croissant de fitness avec prise en compte du critère 3
     */
    void reordonner_critere3();

    /**
     * @brief Afficher de chaque individu de la population, son rang et sa fitness
     */
    void afficher();
};

#endif
