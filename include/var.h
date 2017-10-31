/**
 * \file var.h
 * Implemente les methodes pour la gestions des variables
 */
#ifndef VAR_H
#define VAR_H
#include <string.h>
#include "matrix.h"


#define MAX_VAR 	128
#define MAX_NAME	8

enum {type_float, type_matrix};

/**
 * \struct Var
 * Contient une variable definie par l'utilisateur
 * Peut contenir une matrice ou un float
 */
typedef struct s_var
{
    char name[MAX_NAME];
    int type;
    float f;
    Matrix m;
} *Var;


void resetVariables();
int addVar(Matrix m, float f, char* name);
Var getVar(char* name);

#endif
