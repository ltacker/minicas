/**
 * \file plot.c
 * defini les fonctions pour le plotting
 */
#include "../include/plot.h" 

 
/**
 * Initialise le fichier plot
 * \param nom de la commande
 * \return 1 succes
 */
int plotInit(char* name)
{
    FILE * ret;
    char fname[PATH_MAX];
    
    // Cration du fichier
    strncpy(fname, TMP, PATH_MAX);
    strncat(fname, name, PATH_MAX);
    strncat(fname, DAT, PATH_MAX);
    ret = fopen(fname, "w");
    if(ret == NULL)
    {
	fprintf(stderr, "plot : erreur fopen\n");
	return 0;
    }
    
    fclose(ret);
    
    return 1;
}
 
 
/**
 * Ajoute une valeur dans le graphe finale
 * \param nom du fichier representant le plot
 * \param taille de donnée
 * \param temps de calcul
 */
void plotAdd(char* name, int t, double nb)
{
    FILE* f;
    char fname[PATH_MAX];
    
    strncpy(fname, TMP, PATH_MAX);
    strncat(fname, name, PATH_MAX);
    strncat(fname, DAT, PATH_MAX);
    
    f = fopen(fname, "a");
    fprintf(f, "%d %d\n", t, (int)nb);
    fclose(f);
}


/**
 * Sauvegarde le graphe le graphe dans un fichier image pour le speedtest
 * \param nom de la commande
 * \return 1 succes, 0 echec
 */
int plotSaveSpeedtest(char* name)
{
    FILE * plot;
    char fname[PATH_MAX];
    char buf[1024] = "";
    
    // creation du pipe pour gnuplot
    plot = popen("gnuplot", "w");
    if(plot == NULL)
    {
	fprintf(stderr, "plot : erreur popen\n");
	return 0;
    }
    
    // Ajout des options du fichier
    snprintf(buf, 1024, "set title \"%s\";", name);
    strncat(buf, "set xlabel \"Taille de matrice\";", 1024);
    strncat(buf, "set ylabel \"Temps de calcul (micro seconde)\";", 1024);
    strncat(buf, "set autoscale;", 1024);
    strncat(buf, "set terminal png;", 1024);
    strncat(buf, "set output \"", 1024);
    strncat(buf, name, 1024);
    strncat(buf, ".png\";", 1024);
    strncat(buf, "replot;", 1024);
    strncat(buf, "set terminal x11", 1024);
    
    // instructions
    strncpy(fname, TMP, PATH_MAX);
    strncat(fname, name, PATH_MAX);
    strncat(fname, DAT, PATH_MAX);
    fprintf(plot, " plot \"%s\" with lines; %s\n", fname, buf);
    fflush(plot);
    
    // terminer l'envoi de commandes et fermer gnuplot
    //pause();
    pclose(plot);
    unlink(fname);
    
    return 1;
}


/**
 * Sauvegarde le graphe le graphe dans un fichier image pour les moindres carres
 * \param matrice de la droite
 * \return 1 succes, 0 echec
 */
int plotSaveMoindrecarre(char*name, Matrix droite)
{
    FILE * plot;
    char fname[PATH_MAX];
    char buf[1024] = "";
    float a, b;
    
    // On recupere la droite
    a = getElt(droite, 0, 0);
    b = getElt(droite, 1, 0);
    
    // creation du pipe pour gnuplot
    plot = popen("gnuplot", "w");
    if(plot == NULL)
    {
	fprintf(stderr, "plot : erreur popen\n");
	return 0;
    }
    
    // On defini la fonction de la droite
    fprintf(plot, "droite(x)=%f*x+%f\n", a, b);
    
    // Ajout des options du fichier
    snprintf(buf, 1024, "set title \"%s\";", name);
    strncat(buf, "set autoscale;", 1024);
    strncat(buf, "set terminal png;", 1024);
    strncat(buf, "set output \"", 1024);
    strncat(buf, name, 1024);
    strncat(buf, ".png\";", 1024);
    strncat(buf, "replot;", 1024);
    strncat(buf, "set terminal x11", 1024);
    
    // instructions
    strncpy(fname, TMP, PATH_MAX);
    strncat(fname, name, PATH_MAX);
    strncat(fname, DAT, PATH_MAX);
    fprintf(plot, " plot \"%s\", droite(x); %s\n", fname, buf);
    fflush(plot);
    
    // terminer l'envoi de commandes et fermer gnuplot
    //pause();
    pclose(plot);
    unlink(fname);
    
    return 1;
}
