/*

Fonctions utilisées pour la gestoin des libs chargées

*/

#ifndef _LIBS_H
    #define _LIBS_H

    typedef struct AI
    {
        void *lib;  // La lib

        // Pointeurs vers les fonctions d'une IA
        pfInitLibrary InitLibrary;
        pfStartMatch StartMatch;
        pfStartGame StartGame;
        pfEndGame EndGame;
        pfEndMatch EndMatch;
        pfNextMove NextMove;
        pfAttackResult AttackResult;
        pfPenalty Penalty;
    } AI;

    /**
     * Charge une lib d'IA et toutes ses fonctions
     * @param  AI   *ai   Structure stockant la lib et ses fonctions
     * @param  char *file Chemin vers la lib
     * @return            -1 si erreur, 0 sinon
     */
    int LoadLib(AI *ai, char* file);

#endif