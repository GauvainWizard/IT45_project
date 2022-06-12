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
    double fitness;   // la valeur de la fonction objectif (fitness) de la solution

    // CONSTRUCTEURS
    chromosome(size_t tc); // constructeur de l'objet al�atoirement
    ~chromosome();         // destructeur de l'objet

    // METHODES
    void afficher(); // fonction d'affichage du chromosome (i.e. de la solution)
    bool evaluer();
    // fonction d'�valuation du chromosome (c-�-d calcul la fitness)
    //   Elle doit etre lanc�e � la creation des solution et apres
    //   l'ex�cution des operateurs de mutation et de croisement
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
