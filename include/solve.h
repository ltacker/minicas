/**
 * \file solve.h
 * Implemente la resolutions de systemes linéaires
 */
#ifndef SOLVE_H
#define SOLVE_H


#include <math.h>
#include "operations.h"
#include "matrix.h"


E normeVector(const Matrix V);
void swapLine(Matrix m, int l1, int l2);
void combineLines(Matrix m, E c1, int l1, E c2, int l2);
Matrix solvegausspartiel(const Matrix A, const Matrix B);

#endif
