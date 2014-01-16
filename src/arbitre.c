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
    int bomb, spy, scout, miner, sergeant, lieutenant, captain, major, colonel, general, marshal, flag;
    bomb = spy = scout = miner = sergeant = lieutenant = captain = major = colonel = general = marshal = flag = 0;

    // On compte le nombre de pièces disposées par l'IA
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < 10 ; j++)
        {
            switch(pSide[i][j])
            {
                case EPbomb:
                    bomb++;
                    break;
                case EPspy:
                    spy++;
                    break;
                case EPscout:
                    scout++;
                    break;
                case EPminer:
                    miner++;
                    break;
                case EPsergeant:
                    sergeant++;
                    break;
                case EPlieutenant:
                    lieutenant++;
                    break;
                case EPcaptain:
                    captain++;
                    break;
                case EPmajor:
                    major++;
                    break;
                case EPcolonel:
                    colonel++;
                    break;
                case EPgeneral:
                    general++;
                    break;
                case EPmarshal:
                    marshal++;
                    break;
                case EPflag:
                    flag++;
                    break;
                default:
                    return 0;  // Si case vide ou pièce inconnue
            }
        }
    }

    // Si le nombre de pièce ne correspond pas au nombre théorique, on renvoie 0
    if ((bomb != 6) || (spy != 1) || (scout != 8) || (miner != 5)) return 0;
    if ((sergeant != 4) || (lieutenant != 4) || (captain != 4) || (major != 3)) return 0;
    if ((colonel != 2) || (general != 1) || (marshal != 1) || (flag != 1)) return 0;

    return 1;
}
