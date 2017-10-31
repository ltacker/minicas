/**
 * \file minicas.c
 * Projet Algèbre Linéaire Appliqué
 * Implementation de matrices et opérations
 * \author Lucas BERTRAND
 * \author Gaia ABDESSELAM
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/matrix.h"
#include "../include/operations.h"
#include "../include/solve.h"
#include "../include/var.h"
#include "../include/parser.h"


#define PROMPT	">"

/**
 * Fonction principale
 * Affiche l'interpreteur
 */
int main(int argc, char **argv) 
{
    struct stat buf;
    int c = 1;
    FILE *f_in = fdopen(0, "r");
    char *line;
    size_t n=0; // initialisation sans importance


    resetVariables();
    system("clear");
    printf("Minicas de Lucas BERTRAND et Gaia ABDESSELAM\n");

    if (!fstat(0, &buf) && S_ISREG(buf.st_mode)) 
    {
	line=NULL;
	while (c && getline(&line, &n, f_in) != -1) 
	{
	    printf(">%s\n", line);
	    
	    // On supprime le retour de chariot a la fin
	    memcpy(line + (strlen(line)-1), "\0", 1);
	    
	    // Appel de la commande
	    if(parseLine(line) == 2)
		c = 0;
	    
	    free(line); 
	    line=NULL;
        }
    } 
    else 
    {
	printf("%s", PROMPT);
	fflush(stdout);
	
	line=NULL;
	while (c && getline(&line, &n, f_in)!=1) 
	{
	    // On supprime le retour de chariot a la fin
	    memcpy(line + (strlen(line)-1), "\0", 1);
	    
	    // Appel de la commande
	    if(parseLine(line) == 2)
		c = 0;
	    else
	    {
		printf("%s", PROMPT);
		fflush(stdout);
	    }
	    
	    free(line); 
	    line=NULL;
	}
    }
    
    resetVariables();
       
    return 0;   
}

