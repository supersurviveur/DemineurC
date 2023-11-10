/**
 * @file generation.h
 * @brief Provides functions to generate the grid of the game
 * @authors Julien THILLARD, Clement DI-GALLO, Benjamin TRUC
*/

/**
 * @brief Génère un tableau de nbLignes lignes et de nbColonnes colonnes, puis le rempli avec nbBombs bombes
 * @param nbColonnes Nombre de colonnes du tableau
 * @param nbLignes Nombre de lignes du tableau
 * @param nbBombes Nombre de bombes à placer dans le tableau
 * @param x Coordonnée x de la première case cliquée par le joueur
 * @param y Coordonnée y de la première case cliquée par le joueur
 * @return Un tableau linéarisé de taille nbColonnes * nbLignes, contenant les bombes et les nombres de bombes voisines
 */
int *creeTabAvecBombes(int nbColonnes, int nbLignes, int nbBombes, int x, int y);

/**
 * @brief Initialize raandom generation
*/
void initializeGeneration();
