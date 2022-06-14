#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>

class Random
{
public:
  // initialise le g�n�rateur de variables al�atoire
  static void randomize()
  {
    srand(time(NULL));
  };

  // retourne une variable al�atoire enti�re comprise entre 0 et (borne-1)
  static long aleatoire(long borne)
  {
    return (rand() % borne);
  };
};

#endif
