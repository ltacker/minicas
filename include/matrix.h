/**
 * \file matrix.h
 * Implemente les matrices
 */
#ifndef MATRIX_H
#define MATRIX_H


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


#define SPACING "        "


typedef float E;

typedef struct s_matrix 
{
    int n;
    int m;
    E* mat;
} *Matrix;


Matrix newMatrix(int n, int m);
Matrix copyMatrix(const Matrix m);
E getElt(Matrix mt, int n, int m);
void setElt(Matrix mt, int n, int m, E val);
void deleteMatrix(Matrix m); 
 
void displayMatrix(Matrix m);
void displayFloat(float f);

Matrix identite(int n, int m);
Matrix aleatoire(int n, int m, int min, int max);

#endif
