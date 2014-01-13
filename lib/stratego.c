#include "stratego.h"

/* Variables globales de la lib */

SBox boardIA[10][10]; // Notre "sauvegarde" du jeu
EColor colorIA; // Notre couleur de jeu
int penalty; //Nombre de p�nalit�s d�j� jou�es

/* Fonctions du .h */

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associ� � la librairie
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
	// Placement en dur des pi�ces --> a remplacer par qq chose de moins pr�dictif
	
	colorIA=color; //R�cup�ration de notre couleur
	
	updateBoard(); // A d�finir (update du board)
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
			boardIA[i][j]=12; //On remplace toute pi�ce du board par EPnone
		}
	}
}

/**
 * Fin d'un match
 */
void EndMatch()
{
	// lib�rer la m�moire ?
}

/**
 * Prise de d�cision de la part de l'IA
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @return SMove
 *	mouvement � effectuer par l'IA
 */
SMove NextMove(const SGameState * const gameState)
{
	updateBoard(gamestate); // Mise � jour de notre board en fonction des changement constat�s sur le gamestate
	
	SMove next;
	
	// Prendre une pi�ce de la ligne la plus avanc�e et la faire avancer ou d�placement sur le c�t� si Lake face � elle.
	
	
}

/**
 * Indication du r�sultat d'une attaque (r�sultat envoy� � tous les joueurs)
 * @param SPos armyPos
 *	position de l'�l�ment de l'arm�e du joueur (attaquant ou attaqu�e)
 * @param EPiece armyPiece
 *	type de de l'�l�ment de l'arm�e du joueur (attaquant ou attaqu�e)
 * @param SPos enemyPos
 *	position de l'�l�ment de l'ennemi (attaquant ou attaqu�e)
 * @param EPiece enemyPiece
 *	type de de l'�l�ment de l'ennemi (attaquant ou attaqu�e)
 */
void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	updateBoard() // Mettre � jour le board en fonction de l'attaque
	//faire directement dans la fonction la maj ? --> certainement
	
	//Tester si il y a des pi�ces sp�ciales parmi celles en jeu dans l'attaque
	// Si oui tester en fonction
	
	// Sinon cas global : qui a la plus grosse ?
	
	
		
}

/**
 * Indication d'une p�nalit�e pour r�gle non respect�e -> coup pr�c�dent annul�
 */
void Penalty()
{
	penalty++;
}

