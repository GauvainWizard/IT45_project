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
    this->id = orig.id;
}

Mission::Mission(const Competence c, const Specialite s, const Jour j, const int *horaires, const int id)
{
    this->competence = c;
    this->specialite = s;
    this->jour = j;
    this->horaires[0] = horaires[0];
    this->horaires[1] = horaires[1];
    this->id = id;
}

Mission::~Mission()
{
}

Mission &Mission::operator=(const Mission &orig)
{
    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->jour = orig.jour;
    this->horaires[0] = orig.horaires[0];
    this->horaires[1] = orig.horaires[1];
    this->id = orig.id;
    return *this;
}