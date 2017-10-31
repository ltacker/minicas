/**
 * \file parser.c
 * Implemente les methodes pour analyser les entrées
 */
#include "../include/parser.h"


/**
 * Cette fonction décompose la chaîne fournie selon le séparateur
 * indiqué, en préservant les éléments vides.
 * @param chaine chaîne à décomposer (modifiée)
 * @param sep séparateur
 * @param tab tableau à remplir
 * @param max nombre de cases dans le tableau argv
 * @return nombre de cases remplies, -1 si max est dépassé
 */
ssize_t separer(char chaine [], char sep, char *tab [], ssize_t max)
{
    char *p;
    ssize_t ntab;

    ntab = 0 ;
    tab [ntab++] = p = chaine;
    while(*p != '\0')
    {
	while(*p != '\0' && *p != sep)
	    p++;

	if (*p == sep)
	{
	    *p++ = '\0';
	    if(ntab < max)
		tab [ntab++] = p;
	    else
	    {
		ntab = -1;
		break;
	    }
	}
    }
    return ntab ;
}


/**
 * Retire les espaces d'une chaine de caractere
 * \param chaine de caractere
 */
void removeSpaces(char* source)
{
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
	*i = *j++;
	if(*i != ' ')
	    i++;
    }
    *i = 0;
}


/**
 * Obtient a partir d'une ligne un flottant
 * \param parametre
 * \param flottant
 * \return 1 succes, 0 echec
 */
int getFloat(char* s, float* f)
{
    Var tmpVar;
    
    // On regarde s'il s'agit d'une valeur absolue
    if(!strcmp(s, "0"))
	(*f) = 0.0;
    else if(atoi(s) != 0)
	(*f) = (float)atoi(s);
    else
    {
	// Si ce n'est pas une valeur on l'a cherche en tant que variable
	tmpVar = getVar(s);
	if(tmpVar == NULL)
	{
	    fprintf(stderr, "%s inconnue\n", s);
	    return 0;
	}
	if(tmpVar->type != type_float)
	{
	    fprintf(stderr, "%s n est pas un scalaire\n", s);
	    return 0;
	}
	(*f) = tmpVar->f;
    }
    
    return 1;
}


/**
 * Obtient a partir d'une ligne de parametre une matrice
 * \param parametre
 * \param matrice
 * \return 1 succes, 0 echec
 */
int getMatrix(char* s, Matrix* m)
{
    Var tmpVar;
    
    // On recupere la matrice
    tmpVar = getVar(s);
    if(tmpVar == NULL)
    {
	fprintf(stderr, "%s inconnue\n", s);
	return 0;
    }
    if(tmpVar->type != type_matrix)
    {
	fprintf(stderr, "%s n est pas une matrice\n", s);
	return 0;
    }
    
    (*m) = tmpVar->m;
    return 1;
}


/**
 * Obtient a partir d'une ligne de parametre deux matrices
 * \param parametre
 * \param matrice 1
 * \param matrice 2
 * \return 1 succes, 0 echec
 */
int getTwoMatrix(char* s, Matrix* m1, Matrix* m2)
{
    char* tab[MAX_TAB];
    
    // On separe les deux arguments
    if(separer(s, ',', tab, MAX_TAB) != 2)
    {
	fprintf(stderr, "necessite 2 argument\n");
	return 0;
    }
    
    // On recupere le 1er argument
    if(!getMatrix(tab[0], m1))
	return 0;
    
    // On recupere le 2eme argument
    if(!getMatrix(tab[1], m2))
	return 0;
    
    return 1;
}


/**
 * Obtient a partir d'une ligne de parametre une matrice et un float
 * \param parametre
 * \param matrice
 * \param float
 * \return 1 succes, 0 echec
 */
int getMatrixAndFloat(char* s, Matrix* m, float* f)
{
    char* tab[MAX_TAB];
    Var tmpVar;
    
    // On separe les deux arguments
    if(separer(s, ',', tab, MAX_TAB) != 2)
    {
	fprintf(stderr, "necessite 2 argument\n");
	return 0;
    }
    
    // On recupere le 1er argument
    if(!getMatrix(tab[0], m))
	return 0;
    
    // On recupere le 2eme argument
    if(!getFloat(tab[1], f))
	return 0;
    
    return 1;
}


/** /////////////////////////////////////////////////////////////////////
 * Fonction commandes
 * Les fonctions suivante prennent toutes en parametre
 * le parametre de la commande et renvois une matrice
 * excepté pour comDeterminant, comRank, comDecomp et comMc qui renvois un code d'erreur
 */


/**
 * Interprete la matrice a partir du parametre de matrix
 */
Matrix comMatrix(char* param)
{
    Matrix ret;
    Var tmpVar;
    int l, c, i, j;
    char* ligne[MAX_TAB];
    char* colonne[MAX_TAB];
    float tmp;
    
    // On separe les lignes pour la matrices
    l = (separer(param, ']', ligne, MAX_TAB)-1);
    if(l < 1)
    {
	fprintf(stderr, "commande incorrect (1)\n");
	return NULL;
    }
    
    // On regarde si la premiere ligne est valide (commence par '[')
    if(strlen(ligne[0]) < 2)
    {
	fprintf(stderr, "commande incorrect (2)\n");
	return NULL;
    }
    else if(memcmp(ligne[0], "[", 1) != 0)
    {
	fprintf(stderr, "commande incorrect (3)\n");
	return NULL;
    }
    (ligne[0])++;
    
    // On recupere le nombre de colonne
    c = separer(ligne[0], ',', colonne, MAX_TAB);
    if(c < 1)
    {
	fprintf(stderr, "commande incorrect (4)\n");
	return NULL;
    }
    
    // On alloue la matrice
    ret = newMatrix(l, c);
    
    // on remplie la premiere ligne
    for(j=0; j<c; j++)
    {
	// On recupere l'element
	if(!getFloat(colonne[j], &tmp))
	    return 0;
	
	// On place l'element
	setElt(ret, 0, j, tmp);
    }
    
    
    // on remplie les autres lignes
    for(i=1; i<l; i++)
    {
	// On regarde si la ligne est valide (commence par ,[)
	if(strlen(ligne[i]) < 3)
	{
	    fprintf(stderr, "commande incorrect (5)\n");
	    return NULL;
	}
	else if(memcmp(ligne[i], ",[", 2) != 0)
	{
	    fprintf(stderr, "commande incorrect (6)\n");
	    return NULL;
	}
	(ligne[i]) += 2;
    
	// On recupere les colonnes
	if(separer(ligne[i], ',', colonne, MAX_TAB) != c)
	{
	    fprintf(stderr, "ligne %d a un nombre de colonne incorrect\n", i+1);
	    return NULL;
	}
	
	// on remplie la ligne
	for(j=0; j<c; j++)
	{
	    // On recupere l'element
	    if(!getFloat(colonne[j], &tmp))
		return 0;
		
	    // On place l'element
	    setElt(ret, i, j, tmp);
	}
    }
    
    return ret;
}

/**
 * Realise l'addition de deux matrices
 */
Matrix comAddition(char* param)
{
    Matrix m1, m2;
    
    if(!getTwoMatrix(param, &m1, &m2))
	return NULL;

    return addition(m1, m2);
}

/**
 * Realise la soustraction de deux matrices
 */
Matrix comSub(char* param)
{
    Matrix m1, m2;
    
    if(!getTwoMatrix(param, &m1, &m2))
	return NULL;

    return soustraction(m1, m2);
}

/**
 * Realise la multiplication de deux matrices
 */
Matrix comMult(char* param)
{
    Matrix m1, m2;
    
    if(!getTwoMatrix(param, &m1, &m2))
	return NULL;

    return multiplication(m1, m2);
}

/**
 * Realise la multiplication de la matrice par un scalaire
 */
Matrix comMultscal(char* param)
{
    Matrix m;
    float f;
    
    if(!getMatrixAndFloat(param, &m, &f))
	return NULL;

    return multScal(f, m);
}

/**
 * Realise l'exponentiation de la matrice
 */
Matrix comExpo(char* param)
{
    Matrix m;
    float f;
    
    if(!getMatrixAndFloat(param, &m, &f))
	return NULL;

    return exposant(f, m);
}

/**
 * Realise la transposée d'une matrice
 */
Matrix comTranspose(char* param)
{
    Matrix m;
    
    if(!getMatrix(param, &m))
	return NULL;
    
    return transpose(m);
}

/**
 * Inverse une matrice
 */
Matrix comInvert(char* param)
{
    Matrix m;
    
    if(!getMatrix(param, &m))
	return NULL;
    
    return invert(m);
}

/**
 * Resoud une un systeme lineaire ax = b
 */
Matrix comSolve(char* param)
{
    Matrix m1, m2;
    
    if(!getTwoMatrix(param, &m1, &m2))
	return NULL;

    return solvegausspartiel(m1, m2);
}

/**
 * Calcule le determinant d'une matrice
 */
int comDeterminant(char* param, float* ret)
{
    Matrix m;
    
    if(!getMatrix(param, &m))
	return 0;
    
    (*ret) = (float)determinant(m);
    return 1;
}

/**
 * Calcule le rang d'une matrice
 */
int comRank(char* param, float* ret)
{
    Matrix m;
    
    if(!getMatrix(param, &m))
	return 0;
    
    (*ret) = (float)rang(m);
    return 1;
}


/**
 * Decompose la matrice
 * Les deux matrices triangulaires sont respectivement stocké dans la variable L et U
 */
int comDecomp(char* param)
{
    Matrix m, l, u, p;
    
    if(!getMatrix(param, &m))
	return 0;
	
    // Decomposition de la mtrice
    if(!decompose(m, &p, &l, &u))
	return 0;
	
    // On affiche les matrices
    printf("%s%s:\n", SPACING, MATRICE_P);
    displayMatrix(p);
    printf("%s%s:\n", SPACING, MATRICE_L);
    displayMatrix(l);
    printf("%s%s:\n", SPACING, MATRICE_U);
    displayMatrix(u);
    addVar(p, 0, MATRICE_P);
    addVar(l, 0, MATRICE_L);
    addVar(u, 0, MATRICE_U);
    
    return 1;
} 

/**
 * Obtient la droite des moindres carres + residu
 * La matrice de la droite est stockée dans la variable "droite"
 * La matrice des résidus est stockée dans la variable "residu"
 */
int comMc(char* param)
{
    Matrix m, d, r;
    char* tab[MAX_TAB];
    int useFile = 0;
    
    // On recupere la matrice et eventuellement le nom du fichier
    if(separer(param, ',', tab, MAX_TAB) == 2)
	useFile = 1;
	
    if(!getMatrix(tab[0], &m))
	return 0;
	
    // On calcule la droite
    if(!leastEstimate(m, &d, &r))
	return 0;
	
    // On affiche les matrices
    printf("%s%s:\n", SPACING, MATRICE_DROITE);
    displayMatrix(d);
    printf("%s%s:\n", SPACING, MATRICE_RESIDUS);
    displayMatrix(r);
    addVar(d, 0, MATRICE_DROITE);
    addVar(r, 0, MATRICE_RESIDUS);
    
    // On creer un fichier image de la droite
    if(useFile)
    {
	if(!plotEstimate(tab[1], m, d))
	    return 0;
    }
    
    return 1;
}


/// /////////////////////////////////////////////////////////////////////



/**
 * Analyse le nom de la commande et l'appelle avec ses parametres
 * \param commandes
 * \param nom de la variable de retour, NULL si pas de retour
 * \return 1 success, 0 echec
 */
int parseCommande(char* commande, char* name)
{
    char* tab[MAX_TAB];
    int tmp;
    
    // On separe le nom de la fonction des parametre
    tmp = separer(commande, '(', tab, MAX_TAB);
    if(tmp < 1 || tmp > 2)
    {
	fprintf(stderr, "Commande incorrect (7)\n");
	return 0;
    }
    else if(tmp == 1)
    {
	// Il n'y a pas de '(' donc c'est une affectation d'un float
	float f;
	
	if(strlen(tab[0]) == 0)
	{
	    fprintf(stderr, "Commande inconnue\n");
	    return 0;
	}
	else if(!memcmp(tab[0], "0", 1))
	{
	    // On distingue le cas de 0 car l'appel de atoi renvoie 0 si la chaine est incorrect
	    f = 0.0;
	}
	else
	{
	    // L'utilisateur ne rentre que des valeurs entieres
	    f = (float)atoi(tab[0]);
	    if(f == 0)
	    {
		fprintf(stderr, "Commande inconnue\n");
		return 0;
	    }
	}
	
	// Affichage du resultat
	displayFloat(f);
	if(name != NULL)
	{
	    if(!addVar(NULL, f, name))
		return 0;
	}
    }
    else if(tmp == 2)
    {
	// Il y a '(' donc c'est une commade avec argument
	int typeCom = type_matrix;
	Matrix m;
	float f;
	
	// Affectation d'une commande
	if(strlen(tab[0]) == 0 || strlen(tab[1]) == 0)
	{
	    fprintf(stderr, "Commande incorrect (8)\n");
	    return 0;
	}
	
	// On regarde si le dernier caractere du parametre est ')'
	if(memcmp(tab[1] + (strlen(tab[1])-1), ")", 1) != 0)
	{
	    fprintf(stderr, "Commande incorrect (9)\n");
	    return 0;
	}
	// On supprime la fermeture de parenthese a la fin
	memcpy(tab[1] + (strlen(tab[1])-1), "\0", 1);
	
	// On cherche la fonction a executer
	if(!strcmp(tab[0], COMMANDE_MATRIX))
	{
	    m = comMatrix(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_ADDITION))
	{
	    m = comAddition(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_SUB))
	{
	    m = comSub(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_MULT))
	{
	    m = comMult(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_MULTSCAL))
	{
	    m = comMultscal(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_EXPO))
	{
	    m = comExpo(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_TRANSPOSE))
	{
	    m = comTranspose(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_INVERT))
	{
	    m = comInvert(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_SOLVE))
	{
	    m = comSolve(tab[1]);
	    if(m == NULL)
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_DETERMINANT))
	{
	    typeCom = type_float;
	    
	    if(!comDeterminant(tab[1], &f))
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_RANK))
	{
	    typeCom = type_float;
	    
	    if(!comRank(tab[1], &f))
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_DECOMPOSE))
	{
	    typeCom = type_float;
	    f = 1;
	    
	    if(!comDecomp(tab[1]))
		return 0;
	}
	else if(!strcmp(tab[0], COMMANDE_MOINDRECARRE))
	{
	    typeCom = type_float;
	    f = 1;
	    
	    if(!comMc(tab[1]))
		return 0;
	}
	else
	{
	    fprintf(stderr, "Fonction inconnue : %s\n", tab[0]);
	    return 0;
	}
	
	// On affiche la solution en fonction de son type
	if(typeCom == type_matrix)
	{
	    displayMatrix(m);
	    if(name != NULL)
	    {
		// On l'ajoute aux variables
		if(!addVar(m, 0.0, name))
		    return 0;
	    }
	    else
		deleteMatrix(m);
	}
	else
	{
	    displayFloat(f);
	    if(name != NULL)
	    {
		// On l'ajoute aux variables
		if(!addVar(NULL, f, name))
		    return 0;
	    }
	}
    }
    
    return 1;
}


/**
 * Analyse une ligne et execute ce qu'elle contient
 * \param ligne
 * \return 1 succes, 0 echec, 2 commande quit
 */
int parseLine(char* line)
{
    ssize_t sz = strlen(line);
    int i, tmp;
    char* tab[MAX_TAB];
    char varName[MAX_NAME+1];
    char commande[MAX_COMMANDE+1];
    
    // On verifie d'abord la commande quit et speedtest qui sont differentes
    if(sz >= 4 && !memcmp(COMMANDE_QUIT, line, strlen(COMMANDE_QUIT)))
    {
	return 2;
    }
    if(sz >= 10 && !memcmp(COMMANDE_SPEEDTEST, line, SPEEDTEST_SZ))
    {
	line += SPEEDTEST_SZ;
	return speedtest(line);
    }
    
    // Il s'agit un appel a une fonction, on retire tout les espaces
    removeSpaces(line);
    sz = strlen(line);
    
    // Separe le nom de variable de la commande
    tmp = separer(line, ':', tab, MAX_TAB);
    if(tmp < 1 || tmp > 2)
    {
	fprintf(stderr, "Commande incorrect (10)\n");
	return 0;
    }
    if(tmp == 1)
    {
	// Pas d'assignation : on affiche juste la commande
	strncpy(commande, tab[0], MAX_COMMANDE);
	
	if(!parseCommande(commande, NULL))
	    return 0;
    }
    if(tmp == 2)
    {
	// Assignation : on realise la commande et on la stocke dans la variable
	strncpy(varName, tab[0], MAX_NAME);
	strncpy(commande, tab[1], MAX_COMMANDE);
	
	if(!parseCommande(commande, varName))
	    return 0;
    }
    
    return 1;
}
