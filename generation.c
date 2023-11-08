#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "core.h"

/**
 * @brief Génère un nombre entier aléatoire entre min (inclus) et max (inclus)
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return Le nombre aléatoire généré
 */
int randint(int min, int max)
{
    int n = (rand() % (max + 1 - min)) + min;
    return n;
}


// prend un numero de ligne et un numero de colonne et renvoie l'entier correspondant (dans le tableau de nombredelignes lignes et de nombredecolonnes colonnes)
int coordsEnInt(int nombredecolonnes, int ncolonne, int nligne)
{
    return nligne * nombredecolonnes + ncolonne;
}

// prend un entier et met change la valeur du poiteur en le numero de la ligne pour la premiere valeur et le numero de la colonne pour la seconde
void intEnCoords(int nombredecolonnes, int n, int *pointeurcoordonnees)
{
    pointeurcoordonnees[0] = n / nombredecolonnes;
    pointeurcoordonnees[1] = n % nombredecolonnes;
}

/*
entrees :
    tableauvoisin : un tableau de 9 entiers NÉCESSAIREMENT!!!!
    nombredelignes : nombre de lignes du tableau
    nombredecolonnes : nombre de colonnes du tableau
    lignedeint : ligne de la case dont on cherche les coordonnées des voisines orthogonalement et diagonalement
    colonndeint : colonne de la case dont on cherche les coordonnées des voisines orthogonalement et diagonalement

change les valeurs de tableauvoisin pour obtenir un tableau de neuf entiers correspondant aux valeurs des voisins
        s'il existe dans le tableau
        si la case n'existe pas, c'est la valeur de notre case qui y sera a la place
*/
void listedesentierdevoisins(int *tableauvoisin, int nombredelignes, int nombredecolonnes, int lignedeint, int colonnedeint)
{
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (0 <= lignedeint + y - 1 && lignedeint + y <= nombredelignes && 0 <= colonnedeint + x - 1 && colonnedeint + x <= nombredecolonnes)
            { // si la case existe
                tableauvoisin[x + 3 * y] = coordsEnInt(nombredecolonnes, colonnedeint + x - 1, lignedeint + y - 1);
            }
            else
            { // si la case ne existe pas
                tableauvoisin[x + 3 * y] = -1;
            }
        }
    }
}

/*cree la grille de jeu en fonction du nombre de colonnes (nombredecolonnes) du nombre de lignes (nombredelignes) et du nombre de bombes (nombredebombes) de la grille*/
int *creeTabAvecBombes(int nombredecolonnes, int nombredelignes, int nombredebombes, int x, int y)
{
    int *tableau = generateTable(nombredelignes, nombredecolonnes);                       // tableau est un tableau rempli de 0.
    int nombredebombesencoreaplacer = nombredebombes % (nombredecolonnes * nombredelignes); // si le nombre de bombes est supérieur à celui de la taille de la grille on se rapporte a un nombre plus petit avec le modulo
    while (nombredebombesencoreaplacer > 0)
    {
        int a = randint(0, (nombredecolonnes * nombredelignes) - 1); // a est une case du tableau
        if (
            tableau[a] != BOMB && a != coordsEnInt(nombredecolonnes, x, y) && a != coordsEnInt(nombredecolonnes, x + 1, y) && a != coordsEnInt(nombredecolonnes, x - 1, y) && a != coordsEnInt(nombredecolonnes, x, y + 1) && a != coordsEnInt(nombredecolonnes, x, y - 1) && a != coordsEnInt(nombredecolonnes, x + 1, y + 1) && a != coordsEnInt(nombredecolonnes, x - 1, y - 1) && a != coordsEnInt(nombredecolonnes, x + 1, y - 1) && a != coordsEnInt(nombredecolonnes, x - 1, y + 1) // si la case n'est pas une bombe et n'est pas une case voisine de la case de depart (x,y)
        )
        {
            tableau[a] = BOMB;
            int tablecoordonnees[2];
            int tablevoisins[9];
            intEnCoords(nombredecolonnes, a, tablecoordonnees);                                                             // met dans tablecoordonnee les coordonnés de a.
            listedesentierdevoisins(tablevoisins, nombredelignes, nombredecolonnes, tablecoordonnees[0], tablecoordonnees[1]); // met dans tablevoisin les voisins de a.
            for (int i = 0; i < 9; i++)
            {
                int b = tablevoisins[i];
                if (b != -1 && tableau[b] != BOMB)
                {
                    tableau[b] = tableau[b] + 1; // pour chaque voisin de a (qui n'est pas une bombe), on augmente la valeur (correspondant au nombre de bombes voisines) de 1.
                }
            }
            nombredebombesencoreaplacer--; // on réduit le nombre de bombes à placer, car on en à placer une
        }
    }
    return tableau;
}

/**
 * @brief Initialize random generation
 */
void initializeGeneration()
{
    srand((unsigned int)time(NULL)); // initialisation du debut de liste des nombres pseudo-aléatoire a la valeur du temps pour ne pas avoir toujours la meme grille
}

#ifdef TEST_GENERATION
int test()
{
    int nombredelignes = 16;
    int nombredecolonnes = 32;
    int nombredebombe = 128;
    printf("");
    initializeGeneration();

    int *ttvide = creeTabAvecBombes(nombredelignes, nombredecolonnes, 0);
    // ttvide est le tableau d'affichage

    int *ttbombe = creeTabAvecBombes(nombredelignes, nombredecolonnes, nombredebombe);
    // ttbombe est le tableau avec les bombes et les choffres

    // printTable(ttvide, nombredelignes, nombredecolonnes);
    // printTable(ttbombe, nombredelignes, nombredecolonnes);

    return 0;
}
int main()
{
    return test();
}
#endif // TEST_GENERATION
