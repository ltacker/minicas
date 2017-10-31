/**
 * \file speedtest.h
 * Implemente les methodes pour le test de rapidite
 */
#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <unistd.h>
#if __APPLE__
#include <limits.h>
#elif __linux__
#include <linux/limits.h>
#endif
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <signal.h>
#include "parser.h"
#include "var.h"
#include "plot.h"

int speedtest(char* line);

#endif
