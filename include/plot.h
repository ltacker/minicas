/**
 * \file plot.h
 * defini les fonctions pour le plotting
 */
#ifndef PLOT_H
#define PLOT_H

#define TMP		""
#define DAT		".dat"

#include <string.h>
#if __APPLE__
#include <limits.h>
#elif __linux__
#include <linux/limits.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "matrix.h"

int plotInit(char* name);
void plotAdd(char* name, int t, double nb);
int plotSaveSpeedtest(char* name);
int plotSaveMoindrecarre(char* name, Matrix droite);

#endif
