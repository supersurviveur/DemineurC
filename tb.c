#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int const BOMB = 9; //symbole bombe a 9 pour avoir un tableau afficher rectangulaire
int nombredelignes = 8; 
int nombredecolonnes  = 12;
int nombredebombe =24;

int ocho=8;
int diew=10;


int tablecoordonnees[2]; //ne pas toucher
int tablevoisins[9]; //ne pas toucher


//randomin renvoie un nombre entier aleatoire entre min(inclue) et max(exclue)
int randomint(int  min, int max){
    int n = (rand() % (max+1-min)) + min;
    return n;
}

//prend un numero de ligne et un numero de colonne et renvoie l'entier correspondant (dans le tableau de nombredelignes lignes et de nombredecolonnes colonnes)
int coordonneesenint(int nombredelignes,int nombredecolonnes,int ncolonne,int nligne){
    if (nombredecolonnes<=nligne){
        nligne=nombredecolonnes;
    }
    return nligne*nombredelignes+ncolonne;
}

//prend un entier et met change la valeur du poiteur en le numero de la ligne pour la premiere valeur et le numero de la colonne pour la seconde
void intencoordonnees(int nombredelignes,int n,int*pointeurcoordonnees){
    pointeurcoordonnees[0]=n/nombredelignes;
    pointeurcoordonnees[1]=n%nombredelignes;
}

//cree et renvoie un tableau renplie de 0 de taille nombredelignes * nombredecolonnes
int * creetableauvide(int nombredelignes, int nombredecolonnes) {
    int * table = (int *) malloc(nombredelignes * nombredecolonnes * sizeof(int));
    if (table == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nombredelignes * nombredecolonnes; i++) {
        table[i] = 0;
    }
    return table;
}


//debutcodeclement--------------------------------------------------------------------------------------------------------------------
//reprise du code de clement sans aucun changement sert uniquement a ne pas le reecrire

void printTable(int *table, int rows, int columns) {
    for (int i = 0; i < rows * columns; i++) {
        if (i % columns == 0) {
            printf("\n");
        }
        printf("%d ", table[i]);
    }
    printf("\n");
}

void freeTable(int *table) {
    free(table);
}

void editTable(int *table, int rows, int columns, int row, int column, int value) {
    table[row * columns + column] = value;
}

int getTableValue(int *table, int rows, int columns, int row, int column) {
    return table[row * columns + column];
}

int countTable(const int *table, int rows, int columns, int value) {
    int count = 0;
    for (int i = 0; i < rows * columns; i++) {
        if (table[i] == value) {
            count++;
        }
    }
    return count;
}

//fincodeclement---------------------------------------------------------------------------------------------------------------



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
void listedesentierdevoisins(int *tableauvoisin, int nombredelignes, int nombredecolonnes, int lignedeint, int colonnedeint){
    for(int x=0;x<3;x++){
        for(int y=0;y<3;y++){
            if(0<=lignedeint+y-1 && lignedeint+y<=nombredecolonnes && 0<=colonnedeint+x-1 && colonnedeint+x<=nombredelignes){ //si la case existe
                tableauvoisin[x+3*y]=coordonneesenint(nombredelignes,nombredecolonnes,colonnedeint+x-1,lignedeint+y-1); 
            }else{ //si la case ne existe pas
                tableauvoisin[x+3*y]=coordonneesenint(nombredelignes,nombredecolonnes,colonnedeint,lignedeint);
            }
        }
    }
}

/*cree la grille de jeu en fonction du nombre de colonnes (nombredecolonnes) du nombre de lignes (nombredelignes) et du nombre de bombes (nombredebombes) de la grille*/
int * creetableauaveclesbombes(int nombredecolonnes,int nombredelignes,int nombredebombes){
    int * tableau=creetableauvide(nombredelignes,nombredecolonnes); //tableau est un tableau rempli de 0 
    int nombredebombesencoreaplacer=nombredebombes%(nombredecolonnes*nombredelignes); //si le nombre de bombes est superieur a celui de la taille de la grille on se raporte a un nombre plus petit avec le modulo
    while(nombredebombesencoreaplacer>0){  
        int a=randomint(0,(nombredecolonnes*nombredelignes)); //a est une case du tableau
        if (tableau[a]!=BOMB){ 
            tableau[a]=BOMB;
            intencoordonnees(nombredelignes,a,tablecoordonnees); //met dans tablecoordonnee les coordonnes de a 
            listedesentierdevoisins(tablevoisins, nombredelignes, nombredecolonnes,  tablecoordonnees[0],  tablecoordonnees[1]); //met dans tablevoisin les voisins de a
            for (int i=0;i<9;i++){
                int b=tablevoisins[i];
                if (tableau[b]!=BOMB){
                    tableau[b]=tableau[b]+1; //pour chaque voisin de a (qui n'est pas une bombe), on augmente la valeur (correspondant au nombre de bombes voisine) de 1
                }
            }
        nombredebombesencoreaplacer=nombredebombesencoreaplacer-1;//on reduit le nombre de bombes a placer car on en a placer une
        }
    }
    return tableau;
}

void difficulty(int n,int*p1,int*p2,int*p3){
    if(n==0){*p1=8;*p2=12;*p3=10;}
    if(n==1){*p1=12;*p2=20;*p3=30;}
    if(n==2){*p1=16;*p2=26;*p3=70;}
    if(n==3){*p1=20;*p2=32;*p3=120;}
    if(n==4){*p1=24;*p2=40;*p3=200;}
    if(n==5){*p1=28;*p2=45;*p3=300;}
    if(n==6){*p1=32;*p2=50;*p3=420;}
    if(n==7){*p1=50;*p2=80;*p3=1000;}
}


int main()
{
    printf("");
    srand(time(NULL)); //initialisation du debut de liste des nombre pseudo-aleatoire a la valeur du temps pour ne pas avoir toujour la meme grille


    difficulty(0,&nombredelignes,&nombredecolonnes,&nombredebombe);
    

    int * ttvide = creetableauaveclesbombes(nombredelignes,nombredecolonnes,0);
    //ttvide est le tableau d'affichage

    int * ttbombe = creetableauaveclesbombes(nombredelignes,nombredecolonnes,nombredebombe);
    //ttbombe est le tableau avec les bombes et les chiffres


    printTable(ttvide,nombredelignes,nombredecolonnes);
    printTable(ttbombe,nombredelignes,nombredecolonnes);


    return 0;
}
