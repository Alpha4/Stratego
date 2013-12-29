/*

Fonctions utilisées pour l'arbitrage du jeu

*/

#ifndef _ARBITRE_H
    #define _ARBITRE_H

    /**
     * Vérifie si un coup est valide
     * @param  gameState Etat du jeu avant le coup
     * @param  move      Mouvement demandé
     * @return int       1 si coup valide, 0 sinon
     */
    int coupValide(SGameState *gameState, SMove move);

#endif