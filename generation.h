/**
 * @file generation.h
 * @brief Provides functions to generate the grid of the game
 * @authors Julien THILLARD, Clement DI-GALLO, Benjamin TRUC
*/

int *creetableauaveclesbombes(int nombredecolonnes, int nombredelignes, int nombredebombes, int x, int y);
int *creetableauvide(int nombredelignes, int nombredecolonnes);

/**
 * @brief Initialize raandom generation
*/
void initializeGeneration(void);