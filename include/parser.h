/**
 * \file parser.h
 * Implemente les methodes pour analyser les entrées
 */
#ifndef PARSER_H
#define PARSER_H
#include <string.h>
#if __APPLE__
#include <limits.h>
#elif __linux__
#include <linux/limits.h>
#endif
#include "matrix.h"
#include "operations.h"
#include "solve.h"
#include "var.h"
#include "speedtest.h"

#define MAX_COMMANDE 256
#define MAX_TAB		10

#define COMMANDE_MATRIX		"matrix"
#define COMMANDE_ADDITION	"addition"
#define COMMANDE_SUB		"sub"
#define COMMANDE_MULT		"mult"
#define COMMANDE_MULTSCAL	"mult_scal"
#define COMMANDE_EXPO		"expo"
#define COMMANDE_TRANSPOSE	"transpose"
#define COMMANDE_DETERMINANT	"determinant"
#define COMMANDE_INVERT		"invert"
#define COMMANDE_SOLVE		"solve"
#define COMMANDE_RANK		"rank"
#define COMMANDE_DECOMPOSE	"decomposition"
#define COMMANDE_MOINDRECARRE	"least_estimate"

#define COMMANDE_QUIT		"quit"
#define COMMANDE_SPEEDTEST	"speedtest "
#define SPEEDTEST_SZ		10

#define MATRICE_P		"P"
#define MATRICE_L		"L"
#define MATRICE_U		"U"
#define MATRICE_DROITE		"droite"
#define MATRICE_RESIDUS		"residus"

int parseLine(char* line);
ssize_t separer(char chaine [], char sep, char *tab [], ssize_t max);

#endif
