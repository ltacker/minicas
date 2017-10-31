/**
 * \file moindrecarre.h
 * Implemente la fonction de moindre carre et l'affichage de la droite
 */
#ifndef MOINDRECARRE_H
#define MOINDRECARRE_H

#include "matrix.h"
#include "operations.h"
#include "solve.h"
#include "plot.h"

int plotEstimate(char* name, Matrix points, Matrix droite);
int leastEstimate(Matrix points, Matrix* droite, Matrix* residus);

#endif
