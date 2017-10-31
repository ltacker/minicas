/**
 * \file moindrecarre.c
 * Implemente la fonction de moindre carre et l'affichage de la droite
 */
#include "../include/moindrecarre.h"



/**
 * Creer un fichier image de la droite des moindre carre
 * \param nom du fichier image
 * \param matrice des points
 * \param matrice droite
 * \return 1 succes, 0 echec
 * Precondition : points est une matrice de taille n*2 et droite de taille 2*1
 */
int plotEstimate(char* name, Matrix points, Matrix droite)
{
    int i;

    if(!plotInit(name))
	return 0;
	
    // On met les points dans un fichier
    for(i=0; i<points->n; i++)
	plotAdd(name, getElt(points, i, 0), getElt(points, i, 1));
    
    // On affiche l'image
    if(!plotSaveMoindrecarre(name, droite))
	return 0;
	
    return 1;
}


/**
 * Obtient la droite des moindres carres et les residus
 * \param matrice contenant les coordonné des points
 * \param matrice de la droite
 * \param matrice des residus de chaques points
 * \return 1 succes, 0 echec
 */
int leastEstimate(Matrix points, Matrix* droite, Matrix* residus)
{
    Matrix a, b, at, d, r, tmp;
    int i;
    float f;
    
    // La matrice doit etre de taille n*2
    if(points->n < 2 || points->m != 2)
    {
	fprintf(stderr, "Matrice des points incorrect\n");
	return 0;
    }
    // On remplie les matrices a et b
    a = newMatrix(points->n, 2);
    b = newMatrix(points->n, 1);
    for(i=0; i<points->n; i++)
    {
	f = getElt(points, i, 0);
	setElt(a, i, 0, f);
	setElt(a, i, 1, 1);
	f = getElt(points, i, 1);
	setElt(b, i, 0, f);
    }
    
    // S'il n'y a que 2 points, nous pouvont directement obtenir la droite
    if(points->n == 2)
    {
	(*droite) = solvegausspartiel(a, b);
	(*residus) = newMatrix(points->n, 1);	// Il n'y aura pas de residu
	deleteMatrix(a);
	deleteMatrix(b);
	return 1;
    }
    
    // S'il y a plus d'equation que d'inconnu, nous devons utiliser les moindres carres
    at = transpose(a);
    
    // Nouvelle matrice
    tmp = multiplication(at, a);
    deleteMatrix(a);
    a = tmp;
    
    tmp = multiplication(at, b);
    deleteMatrix(b);
    b = tmp;
    
    // On obtient la droite
    d = solvegausspartiel(a, b);
    deleteMatrix(a);
    deleteMatrix(b);
    deleteMatrix(at);
    
    // On obtient chaque residu
    r = newMatrix(points->n, 1);
    for(i=0; i<points->n; i++)
    {
	//Position du point sur la droite
	f = getElt(d, 0, 0) * getElt(points, i, 0) + getElt(d, 1, 0);
	f -=  getElt(points, i, 1);
	
	if(f < 0)
	    f *= -1;
	
	setElt(r, i, 0, f);
    }
    
    (*droite) = d;
    (*residus) = r;
    
    return 1;
}
