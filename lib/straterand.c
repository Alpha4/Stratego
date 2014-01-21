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
	
	// Placement des lignes 0 à 4
	for (i=0;i<4;i++)
	{
		for (j=0;j<10;j=j+2)
		{
			//Première pièce
			if (boardInit[i][j]==NULL && boardInit[i][j+1]!=NULL) // Cas spécial : case non remplie et la suivante est de la zone drapeau
			{
				if (boardInit[i][j+1]>3) // La pièce suivante est "forte"
				{
					
					boardInit[i][j]=PlacePiece(0,3,pawnsLeft); // On place la pièce
				}
				else // Si la pièce suivante est "faible"
				{
					boardInit[i][j]=PlacePiece(4,8,pawnsLeft);
				}
			}

			else if (boardInit[i][j]==NULL && i<2) // Pas de pièce "de la zone drapeau" en [i][j]
			{
				// Placement d'une première pièce
				boardInit[i][j]=PlacePiece(0,8,pawnsLeft);
			}

			else
				boardInit[i][j]=PlacePiece(0,10,pawnsLeft);
			
			//Deuxième pièce
			if (boardInit[i][j+1]==NULL) // Pas de pièce de la "zone drapeau" en [i][j+1] (Test inutile pour les lignes 2 et 3)
			{
				// Placement d'une deuxième pièce
				if (boardInit[i][j]>3) // Si la première pièce est "forte"
				{
					boardInit[i][j+1]=PlacePiece(0,3,pawnsLeft);
				}
				else if (i<2) // Si la première pièce est "faible" 
				{
					boardInit[i][j+1]=PlacePiece(4,8,pawnsLeft);
				}
				else //Ligne 2 et 3 
					boardInit[i][j+1]=PlacePiece(4,10,pawnsLeft);
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
	else // Maj lorsque l'autre équipe à move sans attack
	{
		EPiece temp;
		SPos pos;
		int flag1=1; // flag pour sortir si on trouve les 2 différences
		int flag2=1;
		while (i<10 && (flag1 || flag2))
		{
			while(j<10 && (flag1 || flag2))
			{
				if(gameStateIA.board[i][j].content!=gameState->board[i][j].content)
				{
					// La case était vide --> on récupère la position
					if (gameStateIA.board[i][j].content==ECnone)
					{
						pos.line=i;
						pos.col=j;
						flag1=0;
					}
					// La case était occupée --> on récupère la pièce qui s'y trouvait et on la vide
					else
					{
						temp=gameState->board[i][j].piece;
						gameStateIA.board[i][j].piece=EPnone;
						gameStateIA.board[i][j].content=ECnone;
						flag2=0;
					}
				}
			}
		}
		if (!(flag1 && flag2)) // On a trouvé une différence, on remplace la pièce dès fois qu'on la connaissait déjà
		{
			gameStateIA.board[pos.line][pos.col].piece=temp;
			gameStateIA.board[pos.line][pos.col].content=colorEnemy;
		}
	}
	
	SMove next;
	
	// Si l'on peut gagner une attaque de manière certaine à ce tour le faire
	// Sinon déplacer une pièce qui le peut au hasard
	
	// Si pas d'attaque maj de notre tableau
	if (gameStateIA.board[next.end.line][next.end.col].content==ECnone)
	{
		gameStateIA.board[next.end.line][next.end.col].content=colorIA;
		gameStateIA.board[next.end.line][next.end.col].piece=gameStateIA.board[next.start.line][next.start.col].piece;
	}
	
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

	// 0 --> faux (on nous attaque) ; 1 --> vrai (on attaque)
    // i.e.: armyPos est la position de départ de notre dernier move.
	int attacking=(last.start.line==armyPos.line && last.start.col==armyPos.col);
	
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
    	if (attacking)
    		result=1;
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
		// Nous sommes les initiateurs de l'attaque
		if (attacking)
		{
			gameStateIA.board[enemyPos.line][enemyPos.col].content=colorIA;
			gameStateIA.board[enemyPos.line][enemyPos.col].piece=armyPiece;
		}
		//On nous a attaqué
		else
		{
			// Suppression de la pièce éliminée
			gameStateIA.board[enemyPos.line][enemyPos.col].content=ECnone;
			gameStateIA.board[enemyPos.line][enemyPos.col].piece=EPnone;
		}
		
		if (colorEnemy==ECred)
			gameStateIA.redOut[enemyPiece]++;
		else
			gameStateIA.blueOut[enemyPiece]++;
		
	}
	else if (result == -1)
	{
		// Nous sommes les initiateurs de l'attaque
		if (attacking)
		{
			// Suppression de la pièce éliminée
			gameStateIA.board[armyPos.line][armyPos.col].content=ECnone;
			gameStateIA.board[armyPos.line][armyPos.col].piece=EPnone;
			// On découvre la valeur d'une pièce ennemie toujours en jeu
			gameStateIA.board[enemyPos.line][enemyPos.col].piece=enemyPiece;
		}
		// On nous a attaqué
		else
		{
			gameStateIA.board[armyPos.line][armyPos.col].content=colorEnemy;
			gameStateIA.board[armyPos.line][armyPos.col].piece=enemyPiece;
		}
		if (colorIA==ECred)
			gameStateIA.redOut[armyPiece]++;
		else
			gameStateIA.blueOut[armyPiece]++;
	}
	else //Les deux pièces disparaissent
	{
		gameStateIA.board[enemyPos.line][enemyPos.col].content=ECnone;
		gameStateIA.board[enemyPos.line][enemyPos.col].piece=EPnone;
		gameStateIA.board[armyPos.line][armyPos.col].content=ECnone;
		gameStateIA.board[armyPos.line][armyPos.col].piece=EPnone;
		if (colorIA==ECred)
		{
			gameStateIA.redOut[armyPiece]++;
			gameStateIA.blueOut[enemyPiece]++;
		}
		else
		{
			gameStateIA.blueOut[armyPiece]++;
			gameStateIA.redOut[enemyPiece]++;
		}
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

/**
 * Placement d'une pièce dans le boardinit
 * @param int i
 * 	ligne i
 * @param int j
 * 	colonne j
 * @param int forcMin
 * 	force minimum de la pièce
 * @param int forceMax
 * 	force maximum de la pièce
 * @param unsigned int pawnsLeft[11]
 * 	tableau des pièces restantes à palcer
 * @param boardInit[4][10]
 *	tableau à remplir
 */
int PlacePiece (int forceMin, int forceMax, unsigned int pawnsLeft[])
{
	int nextPiece;
	do
	{
		nextPiece=PseudoRandom(forceMin,forceMax); // Pièce avec une force entre forceMin et forceMax
	}while(pawnsLeft[nextPiece]==0); // Pièce encore disponible au placement ?

	pawnsLeft[nextPiece]--; // On retire la pièce du tableau de pièces à placer
	return nextPiece;
}





