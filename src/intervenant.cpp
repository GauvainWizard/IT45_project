/**
 * @author CLAUSS Gauvain
 * @file intervenant.cpp
 * @date 07/06/2022
 * @brief Implémentation des méthodes de la classe intervenant
 */

// Inclusion du fichier d'interface de la classe intervenant
#include "intervenant.h"

Intervenant::Intervenant()
{
}

Intervenant::~Intervenant()
{
}

Intervenant::Intervenant(const Intervenant &orig)
{

    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->temps = orig.temps;
}

Intervenant &Intervenant::operator=(const Intervenant &orig)
{
    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->temps = orig.temps;

    return *this;
}

Intervenant::Intervenant(const Competence competence, const Specialite specialite, const Temps temps)
{
    this->competence = competence;
    this->specialite = specialite;
    this->temps = temps;
}

const Competence Intervenant::getCompetence() const
{
    return competence;
}

const Specialite Intervenant::getSpecialite() const
{
    return specialite;
}

const Temps Intervenant::getTemps() const
{
    return temps;
}

void Intervenant::display() const
{
    cout << "Competence : " << competence << " ";
    cout << "Specialite : " << specialite << " ";
    cout << "Temps : " << temps << endl;
}
