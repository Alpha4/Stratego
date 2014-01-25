/*

Fonctions utilisées pour l'arbitrage du jeu

*/

#ifndef _ARBITRE_H
    #define _ARBITRE_H

    #define MAX_PENALTY 2
    #define NB_GAMES 3

    /**
     * Vérifie si un coup est valide
     * @param  SGameState gameState     Etat du jeu avant le coup
     * @param  Smove      move          Mouvement demandé
     * @param  EColor     currentPlayer Joueur qui a joué le coup
     * @param  SMove      histMove[3]   Historique des coup du joueur
     * @return int                      1 si coup valide, 0 sinon
     */
    int isValidMove(SGameState *gameState, SMove move, EColor currentPlayer, SMove histMove[3]);

    /**
     * Renvoie le résulat d'une attaque
     * @param  EPiece piece1 Pièce qui attaque
     * @param  EPiece piece2 Pièce qui est attaquée
     * @return int           0 si destruction mutuelle, 1 si joueur attaquant gagne, 2 si joueur attaqué gagne
     */
    int Attack(EPiece piece1, EPiece piece2);

    /**
     * Configure le plateau qui doit être envoyé au joueur (invesion plateau, anonymage des pièces adverses)
     * @param SGameState gameState     Etat du jeu vu par l'arbitre
     * @param SGameState result        Etat du jeu modifié envoyé au joueur
     * @param EColor     currentPlayer Joueur qui va jouer le coup
     */
    void manageBoard(SGameState *gameState, SGameState *result, EColor currentPlayer);

    /**
     * Vérifie que l'initialisation des pièces par l'IA est correcte
     * @param  EPiece pSide[4][10] Partie du plateau que l'IA a initialisé
     * @return int                 0 si initialisation incorrecte, 1 si correcte
     */
    int VerifyInitAI(EPiece pSide[4][10]);

    /**
     * Vérifie si un des joueurs a gagné la partie
     * @param  SGameState *gameState Le plateau du jeu
     * @param  int        penalty[2] Le nombre de pénalités de chaque joueur
     * @param  EColor     player1    Le joueur 1
     * @param  EColor     player2    Le joueur 2
     * @param  int        nbMoveLeft Nombre de coups restant
     * @return int                   player1 si player1 gagne, player2 si player2 gagne, 0 si la partie n'est pas terminée, -1 si il y a égalité
     */
    int isGameFinished(SGameState *gameState, int penalty[2], EColor player1, EColor player2, int nbMoveLeft);

    /**
     * Enregistre le résultat d'une manche dans un fichier, de la forme : <winnerName>\t<winnerName>\t<winnerName>\n
     * @param char* winner Le nom du gagnant (0 si égalité)
     * @param int   round  Le numéro du round actuel
     */
    void saveResult(char* winner, int round);
    
    /**
     * Vérifie si au moins un coup est possible pour un joueur
     * @param  SGameState *gameState Le plateau du jeu
     * @param  EColor     player     Couleur du joueur à vérifier
     * @return int                   1 si il y a au moins un coup possible, 0 sinon
     */
    int isOneMovePossible(SGameState *gameState, EColor player);

#endif
