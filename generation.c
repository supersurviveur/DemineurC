#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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

/**
 * @brief Convertis les coordonnées d'une cellule en un entier qui correspond à sa position dans un tableau linéarisé
 * @param nbColonnes Nombre de colonnes du tableau
 * @param colonne Colonne de la cellule
 * @param ligne Ligne de la cellule
 * @return L'entier correspondant à la position de la cellule dans le tableau linéarisé
 */
int coordsEnInt(int nbColonnes, int colonne, int ligne)
{
    return ligne * nbColonnes + colonne;
}

/**
 * @brief Rempli un tableau de 2 entiers avec les coordonnées de la cellule de numéro n dans un tableau de nbColonnes colonnes (ligne, colonne)
 * @param nbColonnes Nombre de colonnes du tableau
 * @param n Numéro de la cellule dont on cherche les coordonnées
 * @param pointeurCoordonnees Pointeur vers un tableau de 2 entiers
 */
void intEnCoords(int nbColonnes, int n, int *pointeurCoordonnees)
{
    pointeurCoordonnees[0] = n / nbColonnes;
    pointeurCoordonnees[1] = n % nbColonnes;
}

/**
 * @brief Rempli un tableau de 9 entiers avec les coordonnées des cellules voisines à la cellule de coordonnées (lignedeint, colonnedeint)
 * @param tableauVoisins Tableau de 9 entiers
 * @param nbLignes Nombre de lignes du tableau
 * @param nbColonnes Nombre de colonnes du tableau
 * @param lignedeint Ligne de la cellule dont on cherche les voisins
 * @param colonnedeint Colonne de la cellule dont on cherche les voisins
 */
void listedesentierdevoisins(int *tableauVoisins, int nbLignes, int nbColonnes, int lignedeint, int colonnedeint)
{
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (0 <= lignedeint + y - 1 && lignedeint + y <= nbLignes && 0 <= colonnedeint + x - 1 && colonnedeint + x <= nbColonnes)
            { // si la case existe
                tableauVoisins[x + 3 * y] = coordsEnInt(nbColonnes, colonnedeint + x - 1, lignedeint + y - 1);
            }
            else
            { // si la case ne existe pas
                tableauVoisins[x + 3 * y] = -1;
            }
        }
    }
}

/**
 * @brief Génère un tableau de nbLignes lignes et de nbColonnes colonnes, puis le rempli avec nbBombs bombes
 * @param nbColonnes Nombre de colonnes du tableau
 * @param nbLignes Nombre de lignes du tableau
 * @param nbBombes Nombre de bombes à placer dans le tableau
 * @param x Coordonnée x de la première case cliquée par le joueur
 * @param y Coordonnée y de la première case cliquée par le joueur
 * @return Un tableau linéarisé de taille nbColonnes * nbLignes, contenant les bombes et les nombres de bombes voisines
 */
int *creeTabAvecBombes(int nbColonnes, int nbLignes, int nbBombes, int x, int y)
{
    int *tableau = generateTable(nbLignes, nbColonnes);                   // tableau est un tableau rempli de 0.
    int nombredebombesencoreaplacer = nbBombes % (nbColonnes * nbLignes); // si le nombre de bombes est supérieur à celui de la taille de la grille on se rapporte a un nombre plus petit avec le modulo
    while (nombredebombesencoreaplacer > 0)
    {
        int a = randint(0, (nbColonnes * nbLignes) - 1); // a est une case du tableau
        if (                                             // si la case n'est pas une bombe et n'est pas une case voisine de la case de depart (x,y)
            tableau[a] != BOMB && (a < coordsEnInt(nbColonnes, x - 1, y - 1) || a > coordsEnInt(nbColonnes, x + 1, y - 1)) && (a < coordsEnInt(nbColonnes, x - 1, y) || a > coordsEnInt(nbColonnes, x + 1, y)) && (a < coordsEnInt(nbColonnes, x - 1, y + 1) || a > coordsEnInt(nbColonnes, x + 1, y + 1)))
        {
            tableau[a] = BOMB;
            int tablecoordonnees[2];
            int tablevoisins[9];
            intEnCoords(nbColonnes, a, tablecoordonnees);                                                          // met dans tablecoordonnee les coordonnés de a.
            listedesentierdevoisins(tablevoisins, nbLignes, nbColonnes, tablecoordonnees[0], tablecoordonnees[1]); // met dans tablevoisin les voisins de a.
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
    initializeGeneration();

    assert(coordsEnInt(10, 5, 5) == 55);
    assert(coordsEnInt(10, 0, 0) == 0);
    assert(coordsEnInt(10, 9, 9) == 99);

    int position[2];
    intEnCoords(10, 55, position);
    assert(position[0] == 5);
    assert(position[1] == 5);
    intEnCoords(10, 0, position);
    assert(position[0] == 0);
    assert(position[1] == 0);
    intEnCoords(10, 99, position);
    assert(position[0] == 9);
    assert(position[1] == 9);

    int *ttbombe = creeTabAvecBombes(10, 10, 20, 0, 0); // (0, 0) is the first case the player clicked on
    assert(ttbombe[0] == 0);                            // Fisrt case must be empty, because the first case is the one the player clicked on
    free(ttbombe);

    // Check if listdesentierdevoisins works
    int voisins[9];
    listedesentierdevoisins(voisins, 10, 10, 1, 1);
    assert(voisins[0] == 0);
    assert(voisins[1] == 1);
    assert(voisins[2] == 2);
    assert(voisins[3] == 10);
    assert(voisins[4] == 11);
    assert(voisins[5] == 12);
    assert(voisins[6] == 20);

    printf("Tests passed successfully\n");
    return 0;
}
int main()
{
    return test();
}
#endif // TEST_GENERATION
