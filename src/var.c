/**
 * \file var.c
 * Implemente les methodes pour la gestions des variables
 */
#include "../include/var.h"
 
 
/**
 * Variable globale universelle definissant toute les variables definie
 * par l'utilisateur
 */
Var variables[MAX_VAR];


/**
 * Supprime toute les variables definie depuis l'execution du programme
 */
void resetVariables()
{
    int i;
    Var v;
    
    for(i=0; i<MAX_VAR; i++)
    {
	v = variables[i];
	if(v != NULL)	// Tant qu'il y a des variables on les libere
	{
	    if(v->type == type_matrix)
		deleteMatrix(v->m);
	    free(v);
	    variables[i] = NULL;
	}
	else
	    i = MAX_VAR; // Pour areter la boucle
    }
}

/**
 * Ajoute une variables
 * \param matrices a rajouter si c'en est une
 * \param float a rajouter si c'en est un
 * \param nom de la variables
 * \return 1 succes, 0 erreur
 */
int addVar(Matrix m, float f, char* name)
{
    int i;
    Var v;
    
    if(strlen(name) > MAX_NAME)
    {
	fprintf(stderr, "Nom de variable trop longue\n");
	return 0;
    }
    
    for(i=0; i<MAX_VAR; i++)
    {
	v = variables[i];
	
	// On regarde les variables deja existantes
	if(v != NULL)
	{
	    // Si elle existe deja on l'ecrase
	    if(!strcmp(v->name, name))
	    {
		if(v->type == type_matrix)
		    deleteMatrix(v->m);
		    
		if(m != NULL)
		{
		    v->type = type_matrix;
		    v->m = m;
		}
		else
		{
		    v->type = type_float;
		    v->f = f;
		}
		
		return 1;
	    }
	}
	else
	{
	    // On ajoute une nouvelle variable
	    Var nv = calloc(1, sizeof(struct s_var));
	    
	    strcpy(nv->name, name);
	    
	    if(m != NULL)
	    {
		nv->type = type_matrix;
		nv->m = m;
	    }
	    else
	    {
		nv->type = type_float;
		nv->f = f;
	    }
	    
	    variables[i] = nv;
	    
	    return 1;
	}
    }
    
    fprintf(stderr, "Trop de variable creee (max : %d)\n", MAX_VAR);
    return 0;
}

/**
 * Obtient une variables a partir de son nom
 * \param nom de la variable
 * \return variable ou NULL si non trouvée
 */
Var getVar(char* name)
{
    int i;
    Var v;
    
    for(i=0; i<MAX_VAR; i++)
    {
	v = variables[i];
	
	if(v != NULL)
	{
	    if(!strcmp(v->name, name))
		return v;
	}
	else
	    return NULL;	// On a tout parcouru
    }
    
    return NULL;
}
