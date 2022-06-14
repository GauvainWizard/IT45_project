#ifndef MISSION_HPP_
#define MISSION_HPP_

#include <stdio.h>
#include "enum.h"

/**
 * @class Mission
 * @brief Définit les caractéristiques d'une mission
 */
class Mission
{
private:
    /**
     * @brief Compétence de la mission
     */
    Competence competence;

    /**
     * @brief Spécialité de la mission
     */
    Specialite specialite;

    /**
     * @brief Jour de la mission
     */
    Jour jour;

    /**
     * @brief Id de la mission
     */
    int id;

    /**
     * @brief Horaires de la mission
     * Tableau de 2 entiers des horaires du cours : 1er elem = minute de la journée du début de la mission, 2e elem = minute de fin de la mission
     */
    int horaires[2];

public:
    /**
     *  @brief Constructeur par défaut
     */
    Mission();

    /**
     *  @brief Destructeur
     */
    virtual ~Mission();

    /**
     *  @brief Constructeur par recopie
     *  @param orig - Mission à copier
     */
    Mission(const Mission &orig);

    /**
     *  @brief Construit une mission à partir de ses caractéristiques (competence, specialite, jour, horaires)
     *  @param c - Compétence de la mission
     *  @param s - Spécialité de la mission
     *  @param j - Jour de la mission
     *  @param horaires - Minutes de début et de fin de la mission
     */
    Mission(const Competence c, const Specialite s, const Jour j, const int *horaires, const int id);

    /**
     *  @brief Opérateur d'affectation
     *  @param orig - référence constante sur un objet de classe Mission
     *  @return référence sur un objet de classe Mission
     */
    Mission &operator=(const Mission &orig);

    /**
     *  @brief Retourne la compétence de la mission
     *  @return Competence de la mission
     */
    inline const Competence getCompetence() const
    {
        return competence;
    }

    /**
     *  @brief Retourne la spécialité de la mission
     *  @return Specialite de la mission
     */
    inline const Specialite getSpecialite() const
    {
        return specialite;
    }

    /**
     *  @brief Retourne le jour de la mission
     *  @return Jour de la mission
     */
    inline const Jour getJour() const
    {
        return jour;
    }

    /**
     * @brief Retourne l'id de la mission
     * @return Id de la mission
     */
    inline const int *getHoraires() const
    {
        return horaires;
    }

    /**
     *  @brief Retourne les horaires de la mission
     *  @return Horaires de la mission
     */
    inline const int getId() const
    {
        return id;
    }
};

#endif /* MISSION_HPP_ */
