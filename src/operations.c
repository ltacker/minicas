/**
 * \file operations.c
 * Implemente les operations sur les matrices
 */
#include "../include/operations.h"


/**
 * Additionne 2 matrices
 * \param matrice 1
 * \param matrice 2
 * \return nouvelle matrice
 */
Matrix addition(Matrix m1, Matrix m2)
{
    int i,j;
    Matrix ret;
    
    // Les matrices doivent etre de meme taille
    if(m1->n != m2->n || m2->m != m2->m)
	return NULL;
    
    ret = newMatrix(m1->n, m2->m);
    
    for(i = 0; i < m1->n ; i++)
    {
	for(j = 0; j < m1->m ; j++)
	    setElt(ret, i, j, getElt(m1, i, j) + getElt(m2, i, j));
    }
    
    return ret;
}


/**
 * Soustrait 2 matrices
 * \param matrice 1
 * \param matrice 2
 * \return nouvelle matrice
 */
Matrix soustraction(Matrix m1, Matrix m2)
{
    int i,j;
    Matrix tmp, ret;
    
    tmp = multScal(-1, m2);
    ret = addition(m1, tmp);
    
    deleteMatrix(tmp);
    
    return ret;
}


/**
 * Multiplie 2 matrices
 * \param matrice 1
 * \param matrice 2
 * \return nouvelle matrice
 */
Matrix multiplication(Matrix m1, Matrix m2)
{
    int i, j, k;
    Matrix ret;
    
    
    
    // m1 doit avoir autant de ligne que m2 a de colonne
    if(m1->m != m2->n)
	    return NULL;
    
    ret = newMatrix(m1->n, m2->m);
    
    for(i = 0; i < m1->n ; i++)
	for(j = 0; j < m2->m ; j++)
	    for(k=0; k< m2->n; k++)
		setElt(ret, i, j, getElt(ret, i, j) + (getElt(m2, k, j)*getElt(m1, i,  k)));
    
    
    return ret;
}


/**
 * Multiplie la matrice par un scalaire
 * \param scalaire
 * \param matrice a multiplier
 */
Matrix multScal(E v, const Matrix m)
{
    int i;
    Matrix ret = newMatrix(m->n, m->m);
    
    for(i=0; i<(m->n*m->m); i++)
	ret->mat[i] = (m->mat[i] * v);
	
    return ret;
}


/**
 * Fait la transpose de la matrice
 * \param matrice
 * \return matrice transposé
 */
Matrix transpose(Matrix m)
{
    int i, j;
    Matrix ret = newMatrix(m->m, m->n);
    E tmp;
    
    for(i=0; i<(ret->n); i++)
    {
	for(j=0; j<(ret->m); j++)
	{
	    tmp = getElt(m, j, i);
	    setElt(ret, i, j, tmp);
	}
    }
    
    return ret;
}


/**
 * Calcul l'exposant d'une matrice en appliquant plusieur fois la multiplication
 * \param exposant >= 0
 * \param matrice
 * \return matrice ^ exp
 */
Matrix exposant(E exp, Matrix m)
{
    Matrix ret, tmp;
    int i;
    
    if(exp < 0)
	return NULL;
    
    ret = identite(m->n, m->m);
    
    // On reitere la multiplication
    for(i=0; i<(int)exp; i++)
    {
	tmp = multiplication(m, ret);
	deleteMatrix(ret);
	ret = tmp;
    }
    
    return ret;
}


/**
 * Obtient la forme echelonne de la matrice pour le calcule du determinant et du rang
 * \param matrice a echelonner
 * \param facteur pour l'inverse
 * \return matrice echelonnee
 */
Matrix echelon(Matrix m, int* c)
{
    Matrix ret = copyMatrix(m);
    int i, j, l;
    E max, tmp;
    int pivot = 0;
    
    for(i=0, l=0; i<(ret->m)-1 && l<(ret->n); i++, l++)
    {
	// Choix du pivot partiel
	for(j=l, max=0; j<(ret->n); j++)
	{
	    tmp = getElt(ret, j, i);
	    tmp = abs(tmp);
	    if(tmp > max)
	    {
		max = tmp;
		pivot = j;
	    }
	}
	
	//  On a trouver un pivot
	//if(max > 0)
	//{
	    if(l != pivot)
	    {
		if(c != NULL)
		    (*c) *= -1;
		swapLine(ret, l, pivot);
	    }
	
	    // Soustraction
	    for(j=l+1; j<(ret->n); j++)
	    {
		tmp = -1 * (getElt(ret, j, i) / getElt(ret, l, i));
		combineLines(ret, tmp, l, 1, j);
	    }
	//}
    }
    
    return ret;
}

/**
 * Calcule le determinant de la matrice
 * \param matrice
 * \return determinant
 */
float determinant(Matrix m)
{
    Matrix tmp;
    int i;
    int c = 1;
    float ret = 1;
    
    if(m->n != m->m)
    {
	fprintf(stderr, "La matrice doit etre carre\n");
	return 0;
    }
    
    // On prend la matrice echelon et on multiplie sa diagonale
    tmp = echelon(m, &c);
    for(i=0; i<m->n; i++)
	ret *= getElt(tmp, i, i);
	
    ret *= c;
	
    deleteMatrix(tmp);
    return ret;
}


/**
 * Calcule le rang de la matrice
 * \param matrice
 * \return rang
 */
int rang(Matrix m)
{
    Matrix tmp;
    int i, j, c;
    int ret = 0;
    
    // On cherche la premiere ligne qui ne contient que des 0
    tmp = echelon(m, NULL);
    for(i=0, c=1; i<tmp->n && c; i++)
    {
	for(j=0, c=0; j<tmp->m; j++)
	{
	    if(getElt(tmp, i, j) != 0)
		c=1;
	}
	if(c==1)
	    ret++;
    }
    
    deleteMatrix(tmp);
    
    return ret;
}


/**
 * Calcule l'inverse de la matrice
 * \param matrice a inverser
 * \return matrice inversée
 */
Matrix invert(Matrix m)
{
    Matrix ret, tmpM;
    int i, j;
    E max, tmp;
    int pivot = 0;
    
    if(determinant(m) == 0)
    {
	fprintf(stderr, "invert: la matrice doit etre inversible\n");
	return NULL;
    }
	
    ret = identite(m->n, m->m);
    tmpM = copyMatrix(m);
    
    // On met la matrice sous forme triangulaire inferieur
    for(i=0; i<(ret->m); i++)
    {
	// Choix du pivot partiel
	for(j=i, max=0; j<(ret->n); j++)
	{
	    tmp = getElt(tmpM, j, i);
	    tmp = abs(tmp);
	    if(tmp > max)
	    {
		max = tmp;
		pivot = j;
	    }
	}
	swapLine(tmpM, i, pivot);
	swapLine(ret, i, pivot);
	
	// On veut 1 dans la diagonal
	tmp = (1/getElt(tmpM, i, i));
	combineLines(ret, 0, 0, tmp, i);
	combineLines(tmpM, 0, 0, tmp, i);
	
	// Soustraction
	for(j=i+1; j<(ret->n); j++)
	{
	    tmp = -1 * getElt(tmpM, j, i);
	    combineLines(ret, tmp, i, 1, j);
	    combineLines(tmpM, tmp, i, 1, j);
	}
    }
    
    // On met la matrice sous forme triangulaire superieur
    for(i=(ret->m)-1; i>0; i--)
    {
	// Soustraction
	for(j=i-1; j>=0; j--)
	{
	    tmp = -1 * getElt(tmpM, j, i);
	    combineLines(ret, tmp, i, 1, j);
	    combineLines(tmpM, tmp, i, 1, j);
	}
    }
    
    deleteMatrix(tmpM);
    return ret;
}


/**
 * Decompose une matrice en PLU
 * \param matrice a decomposer
 * \param matrice de permutation
 * \param triangle inferieur
 * \param triangle superieur
 * \return 1 succes, 0 echec
 */
int decompose(Matrix m, Matrix* p, Matrix* l, Matrix* u)
{
    int i, j, k;
    Matrix p1, l1, u1, tmpM;
    E max, tmp;
    int pivot = 0;
    int* perm;
    
    if(m->n != m->m)
    {
	fprintf(stderr, "decompose: la matrice doit etre carre\n");
	return 0;
    }
    if(determinant(m) == 0)
    {
	fprintf(stderr, "decompose: la matrice doit etre inversible\n");
	return 0;
    }
    
    // On calcule la matrice de permutation
    perm = malloc((m->n)*sizeof(int*));
    for(i=0; i<(m->n); i++)
	perm[i] = i;
    u1 = copyMatrix(m);
    // On realise une triangularisation intermediaire pour connaitre les permutations finales
    for(i=0; i<m->n; i++)
    {
	// Choix du pivot partiel
	for(j=i, max=0; j<(m->n); j++)
	{
	    tmp = getElt(u1, j, i);
	    tmp = abs(tmp);
	    if(tmp > max)
	    {
		max = tmp;
		pivot = j;
	    }
	}
	swapLine(u1, i, pivot);
	
	// On sauvegarde la permutation effectuée
	tmp = perm[pivot];
	perm[pivot] = perm[i];
	perm[i] = tmp;
	
	// Soustraction
	for(j=i+1; j<(m->n); j++)
	{
	    tmp = -1 * (getElt(u1, j, i) / getElt(u1, i, i));
	    combineLines(u1, tmp, i, 1, j);
	}
    }
    // On creer la matrice de permutation en fonction des lignes permutées
    p1 = newMatrix(m->n, m->m);
    for(i=0; i<m->n; i++)
    	setElt(p1, i, perm[i], 1);
    free(perm);
    deleteMatrix(u1);
    

    // On realise la decompostion
    l1 = identite(m->n, m->m);
    u1 = multiplication(p1, m);
    
    for(i=0; i<m->n; i++)
    {
	// Soustraction
	for(j=i+1; j<(m->n); j++)
	{
	    tmp = -1 * (getElt(u1, j, i) / getElt(u1, i, i));
	    combineLines(u1, tmp, i, 1, j);
	    
	    // On place l'element dans L
	    setElt(l1, j, i, -1*tmp);
	}
    }
    
    (*u) = u1;
    (*l) = l1;
    (*p) = transpose(p1);	// p1*m = l*u donc m = p1(-1)*l*u
    
    deleteMatrix(p1);
    
    return 1;
}
