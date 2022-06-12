/**
 * @author CLAUSS Gauvain
 * @file intervenant.cpp
 * @date 07/06/2022
 * @brief Implémentation des méthodes de la classe intervenant
 */

// Inclusion du fichier d'interface de la classe intervenant
#include "intervenant.h"
#include "global.h"

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
    this->id = orig.id;
}

Intervenant &Intervenant::operator=(const Intervenant &orig)
{
    this->competence = orig.competence;
    this->specialite = orig.specialite;
    this->temps = orig.temps;
    this->id = orig.id;
    return *this;
}

Intervenant::Intervenant(const Competence competence, const Specialite specialite, const Temps temps, const int id)
{
    this->competence = competence;
    this->specialite = specialite;
    this->temps = temps;
    this->id = id;
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

const int Intervenant::getId() const
{
    return id;
}

void Intervenant::display() const
{
    cout << "Competence : " << competence << " ";
    cout << "Specialite : " << specialite << " ";
    cout << "Temps : " << temps << endl;
    cout << "Id : " << id << endl;
}

bool Intervenant::estDisponible(const Mission mission, const vector<int> gene) const
{
    // on vérifie si l'intervenant a la bonne compétence
    if (competence == mission.getCompetence())
    {
        // on fait la liste des missions de l'intervenant le jour de la mission
        for (size_t i = 0; i < gene.size(); i++)
        {
            // on vérifie si la mission que l'on regarde est du même jour que la mission donnée et que c'est l'intervenant concerné
            if (missions[i].getJour() == mission.getJour() && gene[i] == id)
            {
                // on calcule le temps de trajet en minute entre les deux missions sachant que la distance est en mètre et qu'on va à vitesse moyenne de 50 km/h
                double tempsTrajet = distances[(i + 1) * (missions.size() + 1) + mission.getId() + 1] / (50 * 1000 / 60);
                // si les horaires de la mission sont superposés avec les horaires de la mission que l'on regarde
                if ((missions[i].getHoraires()[0] <= mission.getHoraires()[0] && missions[i].getHoraires()[1] + tempsTrajet > mission.getHoraires()[0]) || (missions[i].getHoraires()[0] < mission.getHoraires()[1] + tempsTrajet && missions[i].getHoraires()[1] >= mission.getHoraires()[1]))
                {
                    // on retourne faux
                    return false;
                }
            }
        }

        // Si on arrive ici, c'est que l'intervenant est disponible
        return true;
    }

    // Si on arrive ici, c'est que l'intervenant n'a pas la bonne compétence
    return false;
}

bool Intervenant::travailleLeJour(const Jour jour, const vector<int> gene) const
{
    // on vérifie si l'intervenant travaille le jour donné
    return getMissionsJour(jour, gene).size() > 0;
}

vector<Mission> Intervenant::getMissionsJour(const Jour jour, const vector<int> gene) const
{
    // On crée la liste de missions
    vector<Mission> missionsRetour;
    // On reserve la mémoire pour la liste
    missionsRetour.reserve(gene.size());
    // on fait la liste des missions de l'intervenant le jour de la mission
    for (size_t i = 0; i < gene.size(); i++)
    {
        // on vérifie si la mission que l'on regarde est du même jour que la mission donnée et que c'est l'intervenant concerné
        if (missions[i].getJour() == jour && gene[i] == id)
        {
            // on ajoute la mission à la liste
            missionsRetour.push_back(missions[i]);
        }
    }

    // On retourne la liste
    return missionsRetour;
}

vector<Mission> Intervenant::getMissions(const vector<int> gene) const
{
    // On crée la liste de missions
    vector<Mission> missionsRetour;
    // On reserve la mémoire pour la liste
    missionsRetour.reserve(missions.size());
    for (size_t i = 0; i < 7; i++)
    {
        // On crée la liste de missions
        vector<Mission> missionsTemp;
        // On reserve la mémoire pour la liste
        missionsTemp.reserve(missions.size());
        // on fait la liste des missions de l'intervenant le jour de la mission
        missionsTemp = getMissionsJour(static_cast<Jour>(i), gene);
        // on ajoute les missions à la liste
        missionsRetour.insert(missionsRetour.end(), missionsTemp.begin(), missionsTemp.end());
    }

    // On retourne la liste
    return missionsRetour;
}