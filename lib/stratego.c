#include "stratego.h"
#include <stdio.h> // A retirer à la fin ? pas besoin d'I/O pour l'IA logiquement
#include <stdlib.h>
#include <time.h>
/* Variables globales de la lib */

SBox boardIA[10][10]; // Notre "sauvegarde" du jeu
EColor colorIA; // Notre couleur de jeu
EColor colorEnemy; // Couleur de l'ennemi --> pas de recalcul à chauqe utilisation
int penalty; //Nombre de pénalités déjà jouées
int i,j; // Coordonnées ligne et colonne
int random; // Nombre pseudo aléatoire
/* Fonctions du .h */

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associé à la librairie
 */
void InitLibrary(char name[50])
{
	name="StrateRand"; //Jeu de mot Stratego Random / Stratego errant xD
}

/**
 * Initialiser l'IA pour un match
 */
void StartMatch()
{
	// QUOI FAIRE ICI ?
	printf("StartMatch");
}

/**
 * Initialiser l'IA pour une manche (d'un match)
 * @param const EColor color
 *	couleur du joueur
 * @param EPiece boardInit[4][10]
 *	variable en sortie : initialisation du jeu pour un joueur (ligne 0 = ligne la plus proche du joueur)
 */
void StartGame(const EColor color,EPiece boardInit[4][10])
{
	srand(time(NULL)); // Départ de la séquence rand en fonction du temps
	unsigned int PawnsLeft[11]=[6,1,8,5,4,4,4,3,2,1,1,1] //Tableau des pions restants à placer

	// Placement du drapeau + alentour : zone 3*2 [5 0 5][0 11 0]
	random=PseudoRandom(0,7);
	for (i=0;i<2;i++)
	{
		for(j=0;j<j+3;j++)
		{
			if ((j==0 || j==2) && i==1)
			{
				boardInit[i][j+random]=5;
				PawnsLeft[5]--;
			}
			else if (j==1 && i=0)
			{
				boardInit[i][j+random]=11;
				PawnsLeft[11]--;
			}
			else
			{	
				boardInit[i][j+random]=0;
				PawnsLeft[0]--;
			}
		}
	}
	
	
	// Placement des lignes 0 et 1
	for (i=0;i<2;i++)
	{
		for (j=0;j<10;j=j+2)
		{
			if (boardInit[i][j]==NULL)
			{
				// Placement d'une première pièce
				do 
				{
					random=PseudoRandom(0,8);
				}while(PawnsLeft[random]==0);

				boardInit[i][j]=random;
				PawnsLeft[random]--;

				// Placement d'une deuxième pièce
				if (random>3) // Si la première pièce est "forte"
				{
					do
					{
						random=PseudoRandom(0,3);
					}while(PawnsLeft[random]==0);

					boardInit[i][j+1]=random;
					PawnsLeft[random]--;
				}
				else // Si la première pièce est "faible"
				{
					do
					{
						random=PseudoRandom(4,8);
					}while(PawnsLeft[random]==0);

					boardInit[i][j+1]=random;
					PawnsLeft[random]--;
				}
			}
		}
	}

	// Placement des lignes 2 et 3
	for (i=2;i<4;i++)
	{
		for (j=0;j<10;j=j+2)
		{
			// Placement d'une première pièce
			do 
			{
				random=PseudoRandom(0,10);
			}while(PawnsLeft[random]==0);

			boardInit[i][j]=random;
			PawnsLeft[random]--;

			// Placement d'une deuxième pièce
			if (random>4) // Si la première pièce est "forte"
			{
				do
				{
					random=PseudoRandom(0,4);
				}while(PawnsLeft[random]==0);

				boardInit[i][j+1]=random;
				PawnsLeft[random]--;
			}
			else // Si la première pièce est "faible"
			{
				do
				{
					random=PseudoRandom(5,10);
				}while(PawnsLeft[random]==0);

				boardInit[i][j+1]=random;
				PawnsLeft[random]--;
			}
		}
	}

	
	// Stockage des couleurs
	if (colorIA=color==ECred)//Attribution des couleurs des joueurs
		colorEnemy=ECblue;
	else
		colorEnemy=ECred;
	
}

/**
 * Fin d'une manche (d'un match)
 */
void EndGame()
{
	int i,j;
	for (i=0; i<4;i++)
	{
		for (j=0;j<10;j++)
		{
			boardIA[i][j]=EPnone; //On remplace toute pièce du board par EPnone
		}
	}
}

/**
 * Fin d'un match
 */
void EndMatch()
{
	// libérer la mémoire ?
	printf("EndMatch");
}

/**
 * Prise de décision de la part de l'IA
 * @param const SGameState * const gameState
 *	l'état du jeu courant
 * @return SMove
 *	mouvement à effectuer par l'IA
 */
SMove NextMove(const SGameState * const gameState)
{
	updateBoard(gamestate); // Mise à jour de notre board en fonction des changement constatés sur le gamestate
	
	SMove next;
	
	// Si l'on peut gagner une attaque de manière certaine à ce tour le faire
	// Sinon déplacer une pièce qui le peut au hasard
	
	
}

/**
 * Indication du résultat d'une attaque (résultat envoyé à tous les joueurs)
 * @param SPos armyPos
 *	position de l'élément de l'armée du joueur (attaquant ou attaquée)
 * @param EPiece armyPiece
 *	type de de l'élément de l'armée du joueur (attaquant ou attaquée)
 * @param SPos enemyPos
 *	position de l'élément de l'ennemi (attaquant ou attaquée)
 * @param EPiece enemyPiece
 *	type de de l'élément de l'ennemi (attaquant ou attaquée)
 */
void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	// Mettre à jour le board en fonction de l'attaque
	
	//Tester si il y a des pièces spéciales parmi celles en jeu dans l'attaque
	// Si oui tester en fonction
	
	// Sinon cas global : qui a la plus grosse ?
	//si égalité les deux sont éliminées
	
		
}

/**
 * Indication d'une pénalitée pour règle non respectée -> coup précédent annulé
 */
void Penalty()
{
	penalty++;
}

/* Nos fonctions spécifiques */

/**
 * Génération d'un nombre pseudo aléatoire ddans les bornes précisées
 * @param int a
 *  borne inférieure 
 * @param int b
 *  borne supérieure
 * @return int 
 *  nombre pseudo-aléatoire contenue entre les bornes a et b
 */
int PseudoRandom(int a,int b)
{
	return rand()%(b-a) +a;
}









