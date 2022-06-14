#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "random.h"
#include "global.h"

/**
 * @class chromosome
 * @brief Classe représentant la structure d'une solution du problème
 */
class chromosome
{
public:
    /**
     * @brief Liste de gênes formant le chromosome
     */
    vector<int> gene;

    /**
     * @brief Taille du chromosome (nombre de gênes)
     */
    size_t taille;

    /**
     * @brief Pénalité du chromosome (somme des pénalités des contraintes)
     */
    double penalite;

    /**
     * @brief Valeur de la fonction objectif (fitness) de la solution
     */
    double critere1;

    /**
     * @brief Valeur du critère 2 des meilleures solutions
     */
    double critere2;

    /**
     * @brief Valeur du critère 3 des meilleures solutions
     */
    double critere3;

    /**
     * @brief Constructeur aléatoire
     * @param tc - Taille du chromosome
     */
    chromosome(size_t tc);

    /**
     * @brief Destructeur
     */
    ~chromosome();

    /**
     * @brief Affichage de la solution
     */
    void afficher();

    /**
     * @brief Evaluation du chromosome, calcul des critères 1, 2 et 3
     * @return Booléen retournant faux si une contrainte dure n'est pas validée, vrai sinon.
     */
    bool evaluer();

    /**
     * @brief Copier le chromosome source vers le chromosome this
     * @param source - pointeur sur un chromosome à copier
     */
    void copier(chromosome *source);

    /**
     * @brief Test si 2 chromosome sont identiques
     * @param chro - pointeur sur un chromosome à comparer aevc this
     * @return Booléen retournant vrai si les deux chromosomes sont identiques, faux sinon.
     */
    bool identique(chromosome *chro);

    /**
     * @brief Echange entre deux gênes passés en paramètre
     * @param gene1 - entier correspondant à l'index du gêne à échanger
     * @param gene2 - entier correspondant à l'index de l'autre gêne à échanger
     */
    void echange_2_genes(int gene1, int gene2);

    /**
     * @brief Echange entre deux gênes consécutifs choisis aléatoirement
     */
    void echange_2_genes_consecutifs();

    /**
     * @brief Echange entre deux gênes quelconques choisis alétoirement
     */
    void echange_2_genes_quelconques();

    /**
     * @brief Déplace un gêne dans le chromosome
     */
    void deplacement_1_gene();

    /**
     * @brief Inverse une séquence de gênes dans le chromosome
     */
    void inversion_sequence_genes();
};

#endif
