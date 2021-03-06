/*

Fonctions utilisées pour l'arbitrage du jeu

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "stratego.h"
#include "interface.h"
#include "arbitre.h"
#include "libs.h"

int isValidMove(SGameState *gameState, SMove move, EColor currentPlayer, SMove histMove[3])
{
    int k = 0, t;

    // Coordonnées case initiale
    int i0 = move.start.line;
    int j0 = move.start.col;
    // Coordonnées case finale
    int i1 = move.end.line;
    int j1 = move.end.col;
    // Nombre de cases de déplacement en horizontal et en vertical
    int moveI = abs(i1 - i0);
    int moveJ = abs(j1 - j0);

    // Lignes et colonnes hors du plateau
    if ((i0 < 0) || (i0 > 9) || (i1 < 0) || (i1 > 9)) return 0;
    if ((j0 < 0) || (j0 > 9) || (j1 < 0) || (j1 > 9)) return 0;

    // La pièce sélectionnée n'est pas celle du joueur courant
    if (gameState->board[i0][j0].content != currentPlayer) return 0;
    // La pièce sélectionnée est une bombe ou le drapeau (pièce fixe)
    if ((gameState->board[i0][j0].piece == EPbomb) || (gameState->board[i0][j0].piece == EPflag)) return 0;
    // La case d'arrivé est un lac ou est occupée par une pièce du joueur courant
    if ((gameState->board[i1][j1].content == currentPlayer) || (gameState->board[i1][j1].content == EClake)) return 0;

    if (moveI == moveJ) return 0;  // Déplacement diagonal ou surplace
    if (gameState->board[i0][j0].piece != EPscout)  // La pièce n'est pas un éclaireur
    {
        if ((moveI >= 2) || (moveJ >= 2)) return 0;  // Déplacement >= 2
        if ((moveI == 1) && (moveJ != 0)) return 0;
        if ((moveI == 0) && (moveJ != 1)) return 0;
    }
    else  // La pièce est un éclaireur
    {
        if ((moveI != 0) && (moveJ != 0)) return 0;

        /* On va véfifier que l'éclaireur ne saute pas par-dessus une pièce ou un lac */
        if (moveI > 1)  // Déplacement vertical
        {
            for (t=1; t < moveI; t++)
            {
                if (i1 - i0 > 0)  // Déplacement vers le haut
                {
                    if (gameState->board[i0 + t][j0].content != ECnone) return 0;
                }
                else if (gameState->board[i0 - t][j0].content != ECnone) return 0;  // Déplacmenent vers le bas
            }
        }
        else  // Déplacement horizontal
        {
            for (t=1; t < moveJ; t++)
            {
                if (j1 - j0 > 0)  // Déplacemnt vers la droite
                {
                    if (gameState->board[i0][j0 + t].content != ECnone) return 0;
                }
                else if (gameState->board[i0][j0 - t].content != ECnone) return 0;  // Déplacement vers la gauche
            }
        }
    }
    /* Fin de la partie pour l'éclaireur */


    /* Vérification des allers-retours */

    if ((histMove[0].start.line == -1) && (histMove[0].start.col == -1))  // 1er coup du joueur
    {
        histMove[0] = move;
    }
    else
    {
        if (histMove[2].start.line == -1)  // 2eme ou 3eme coup du joueur
        {
            while (histMove[k].start.line != -1) k++;
            histMove[k] = move;
        }
        else  // 4eme coup et +, on vérifie la possibilité des allers-retours
        {
            if ((histMove[0].start.line == histMove[1].end.line) && (histMove[0].start.col == histMove[1].end.col)
            && (histMove[0].start.line == histMove[2].start.line) && (histMove[0].start.col == histMove[2].start.col)
            && (histMove[0].start.line == i1) && (histMove[0].start.col == j1))
            {

                if ((histMove[0].end.line == histMove[1].start.line) && (histMove[0].end.col == histMove[1].start.col)
                && (histMove[0].end.line == histMove[2].end.line) && (histMove[0].end.col == histMove[2].end.col)
                && (histMove[0].end.line == i0) && (histMove[0].end.col == j0))
                {
                    return 0;
                }
            }

            for (k = 0 ; k <= 1 ; k++) histMove[k] = histMove[k+1];
            histMove[2] = move;
        }
    }

    return 1;
}

int Attack(EPiece piece1, EPiece piece2)
{
    if (piece2 == EPflag) return 1;  // Attaque sur le drapeau
    if ((piece1 == EPspy) && (piece2 == EPmarshal)) return 1;  // Attaque de l'espion sur le maréchal
    if (piece2 == EPbomb)  // Si la pièce attaquée est une bombe
    {
        if (piece1 == EPminer) return 1;  // Joueur 1 gagne si Pièce 1 est un démineur
        else return 2;  // Joueur 2 gagne sinon
    }

    if (piece1 < piece2) return 2;  // Pièce 1 plus faible que Pièce 2
    return (piece1 > piece2);  // 1 si pièce 1 plus forte que pièce 2, 0 sinon
}

void manageBoard(SGameState *gameState, SGameState *result, EColor currentPlayer)
{
    int i, j;

    if (currentPlayer == ECred)
    {
        for (i = 0 ; i < SQUARES_BY_SIDE ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                result->board[i][j].content = gameState->board[i][j].content;
                if (currentPlayer != gameState->board[i][j].content)
                {
                    result->board[i][j].piece = EPnone;
                }
                else
                {
                    result->board[i][j].piece = gameState->board[i][j].piece;
                }
            }
        }
    }
    else
    {
        for (i = 0 ; i < SQUARES_BY_SIDE ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                result->board[9 - i][9 - j].content = gameState->board[i][j].content;
                if (currentPlayer != gameState->board[i][j].content)
                {
                    result->board[9 - i][9 - j].piece = EPnone;
                }
                else
                {
                    result->board[9 - i][9 - j].piece = gameState->board[i][j].piece;
                }
            }
        }
    }

    for (i = 0 ; i < 11 ; i++)
    {
        result->redOut[i] = gameState->redOut[i];
        result->blueOut[i] = gameState->blueOut[i];
    }
}

int VerifyInitAI(EPiece pSide[4][10])
{
    int i, j;
    int tabPieces[12] = {0};

    // On compte le nombre de pièces disposées par l'IA
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < 10 ; j++)
        {
            if (pSide[i][j] < 0 || pSide[i][j] > 11) return 0;
            tabPieces[pSide[i][j]]++;
        }
    }

    // On compare le nombre des différentes pièces sur le plateau et leur nombre théorique
    for (j = 0 ; j < 12 ; j++)
    {
        if (tabPieces[j] != getNumberOfPiece(j)) return 0;
    }

    return 1;
}

int isGameFinished(SGameState *gameState, int penalty[2], EColor player1, EColor player2, int nbMoveLeft)
{
    int i, j;
    int f1 = 0, f2 = 0;  // 1 si le flag existe toujours
    int m1 = 0, m2 = 0;  // 1 si le joueur à au moins une pièce qui peut bouger
    int nbPiecesOutRed = 0, nbPiecesOutBlue = 0;
    int mPossible1, mPossible2;

    // On teste d'abord les pénalités
    if (penalty[player1 - 2] > MAX_PENALTY)
        return player2;
    if (penalty[player2 - 2] > MAX_PENALTY)
        return player1;

    if (gameState != NULL)  // On vérifie l'état du plateau pour savoir si quelqu'un a gagné
    {
        // On parcours le plateau pour récupérer les informations dont on a besoin
        for (i = 0 ; i < 10 ; i++)
        {
            for (j = 0 ; j < 10 ; j++)
            {
                if (gameState->board[i][j].piece == EPflag)
                {
                    if (gameState->board[i][j].content == player1)
                        f1 = 1;
                    else
                        f2 = 1;
                }
                if (gameState->board[i][j].piece != EPflag && gameState->board[i][j].piece != EPbomb)
                {
                    if (gameState->board[i][j].content == player1)
                        m1 = 1;
                    else
                        m2 = 1;
                }
            }
        }

        if (f1 == 0)  // Le joueur 1 n'a plus de flag
            return player2;
        if (f2 == 0)  // Le joueur 2 n'a plus de flag
            return player1;

        // On vérifie ensuite l'état de ces informations
        if (m1 == 0 && m2 == 0)
        {
            for (i = 0 ; i < 11 ; i++)
            {
                nbPiecesOutBlue += gameState->blueOut[i];
                nbPiecesOutRed += gameState->redOut[i];
            }

            if (nbPiecesOutRed < nbPiecesOutBlue)
                return ECred;
            else if (nbPiecesOutRed > nbPiecesOutBlue)
                return ECblue;
            else
                return -1;  // Egalité
        }

        if (nbMoveLeft == 0)
        {
            for (i = 0 ; i < 11 ; i++)
            {
                nbPiecesOutBlue += gameState->blueOut[i];
                nbPiecesOutRed += gameState->redOut[i];
            }

            if (nbPiecesOutRed < nbPiecesOutBlue)
                return ECred;
            else if (nbPiecesOutRed > nbPiecesOutBlue)
                return ECblue;
            else
                return -1;  // Egalité
        }
        
        // On vérifie que les joueurs peuvent faire un mouvement
        mPossible1 = isOneMovePossible(gameState, player1);
        mPossible2 = isOneMovePossible(gameState, player2);
        if (mPossible1 == 0)
        {
            if (mPossible2 == 1)
                return player2;
            else  // Aucun joueur ne peut bouger donc on détermine le vainqueur par rapport au nombre de pièces
            {
                for (i = 0 ; i < 11 ; i++)
                {
                    nbPiecesOutBlue += gameState->blueOut[i];
                    nbPiecesOutRed += gameState->redOut[i];
                }

                if (nbPiecesOutRed < nbPiecesOutBlue)
                    return ECred;
                else if (nbPiecesOutRed > nbPiecesOutBlue)
                    return ECblue;
                else
                    return -1;  // Egalité
            }
        }
        if (mPossible1 == 1 && mPossible2 == 0)
            return player1;
    }

    return 0;
}

void saveResult(char* winner, int round)
{
    FILE *file = NULL;  // Pointeur vers le fichier

    file = fopen("result.txt", "a");  // Ouverture du fichier en mode "ajout" (on ajoute du contenu à la fin du fichier)

    if (file != NULL)  // Le fichier s'est bien ouvert
    {
        if (round == NB_GAMES)  // On est au dernier round, on termine la ligne
            fprintf(file, "%s\n", winner);  // On écrit dans le fichier
        else  // On n'est pas au dernier round, on continue sur la même ligne
            fprintf(file, "%s\t", winner);  // On écrit dans le fichier
        fclose(file);  // On ferme le fichier
    }

}

int isOneMovePossible(SGameState *gameState, EColor player)
{
    int i, j;
	EColor player2;
	
	if (player == ECblue) 
		player2 = ECred;
	else 
		player2 = ECblue;

    for (i = 1 ; i <= 8 ; i++)
    {
        for (j = 1 ; j <= 8 ; j++)  // On vérifie les cases de [1,1] à [8,8]
        {
            if (gameState->board[i][j].content == player && gameState->board[i][j].piece != EPbomb && gameState->board[i][j].piece != EPflag)  // Il y a une pièce du joueur sur la case, qui est ni une bombe, ni le drapeau
            {
                if (gameState->board[i+1][j].content == ECnone || gameState->board[i-1][j].content == ECnone || gameState->board[i][j+1].content == ECnone || gameState->board[i][j-1].content == ECnone)
                    return 1;
                if (gameState->board[i+1][j].content == ECnone || gameState->board[i-1][j].content == ECnone || gameState->board[i][j+1].content == ECnone || gameState->board[i][j-1].content == ECnone)
                    return 1;
		
                if (gameState->board[i+1][j].content == player2 || gameState->board[i-1][j].content == player2 || gameState->board[i][j+1].content == player2 || gameState->board[i][j-1].content == player2)
                    return 1;
                if (gameState->board[i+1][j].content == player2 || gameState->board[i-1][j].content == player2 || gameState->board[i][j+1].content == player2 || gameState->board[i][j-1].content == player2)
                    return 1;
            }
        }
	
        if (gameState->board[i][0].content == player && gameState->board[i][0].piece != EPbomb && gameState->board[i][0].piece != EPflag)  // On vérifie la colonne de gauche sans ses extrémités : [1,0], [2,0]...[8,0]
        {
            if (gameState->board[i+1][0].content == ECnone || gameState->board[i-1][0].content == ECnone || gameState->board[i][1].content == ECnone)
                return 1;
	    if (gameState->board[i+1][0].content == player2 || gameState->board[i-1][0].content == player2 || gameState->board[i][1].content == player2)
                return 1;
        }
	
        if (gameState->board[i][9].content == player && gameState->board[i][9].piece != EPbomb && gameState->board[i][9].piece != EPflag)  // On vérifie la colonne de droite sans ses extrémités : [1,9], [2,9]...[8,9]
        {
            if (gameState->board[i+1][9].content == ECnone || gameState->board[i-1][9].content == ECnone || gameState->board[i][8].content == ECnone)
                return 1;
	    if (gameState->board[i+1][9].content == player2 || gameState->board[i-1][9].content == player2 || gameState->board[i][8].content == player2)
                return 1;
        }
    }

    for (j = 1 ; j <= 8 ; j++)
    {
        if (gameState->board[0][j].content == player && gameState->board[0][j].piece != EPbomb && gameState->board[0][j].piece != EPflag)  // On vérifie la ligne du bas sans ses extrémités : [0,1], [0,2]...[0,8]
        {
            if (gameState->board[0][j+1].content == ECnone || gameState->board[0][j-1].content == ECnone || gameState->board[1][j].content == ECnone)
                return 1;
	    if (gameState->board[0][j+1].content == player2 || gameState->board[0][j-1].content == player2 || gameState->board[1][j].content == player2)
                return 1;
        }
	
        if (gameState->board[9][j].content == player && gameState->board[9][j].piece != EPbomb && gameState->board[9][j].piece != EPflag)  // On vérifie la ligne du haut sans ses extrémités : [9,1], [9,2]...[9,8]
        {
            if (gameState->board[9][j+1].content == ECnone || gameState->board[9][j-1].content == ECnone || gameState->board[8][j].content == ECnone)
                return 1;
	    if (gameState->board[9][j+1].content == player2 || gameState->board[9][j-1].content == player2 || gameState->board[8][j].content == player2)
                return 1;
        }
    }

    if (gameState->board[0][0].content == player && gameState->board[0][0].piece != EPbomb && gameState->board[0][0].piece != EPflag)  // On vérifie la case [0][0]
    {
        if (gameState->board[1][0].content == ECnone || gameState->board[0][1].content == ECnone) return 1;
	if (gameState->board[1][0].content == player2 || gameState->board[0][1].content == player2) return 1;
    }

    if (gameState->board[0][9].content == player && gameState->board[0][9].piece != EPbomb && gameState->board[0][9].piece != EPflag)  // On vérifie la case [0][9]
    {
        if (gameState->board[1][9].content == ECnone || gameState->board[0][8].content == ECnone) return 1;
        if (gameState->board[1][9].content == player2 || gameState->board[0][8].content == player2) return 1;
    }

    if (gameState->board[9][0].content == player && gameState->board[9][0].piece != EPbomb && gameState->board[9][0].piece != EPflag)  // On vérifie la case [9][0]
    {
        if (gameState->board[8][0].content == ECnone || gameState->board[9][1].content == ECnone) return 1;
        if (gameState->board[8][0].content == player2 || gameState->board[9][1].content == player2) return 1;
    }

    if (gameState->board[9][9].content == player && gameState->board[9][9].piece != EPbomb && gameState->board[9][9].piece != EPflag)  // On vérifie la case [9][9]
    {
        if (gameState->board[9][8].content == ECnone || gameState->board[8][9].content == ECnone) return 1;
        if (gameState->board[9][8].content == player2 || gameState->board[8][9].content == player2) return 1;
    }

    return 0;
}
