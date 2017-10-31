/**
 * \file matrix.c
 * Implemente les matrices
 */
#include "../include/matrix.h"


/**
 * Creer une nouvelle matrice
 * \param nombre de ligne
 * \param nombre de colonne
 * \return matrice
 */
Matrix newMatrix(int n, int m)
{
    Matrix ret;
    
    ret = malloc(sizeof(struct s_matrix));
    
    ret->n = n;
    ret->m = m;
    
    ret->mat = calloc((n*m), sizeof(E));
    
    return ret;
}

/**
 * Obtient une copie de matrice
 * \param matrice a copier
 * \return nouvelle matrice
 */
Matrix copyMatrix(const Matrix m)
{
    int i;
    Matrix ret = newMatrix(m->n, m->m);
    
    for(i=0; i<(m->n*m->m); i++)
	ret->mat[i] = m->mat[i];
	
    return ret;
}


/**
 * Obtient un element de la matrice
 * \param matrice
 * \param ligne de l'element
 * \param colonne de l'element
 * \return element
 */
E getElt(Matrix mt, int n, int m)
{
    return mt->mat[(mt->m * n) + m];
}


/**
 * Fixe un element de la matrice
 * \param matrice
 * \param ligne de l'element
 * \param colonne de l'element
 * \param element
 */
void setElt(Matrix mt, int n, int m, E val)
{
    mt->mat[(mt->m * n) + m] = val;
}


/**
 * Detruit une matrice
 * \param matrice a detruire
 */
void deleteMatrix(Matrix m)
{
    free(m->mat);
    free(m);
}


/**
 * Creer une matrice identite
 * \param nombre de ligne
 * \param nombre de colonne
 * \return matrice identite
 */
Matrix identite(int n, int m)
{
    int i;
    Matrix ret;
    
    if(n != m)
	return NULL;
	    
    ret = newMatrix(n, m);
    
    for(i=0; i < ret->n; i++)
	setElt(ret, i, i, 1);
	    
    return ret;
}


/**
 * Creer une matrice aleatoire
 * \param nombre de ligne
 * \param nombre de colonne
 * \param valeur minimal d'un element
 * \param valeur maximal d'un element
 * \return matrice aleatoire
 */
Matrix aleatoire(int n, int m, int min, int max)
{
    int i, j;
    Matrix ret;
	 
    srand(time(NULL));

    ret = newMatrix(n, m);

    for(i=0; i < ret->n; i++)
    {
	for(j=0; j < ret->m; j++)
	    setElt(ret, i, j, (rand()%(max-min) + min));
    }
	
    return ret;
}


/**
 * Affiche une matrice
 * Si la matrice ne contient que des valeurs entieres
 * la fonction n'affiche pas les flottants
 * \param matrice a afficher
 */
void displayMatrix(Matrix m)
{
    int i, j;
    int f = 0;	// Defini si les variables doivent etre afficher en valeur flottante
    float tmp;
    
    // On verifie si la matrice contient des valeurs flottantes
    for(i=0; i < (m->n); i++)
    {
	for(j=0; j < (m->m); j++)
	{
	    tmp = getElt(m, i, j);
	    if(floor(tmp) != tmp)
		f = 1;
	}
    }
    
    // Si la matrice est flottante en l'affiche avec "%f"
    for(i=0; i < (m->n); i++)
    {
	printf(SPACING);
	printf("[");
	
	for(j=0; j < (m->m); j++)
	{
	    tmp = getElt(m, i, j);
	    if(f)
		printf(" %f ", tmp);
	    else
		printf(" %d ", (int)tmp);
	}
	
	printf("]\n");
    }
    
    printf("\n");
}


/**
 * Affiche un float
 * \param float a afficher
 */
void displayFloat(float f)
{
    printf(SPACING);
    if(floor(f) == f)
	printf("%d\n", (int)f);
    else
	printf("%f\n", f);
}
