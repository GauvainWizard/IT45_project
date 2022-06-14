#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "random.h"
#include "global.h"

// La classe chromosome represente la structure d'une solution du probl�me
class chromosome
{
public:
    // ATTRIBUTS
    vector<int> gene; // les g�nes du chromosome/solution
    size_t taille;    // la taille du chromosome = nombre de g�nes
    double penalite;  // la penalite du chromosome
    double critere1;  // la valeur de la fonction objectif (fitness) de la solution
    double critere2;  // la valeur du critère 2 des meilleures solutions
    double critere3;  // la valeur du critère 3 des meilleures solutions

    // CONSTRUCTEURS
    chromosome(size_t tc); // constructeur de l'objet al�atoirement
    ~chromosome();         // destructeur de l'objet

    // METHODES
    void afficher(); // fonction d'affichage du chromosome (i.e. de la solution)
    bool evaluer();
    // fonction d'�valuation du chromosome (c-�-d calcul la fitness)
    //   Elle doit etre lanc�e � la creation des solution et apres
    //   l'ex�cution des operateurs de mutation et de croisement

    // Calcul du nombre d’affectations dont la specialite est insatisfaite
    void calcul_critere2();

    // Calcul de distance parcourue et la somme du nombres des heures non-travaillees et supplementaires
    void calcul_critere3();

    void copier(chromosome *source);  // copie le chromosome 'source'
    bool identique(chromosome *chro); // test si 2 chromosome sont identique

    // OPERATEURS DE MUTATION
    void echange_2_genes(int gene1, // interchange 2 g�nes du chromosome
                         int gene2);
    void echange_2_genes_consecutifs(); // interchange 2 g�nes cons�cutifs du chromosome
    void echange_2_genes_quelconques(); // interchange al�atoirement 2 g�nes du chromosome
    void deplacement_1_gene();          // d�place un g�ne dans le chromosome
    void inversion_sequence_genes();    // inverse une s�quence de g�nes du chromosome
};

#endif
