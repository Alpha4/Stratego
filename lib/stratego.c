#include "stratego.h"
#include <stdio.h> // A retirer à la fin ? pas besoin d'I/O pour l'IA logiquement
#include <stdlib.h>
#include <time.h>
/* Variables globales de la lib */

SBox boardIA[10][10]; // Notre "sauvegarde" du jeu
EColor colorIA; // Notre couleur de jeu
EColor colorEnemy; // Couleur de l'ennemi --> pas de recalcul à chauqe utilisation
int penalty; //Nombre de pénalités déjà jouées

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

	boardInit[3]=[6,2,2,5,2,6,3,10,2,6];
	boardInit[2]=[5,4,0,1,9,2,7,7,8,2];
	boardinit[1]=[4,0,4,7,8,5,0,5,6,4];
	boardinit[0]=[2,3,0,2,3,0,11,0,3,3];
	// Placement en dur des pièces --> a remplacer par qq chose de moins prédictif
	
	//placer groupe de pièce drapeau + alentour --> zone de 3*3 ou 3*2 sur les lignes les plus basses
	
	
	//placer les premieres lignes
	
	//placer les lignes de front
	/*
		Pièce 0 : 6
		Pièce 1 : 1
		Pièce 2 : 8
		Pièce 3 : 5
		Pièce 4 : 4
		Pièce 5 : 4
		Pièce 6 : 4
		Pièce 7 : 3
		Pièce 8 : 2
		Pièce 9 : 1
		Pièce 10 : 1
		Pièce 11 : 1
	*/
		
	
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









