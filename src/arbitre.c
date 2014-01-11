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

int isValidMove(SGameState *gameState, SMove move, EColor currentPlayer)
{
    // Coordonnées case initiale
    int i0 = move.start.line;
    int j0 = move.start.col;
    // Coordonnées case finale
    int i1 = move.end.line;
    int j1 = move.end.col;
    // Nombre de cases de déplacement en horizontal et en vertical
    int movei = abs(i1 -i0);
    int movej = abs(j1-j0);

    // Lignes et colonnes hors du plateau
    if ((i0 < 0) || (i0 > 9) || (i1 < 0) || (i1 > 9)) return 0;
    if ((j0 < 0) || (j0 > 9) || (j1 < 0) || (j1 > 9)) return 0;

    // La pièce sélectionnée n'est pas celle du joueur courant
    if (gameState->board[i0][j0].content != currentPlayer) return 0;
    // La pièce sélectionnée est une bombe ou le drapeau (pièce fixe)
    if ((gameState->board[i0][j0].piece == 0) || (gameState->board[i0][j0].piece == 11)) return 0;

    // La case d'arrivé est un lac ou est occupée par une pièce du joueur courant
    if ((gameState->board[i0][j0].conent == currentPlayer) || (gameState->board[i0][j0].conent == EClake)) return 0;

    if (movei == movej) return 0; // Déplacement diagonal ou surplace
    if (gameState->board[i0][j0].piece != 2) // La pièce n'est pas un éclaireur
    {
        if ((movei != 0) && (movej != 1)) return 0; // Déplacement vertical >= 2
        if ((movei != 1) && ((movej != 0)) return 0; // Déplacement horizontal >= 2

     }
    else if ((movei != 0) && (movej != 0)) return 0; // La pièce est un éclaireur


    return 1;
}
