/**
 * \file solve.c
 * Implemente la resolutions de systemes linéaires
 */
#include "../include/solve.h"


/**
 * Obtient la norme du vecteur
 * \param Matrice etant un vecteur (ligne ou colonne egale a 1)
 * \return Norme ou -1 en cas d'erreur
 */
E normeVector(const Matrix m)
{
    E ret, tmp;
    int i;
    
    if(m->n > 1 && m->m == 1)
    {
	for(i=0, ret=0; i<m->n; i++)
	{
	    tmp = getElt(m, i, 0);
	    ret += tmp*tmp;
	}
    }
    if(m->n == 1 && m->m > 1)
    {
	for(i=0, ret=0; i<m->m; i++)
	{
	    tmp = getElt(m, 0, i);
	    ret += tmp*tmp;
	}
    }
    else
	return -1;
    
    return sqrt(ret);
}

/**
 * Echange les lignes d'une matrice
 * \param matrice modifiée par effet de bord
 * \param ligne 1
 * \param ligne 2
 */
void swapLine(Matrix m, int l1, int l2)
{
    int i;
    E tmp;
    
    for(i=0; i<m->m; i++)
    {
	tmp = getElt(m, l1, i);
	setElt(m, l1, i, getElt(m, l2, i));
	setElt(m, l2, i, tmp);
    }
}

/**
 * Combine la ligne l2 par la ligne l1
 * \param matrice modifiée par effet de bord
 * \param scalaire de la ligne 1
 * \param ligne 1
 * \param scalaire de la ligne 2
 * \param ligne 2
 */
void combineLines(Matrix m, E c1, int l1, E c2, int l2)
{
    int i;
    
    for(i=0; i<m->m; i++)
	setElt(m, l2, i, c2*getElt(m, l2, i) + c1*getElt(m, l1, i));
}

/**
 * Realise la triangularisation de matrice
 * \param Matrice a modifiée par effet de bord
 * \param Matrice b modifiée par effet de bord
 */
void triangSup(Matrix a, Matrix b)
{
    int i, j;
    E max, tmp;
    int pivot = 0;
    
    for(i=0; i<(a->n)-1; i++)
    {
	// Choix du pivot partiel
	for(j=i, max=0; j<(a->n); j++)
	{
	    tmp = getElt(a, j, i);
	    tmp = abs(tmp);
	    if(tmp > max)
	    {
		max = tmp;
		pivot = j;
	    }
	}
	swapLine(a, i, pivot);
	swapLine(b, i, pivot);
	
	// Soustraction
	for(j=i+1; j<(a->n); j++)
	{
	    tmp = -1 * (getElt(a, j, i) / getElt(a, i, i));
	    combineLines(a, tmp, i, 1, j);
	    combineLines(b, tmp, i, 1, j);
	}
    }
}

/**
 * Effectue la remontee pour la methode de Gauss
 * La matrice a doit etre en triangle superieur
 * \param matrice a
 * \param matrice b
 * \return matrice x
 */
Matrix remontee(Matrix a, Matrix b)
{
    Matrix x = newMatrix(a->n, 1);
    int i, j;
    E tmp;
    
    for(i=((a->n)-1); i>=0; i--)
    {
	// Calcul de l'element a placer
	tmp = getElt(b, i, 0);
	
	for(j=(i+1); j<a->n; j++)
	    tmp -= (getElt(x, j, 0) * getElt(a, i, j));
	    
	tmp /= getElt(a, i, i);
	
	// On met l'element
	setElt(x, i, 0, tmp);
    }
    
    return x;
}

/**
 * Resoud une equatiuon linéaire par methode de gauss
 * \param matrice a
 * \param matrice b
 * \return matrice x
 */
Matrix solvegausspartiel(const Matrix a, const Matrix b)
{
    Matrix cpa, cpb, x;
    
    cpa = copyMatrix(a);
    cpb = copyMatrix(b);
    
    // On realise la triangularisation puis la remontee
    triangSup(cpa, cpb);
    x = remontee(cpa, cpb);
    
    deleteMatrix(cpa);
    deleteMatrix(cpb);
    
    return x;
}
