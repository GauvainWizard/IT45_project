#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>

class Random
{
public:
  // Initialise le générateur de variables al�atoire
  static void randomize()
  {
    srand(time(NULL));
  };

  // Retourne une variable aléatoire entière comprise entre 0 et (borne-1)
  static long aleatoire(long borne)
  {
    return (rand() % borne);
  };
};

#endif
