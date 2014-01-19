#include <stdio.h> // A retirer à la fin ? pas besoin d'I/O pour l'IA logiquement
#include <stdlib.h>
#include <time.h>
#include "straterand.h"

/* Variables globales de la lib */

SGameState gameStateIA; // Notre "sauvegarde" du jeu
EColor colorIA; // Notre couleur de jeu
EColor colorEnemy; // Couleur de l'ennemi --> pas de recalcul à chauqe utilisation
int penalty; //Nombre de pénalités déjà jouées
int i,j; // Coordonnées ligne et colonne
int randomInt; // Nombre pseudo aléatoire
int turn; // Compteur de tours (uniquement les notres -> utile une fois pour le NextMove)
SMove last; // Notre dernier mouvement effectué

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
	turn=0; // Nombre de tours remis à 0
	srand(time(NULL)); // Départ de la séquence rand en fonction du temps
	unsigned int pawnsLeft[11]={6,1,8,5,4,4,4,3,2,1,1}; //Tableau des pions restants à placer

	// Placement du drapeau + alentour : zone 3*2 [5 0 5][0 11 0]
	randomInt=PseudoRandom(0,7);
	for (i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			if ((j==0 || j==2) && i==1) // Lieutenant
			{
				boardInit[i][j+randomInt]=5;
				pawnsLeft[5]--;
			}
			else if (j==1 && i==0) //Flag
			{
				boardInit[i][j+randomInt]=11;
			}
			else //Bomb
			{	
				boardInit[i][j+randomInt]=0;
				pawnsLeft[0]--;
			}
		}
	}
	
	
	// Placement des lignes 0 et 1
	for (i=0;i<2;i++)
	{
		for (j=0;j<10;j=j+2)
		{
			if (boardInit[i][j]==NULL && boardInit[i][j+1]!=NULL) // Cas spécial : case non remplie et la suivante est de la zone drapeau
			{
				if (boardInit[i][j+1]>3) // La pièce suivante est "forte"
				{
					do
					{
						randomInt=PseudoRandom(0,3); // Pièce avec une force entre 0 et 3
					}while(pawnsLeft[randomInt]==0);

					boardInit[i][j]=randomInt;
					pawnsLeft[randomInt]--;
				}
				else // Si la pièce suivante est "faible"
				{
					do
					{
						randomInt=PseudoRandom(4,8); // Pièce avec une force entre 0 et 3
					}while(pawnsLeft[randomInt]==0);

					boardInit[i][j]=randomInt;
					pawnsLeft[randomInt]--;
				}
			}

			else if (boardInit[i][j]==NULL) // Pas de pièce "de la zone drapeau" en [i][j]
			{
				// Placement d'une première pièce
				do 
				{
					randomInt=PseudoRandom(0,8); // Pièce avec une force entre 0 et 8
				}while(pawnsLeft[randomInt]==0); // Pièce encore disponible au placement ?

				boardInit[i][j]=randomInt;
				pawnsLeft[randomInt]--;
			}

			else // On prend la pièce de cette case pour générer la prochaine pièce si besoin
			{
				randomInt=boardInit[i][j];
			}

			if (boardInit[i][j+1]==NULL) // Pas de pièce de la "zone drapeau" en [i][j+1]
			{
				// Placement d'une deuxième pièce
				if (randomInt>3) // Si la première pièce est "forte"
				{
					do
					{
						randomInt=PseudoRandom(0,3); // Pièce avec une force entre 0 et 3
					}while(pawnsLeft[randomInt]==0);

					boardInit[i][j+1]=randomInt;
					pawnsLeft[randomInt]--;
				}
				else // Si la première pièce est "faible"
				{
					do
					{
						randomInt=PseudoRandom(4,8); // Pièce avec une force entre 0 et 3
					}while(pawnsLeft[randomInt]==0);

					boardInit[i][j+1]=randomInt;
					pawnsLeft[randomInt]--;
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
				randomInt=PseudoRandom(0,10);
			}while(pawnsLeft[randomInt]==0);

			boardInit[i][j]=randomInt;
			pawnsLeft[randomInt]--;

			// Placement d'une deuxième pièce
			if (randomInt>3) // Si la première pièce est "forte"
			{
				do
				{
					randomInt=PseudoRandom(0,3);
				}while(pawnsLeft[randomInt]==0);

				boardInit[i][j+1]=randomInt;
				pawnsLeft[randomInt]--;
			}
			else // Si la première pièce est "faible"
			{
				do
				{
					randomInt=PseudoRandom(4,10);
				}while(pawnsLeft[randomInt]==0);

				boardInit[i][j+1]=randomInt;
				pawnsLeft[randomInt]--;
			}
		}
	}

	// Stockage des couleurs
	if ((colorIA=color)==ECred)//Attribution des couleurs des joueurs
		colorEnemy=ECblue;
	else
		colorEnemy=ECred;
	
}

/**
 * Fin d'une manche (d'un match)
 */
void EndGame()
{
	/* INUTILE ?? 
	   Vu qu'on copie le premier Gamestate du premier appel de NextMove
	*/
	for (i=0; i<10;i++)
	{
		for (j=0;j<10;j++)
		{
			//On remplace toute pièce du board par EPnone
			gameStateIA.board[i][j].piece=EPnone;

			 // On replace les SBox "lake"
			if ((i==4 || i==5) && (j==2 || j==3 || j==6 || j==7))
				gameStateIA.board[i][j].content=EClake;

			//On remplace toute autre SBox du board par ECnone
			else
				gameStateIA.board[i][j].content=ECnone; 
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

	// Mise à jour de notre gameState
	if (turn==0)
	{
		for (i=0;i<10;i++)
		{
			for(j=0;j<10;j++)
			{
				gameStateIA.board[i][j].piece=gameState->board[i][j].piece;
				gameStateIA.board[i][j].content=gameState->board[i][j].content;
			}
		}

		for (i = 0 ; i < 11 ; i++)
   		{
	        gameStateIA.redOut[i] = 0;
	        gameStateIA.blueOut[i] = 0;
	    }
	}
	else
	{
		printf("test\n");
	}
	
	SMove next;
	
	// Si l'on peut gagner une attaque de manière certaine à ce tour le faire
	// Sinon déplacer une pièce qui le peut au hasard
	last=next; // on met à jour notre dernier coup
	return next;
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
	int result; // 0 --> égalité ; 1 --> Win ; -1 --> Lose
	
	
	// Cas spéciaux
	if (armyPiece==11) // Notre drapeau est pris
        result = -1;

    else if (enemyPiece==11) // On a pris le leur
        result = 1;

    // Une de nos bombes est en jeu
    else if (armyPiece==0)
    {
    	if (enemyPiece==3)
    		result = -1;

   		else
    		result = 1;
    }

    // Une de leurs bombes est en jeu
    else if (enemyPiece==0)
    {
    	if(armyPiece==3)
    		result=1;
    	else
    		result=-1;
    }

    // Marshal vs Spy
    else if((armyPiece==10 && enemyPiece==1) || (armyPiece==1 && enemyPiece==10))
    {
    	// On regarde s'il s'agit de notre attaque
    	// i.e.: armyPos est la position de départ de notre dernier move.
    	if (last.start.line==armyPos.line && last.start.col==armyPos.col)
    		result==1;
    	else
    		result=-1;
    }


	// Cas global :
	else if(armyPiece<enemyPiece)
        result = -1; // Attaque perdue
    
    else if (armyPiece>enemyPiece)
        result = 1; // Attaque remportée
    
    else
    	result = 0; // Égalité

    // Mettre à jour le board en fonction de l'attaque
	if (result == 1)
	{
		// Suppression de la pièce éliminée
		gameStateIA.board[enemyPos.line][enemyPos.col].content=ECnone;
		gameStateIA.board[enemyPos.line][enemyPos.col].piece=EPnone;
		if (colorEnemy==ECred)
			redOut[enemyPiece]++;
		else
			blueOut[enemyPiece]++;
	}
	else if (result == -1)
	{
		// Suppression de la pièce éliminée
		gameStateIA.board[armyPos.line][armyPos.col].content=ECnone;
		gameStateIA.board[armyPos.line][armyPos.col].piece=EPnone;
		if (colorIA==ECred)
			redOut[armyPiece]++;
		else
			blueOut[armyPiece]++;

		// On découvre la valeur d'une pièce ennemie toujours en jeu
		gameStateIA.board[enemyPos.line][enemyPos.col].piece=enemyPiece;
	}
	else //Les deux pièces disparaissent
	{
		gameStateIA.board[enemyPos.line][enemyPos.col].content=ECnone;
		gameStateIA.board[enemyPos.line][enemyPos.col].piece=EPnone;
		gameStateIA.board[armyPos.line][armyPos.col].content=ECnone;
		gameStateIA.board[armyPos.line][armyPos.col].piece=EPnone;
		if (colorIA==ECred)
			redOut[armyPiece]++;
			blueOut[enemyPiece]++;
		else
			blueOut[armyPiece]++;
			redOut[enemyPiece]++;
	}

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








