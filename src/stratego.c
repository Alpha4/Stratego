#include "stratego.h"

/* Variables globales de la lib */

SBox boardIA[10][10]; // Notre "sauvegarde" du jeu
EColor colorIA; // Notre couleur de jeu
int penalty; //Nombre de pénalités déjà jouées

/* Fonctions du .h */

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associé à la librairie
 */
void InitLibrary(char name[50])
{
	name="A_TROUVER";
}

/**
 * Initialiser l'IA pour un match
 */
void StartMatch()
{
	// QUOI FAIRE ICI ?
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
	boardInit[3]=[6,2,2,5,2,6,3,10,2,6];
	boardInit[2]=[5,4,0,1,9,2,7,7,8,2];
	boardinit[1]=[4,0,4,7,8,5,0,5,6,4];
	boardinit[0]=[2,3,0,2,3,0,11,0,3,3];
	// Placement en dur des pièces --> a remplacer par qq chose de moins prédictif
	
	colorIA=color; //Récupération de notre couleur
	
	updateBoard(); // A définir (update du board)
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
			boardIA[i][j]=12; //On remplace toute pièce du board par EPnone
		}
	}
}

/**
 * Fin d'un match
 */
void EndMatch()
{
	// libérer la mémoire ?
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
	
	// Prendre une pièce de la ligne la plus avancée et la faire avancer ou déplacement sur le côté si Lake face à elle.
	
	
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
	updateBoard() // Mettre à jour le board en fonction de l'attaque
	//faire directement dans la fonction la maj ? --> certainement
	
	//Tester si il y a des pièces spéciales parmi celles en jeu dans l'attaque
	// Si oui tester en fonction
	
	// Sinon cas global : qui a la plus grosse ?
	
	
		
}

/**
 * Indication d'une pénalitée pour règle non respectée -> coup précédent annulé
 */
void Penalty()
{
	penalty++;
}

