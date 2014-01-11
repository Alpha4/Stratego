/*

Fonctions utilisées pour l'arbitrage du jeu

*/

#ifndef _ARBITRE_H
    #define _ARBITRE_H

    /**
     * Vérifie si un coup est valide
     * @param  SGameState gameState Etat du jeu avant le coup
     * @param  Smove       move     Mouvement demandé
     * @return int                  1 si coup valide, 0 sinon
     */
    int isValidMove(SGameState *gameState, SMove move, EColor currentPlayer, SMove histMove[3]);

#endif
