#include "../include/speedtest.h"



// Pid du processus fils pour calculer le temps d'une action
int pid;
// Conteur d'alarme pour savoir quand on doit areter le processus fils
int alarmCount;
// Definie si le programme a du s'arreter 
int arret;

/**
 * Arete le processus fils quand on a attein une limite de temps
 */
void timeout()
{
    alarmCount--;
    
    arret = 1;
    kill(pid, SIGKILL);
}



/**
 * Execute une commande avec une taille et renvois le temps pris par celle ci
 * \param commande
 * \param taille
 * \param nombre max de seconde en mode reel
 * \return temps pris par la commande, -1 si elle a echoué, -2 le temps s'est ecoulé
 */
double speedtestCommande(char* commande, int t, int smax)
{
    struct tms tmp;
    double nb, start;
    int c = 1;
    Matrix m1, m2, b, ret, points, tmp1, tmp2, tmp3;
    float tmpF;
    int err = 0;
    int st;
    
    // Valeur de test
    while(c)
    {
	// La premiere matrice doit etre inversible
	m1 = aleatoire(t, t, 0, 100);
	if(determinant(m1) != 0)
	    c = 0;
	else
	    deleteMatrix(m1);
    }
    m2 = aleatoire(t, t, 0, 100);
    b = aleatoire(t, 1, 0, 100);
    ret = NULL;
    
    // Temps de reference
    times(&tmp);
    start = (double)tmp.tms_cutime + (double)tmp.tms_cstime;
    
    // On creer le processus fils
    pid = 0;
    pid = fork();
    
    if(pid == -1)
    {
	fprintf(stderr, "speedtest: erreur fork\n");
	return -1;
    }
    else if(pid == 0)
    {
	ret = NULL;
	err = 0;
	
	// On realise l'operation
	if(!strcmp(commande, COMMANDE_ADDITION))
	{
	    ret = addition(m1, m2);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_SUB))
	{
	    ret = soustraction(m1, m2);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_MULT))
	{
	    ret = multiplication(m1, m2);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_MULTSCAL))
	{
	    ret = multScal(2, m1);	// le scalaire ne modifie pas le temps	
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_EXPO))
	{
	    ret = exposant(t, m1);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_TRANSPOSE))
	{
	    ret = transpose(m1);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_INVERT))
	{
	    ret = invert(m1);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_SOLVE))
	{
	    ret = solvegausspartiel(m1, b);
	    if(ret == NULL)
		err = 1;
	}
	else if(!strcmp(commande, COMMANDE_DETERMINANT))
	{
	    tmpF = determinant(m1);
	}
	else if(!strcmp(commande, COMMANDE_RANK))
	{
	    tmpF = rang(m1);
	}
	else if(!strcmp(commande, COMMANDE_MOINDRECARRE))
	{
	    if(t >= 2)
	    {
		points = newMatrix(t, 2);
		if(!leastEstimate(points, &tmp1, &tmp2))
		    err = 1;
		    
		deleteMatrix(points);
		if(!err)		// tmp sont seulement instanciées si la fonction a réussis
		{
		    deleteMatrix(tmp1);
		    deleteMatrix(tmp2);
		}
	    }
	}
	else if(!strcmp(commande, COMMANDE_DECOMPOSE))
	{
	    if(!decompose(m1, &tmp1, &tmp2, &tmp3))
		err = 1;
		
	    if(!err)			// tmp sont seulement instanciées si la fonction a réussis
	    {
		deleteMatrix(tmp1);
		deleteMatrix(tmp2);
		deleteMatrix(tmp3);
	    }
	}
	else
	{
	    fprintf(stderr, "speedtest : commande inconnue\n");
	    err = 1;
	}
	
	deleteMatrix(m1);
	deleteMatrix(m2);
	deleteMatrix(b);
	if(ret != NULL)
	    deleteMatrix(ret);
	
	if(err)
	    exit(1);
	
	exit(0);
    }
    
    // On initialise l'horloge de fin
    if(smax > 0)
    {
	alarmCount++;
	alarm(smax);
    }
    
    // On attend la fin du fils
    wait(&st);
    if(!WIFEXITED(st))
    {
	if(arret)
	    return -2;	// Arret par timeout
	else
	    return -1;	// Arret par erreur
    }
    else if(WEXITSTATUS(st) == 1)
    {
	return -1; // Erreur
    }
    
    // Calcul du temps pris
    times(&tmp);
    nb = (double)tmp.tms_cutime + (double)tmp.tms_cstime;
    nb -= start;
    nb /= (double)CLOCKS_PER_SEC;
    nb *= 1000000;
    
    deleteMatrix(m1);
    deleteMatrix(m2);
    deleteMatrix(b);
    
    return nb;
}



/**
 * Effectue le test de rapidite en fonction de la commande donnée
 * \param ligne : "commande taille_min taille_max pas [sec_max]"
 * \return 1 succes, 0 echec
 */
int speedtest(char* line)
{
    char* tab[MAX_TAB];
    int tmp, tMin, tMax, pas;
    int secMax = -1;
    double nb;
    
    // On obtient tout les elements de la ligne
    tmp = separer(line, ' ', tab, MAX_TAB);
    
    // L'utilisateur a specifier un nombre de seconde max
    if(tmp == 5)
    {
	arret = 0;
	alarmCount = 0;
	secMax = atoi(tab[4]);
	if(!secMax)
	{
	    fprintf(stderr, "speedtest : nombre de seconde incorrect\n");
	    return 0;
	}
	
	// Initialisation du signal d'arret
	signal(SIGALRM, &timeout);
    }
    else if(tmp != 4)
    {
	fprintf(stderr, "speedtest : mauvais nombre d argument\n");
	return 0;
    }
    
    tMin = atoi(tab[1]);
    tMax = atoi(tab[2]);
    pas = atoi(tab[3]);
    
    if(tMin == 0 || tMax == 0 || pas == 0)
    {
	fprintf(stderr, "speedtest : commande incorrect\n");
	return 0;
    }
    
    // On initialise le fichier pour l'affichage
    if(plotInit(tab[0]) == 0)
	return 0;
    
    // On effectue les calculs iterativement
    while(tMin <= tMax)
    {
	nb = speedtestCommande(tab[0], tMin, secMax);
	if(nb == -1)
	    return 0;
	
	// Si on a depasser le nombre de seconde on arete la boucle
	if(nb == -2)
	{
	    tMin = tMax + 1;
	}
	else
	{
	    plotAdd(tab[0], tMin, nb);
	    tMin += pas;
	}
    }
    
    // On sauvegarde le graphe
    if(!plotSaveSpeedtest(tab[0]))
	return 0;
    
    return 1;
}
