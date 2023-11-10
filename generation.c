#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

/**
 * @brief Génère un nombre entier aleatoire entre min (inclus) et max (exclus)
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return Le nombre aleatoire généré
 */
int randomint(int min, int max)
{
    int n = (rand() % (max + 1 - min)) + min;
    return n;
}

// prend un numero de ligne et un numero de colonne et renvoie l'entier correspondant (dans le tableau de nombredelignes lignes et de nombredecolonnes colonnes)
int coordonneesenint(int nombredelignes, int nombredecolonnes, int ncolonne, int nligne)
{
    if (nombredecolonnes <= nligne)
    {
        nligne = nombredecolonnes;
    }
    return nligne * nombredelignes + ncolonne;
}

// prend un entier et met change la valeur du poiteur en le numero de la ligne pour la premiere valeur et le numero de la colonne pour la seconde
void intencoordonnees(int nombredelignes, int n, int *pointeurcoordonnees)
{
    pointeurcoordonnees[0] = n / nombredelignes;
    pointeurcoordonnees[1] = n % nombredelignes;
}

// cree et renvoie un tableau renplie de 0 de taille nombredelignes * nombredecolonnes
int *creetableauvide(int nombredelignes, int nombredecolonnes)
{
    int *table = (int *)malloc(nombredelignes * nombredecolonnes * sizeof(int));
    if (table == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nombredelignes * nombredecolonnes; i++)
    {
        table[i] = 0;
    }
    return table;
}

/*
entrees :
    tableauvoisin : un tableau de 9 entier NECESSAIREMENT!!!!
    nombredelignes : nombres de lignes du tableau
    nombredecolonnes : nombres de colonnes du tableau
    lignedeint : ligne de la case dont on cherche les coordonnees des voisines orthoganalement et diagonalement
    colonndeint : colonne de la case dont on cherche les coordonnees des voisines orthoganalement et diagonalement

change les valeurs de tableauvoisin pour obtenir un tableau de neuf entiers correspondant aux valeurs des voisins
        si il existe dans le tableau
        si la case ne existe pas, c'est la valeur de notre case qui y sera a la place
*/
void listedesentierdevoisins(int *tableauvoisin, int nombredelignes, int nombredecolonnes, int lignedeint, int colonnedeint)
{
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (0 <= lignedeint + y - 1 && lignedeint + y <= nombredecolonnes && 0 <= colonnedeint + x - 1 && colonnedeint + x <= nombredelignes)
            { // si la case existe
                tableauvoisin[x + 3 * y] = coordonneesenint(nombredelignes, nombredecolonnes, colonnedeint + x - 1, lignedeint + y - 1);
            }
            else
            { // si la case ne existe pas
                tableauvoisin[x + 3 * y] = -1;
            }
        }
    }
}

/*cree la grille de jeu en fonction du nombre de colonnes (nombredecolonnes) du nombre de lignes (nombredelignes) et du nombre de bombes (nombredebombes) de la grille*/
int *creetableauaveclesbombes(int nombredecolonnes, int nombredelignes, int nombredebombes, int x, int y)
{
    int *tableau = creetableauvide(nombredelignes, nombredecolonnes);                       // tableau est un tableau rempli de 0
    int nombredebombesencoreaplacer = nombredebombes % (nombredecolonnes * nombredelignes); // si le nombre de bombes est superieur a celui de la taille de la grille on se raporte a un nombre plus petit avec le modulo
    while (nombredebombesencoreaplacer > 0)
    {
        int a = randomint(0, (nombredecolonnes * nombredelignes) - 1); // a est une case du tableau
        if (
            tableau[a] != BOMB && a != coordonneesenint(nombredelignes, nombredecolonnes, x, y) && a != coordonneesenint(nombredelignes, nombredecolonnes, x + 1, y) && a != coordonneesenint(nombredelignes, nombredecolonnes, x - 1, y) && a != coordonneesenint(nombredelignes, nombredecolonnes, x, y + 1) && a != coordonneesenint(nombredelignes, nombredecolonnes, x, y - 1) && a != coordonneesenint(nombredelignes, nombredecolonnes, x + 1, y + 1) && a != coordonneesenint(nombredelignes, nombredecolonnes, x - 1, y - 1) && a != coordonneesenint(nombredelignes, nombredecolonnes, x + 1, y - 1) && a != coordonneesenint(nombredelignes, nombredecolonnes, x - 1, y + 1) // si la case n'est pas une bombe et n'est pas une case voisine de la case de depart (x,y)
        )
        {
            tableau[a] = BOMB;
            int tablecoordonnees[2];
            int tablevoisins[9];
            intencoordonnees(nombredelignes, a, tablecoordonnees);                                                             // met dans tablecoordonnee les coordonnes de a
            listedesentierdevoisins(tablevoisins, nombredelignes, nombredecolonnes, tablecoordonnees[0], tablecoordonnees[1]); // met dans tablevoisin les voisins de a
            for (int i = 0; i < 9; i++)
            {
                int b = tablevoisins[i];
                if (b != -1 && tableau[b] != BOMB)
                {
                    tableau[b] = tableau[b] + 1; // pour chaque voisin de a (qui n'est pas une bombe), on augmente la valeur (correspondant au nombre de bombes voisine) de 1
                }
            }
            nombredebombesencoreaplacer = nombredebombesencoreaplacer - 1; // on reduit le nombre de bombes a placer car on en a placer une
        }
    }
    return tableau;
}

/**
 * @brief Initialize raandom generation
 */
void initializeGeneration(void)
{
    srand((unsigned int)time(NULL)); // initialisation du debut de liste des nombre pseudo-aleatoire a la valeur du temps pour ne pas avoir toujour la meme grille
}

#ifdef TEST
int test()
{
    int nombredelignes = 16;
    int nombredecolonnes = 32;
    int nombredebombe = 128;
    printf("");
    initializeGeneration();

    int *ttvide = creetableauaveclesbombes(nombredelignes, nombredecolonnes, 0);
    // ttvide est le tableau d'affichage

    int *ttbombe = creetableauaveclesbombes(nombredelignes, nombredecolonnes, nombredebombe);
    // ttbombe est le tableau avec les bombes et les choffres

    // printTable(ttvide, nombredelignes, nombredecolonnes);
    // printTable(ttbombe, nombredelignes, nombredecolonnes);

    return 0;
}
int main()
{
    return test();
}
#endif
