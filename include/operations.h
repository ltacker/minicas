/**
 * \file operations.h
 * Implemente les operations sur les matrices
 */
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "solve.h"
#include "matrix.h"

Matrix addition(Matrix m1, Matrix m2);
Matrix soustraction(Matrix m1, Matrix m2);
Matrix multiplication(Matrix m1, Matrix m2);
Matrix multScal(E v, const Matrix M);

Matrix transpose(Matrix m);
Matrix exposant(E exp, Matrix m);
float determinant(Matrix m);
int rang(Matrix m);
Matrix invert(Matrix m);
int decompose(Matrix m, Matrix* p, Matrix* l, Matrix* u);

#endif
