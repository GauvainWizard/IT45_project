/**
 * @author BERNARD Samuel
 * @file mission.cpp
 * @date 01/06/2022
 * @brief Implémentation des méthodes de la classe mission
 */

// Inclusion du fichier d'interface de la classe mission
#include "mission.h"

Mission::Mission()
{
}

Mission::Mission(const Mission &orig)
{
    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->jour = orig.jour;
    this->horaires[0] = orig.horaires[0];
    this->horaires[1] = orig.horaires[1];
}

Mission::Mission(const Competence c, const Specialite s, Jour j, const int *horaires)
{
    this->competence = c;
    this->specialite = s;
    this->jour = j;
    this->horaires[0] = horaires[0];
    this->horaires[1] = horaires[1];
}

Mission::~Mission()
{
    /* Rien à mettre ici car on ne fait pas d'allocation dynamique
       dans la classe Pression. */
}

Mission &Mission::operator=(const Mission &orig)
{
    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->jour = orig.jour;
    this->horaires[0] = orig.horaires[0];
    this->horaires[1] = orig.horaires[1];
    return *this;
}

const Competence Mission::getCompetence() const
{
    return competence;
}

const Specialite Mission::getSpecialite() const
{
    return specialite;
}

const Jour Mission::getJour() const
{
    return jour;
}

const int *Mission::getHoraires() const
{
    return horaires;
}