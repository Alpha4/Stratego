/*

Fonctions gérant l'interface, notamment les fonctions gérant les interractions avec un joueur humain et non une IA.
Contient également les constantes nécessaires

*/

#ifndef _INTERFACE_H
    #define _INTERFACE_H

    #define TAILLE_CASE         64 // Taille en pixels d'une case du plateau
    #define NB_BLOCS_COTE       10 // Nombre de cases par coté du plateau
    #define LARGEUR_FENETRE     TAILLE_CASE * NB_BLOCS_COTE + 500  // On laisse 500px à droite du plateau pour de l'affichage
    #define HAUTEUR_FENETRE     TAILLE_CASE * NB_BLOCS_COTE

    /**
     * Demande à un joueur humain de placer ses pièces une par une
     * @param ecran La SDL_Surface de la fenêtre sur laquelle on va afficher
     * @param color La couleur du joueur
     * @param side[4][10] La partie du plateau de jeu sur laquelle le joueur place ses pièces
     */
    void placementPiece(SDL_Surface *ecran, EColor color, EPiece side[4][10]);

    /**
     * Permet de connaitre le nom d'une pièce en fonction de son type, pour l'affichage
     * @param  piece Le type de la pièce
     * @return       Le nom de la pièce
     */
    char* getNomPieceAPlacer(EPiece piece);

    /**
     * Renvoie la pièce suivante que le joueur doit placer sur le plateau lors du placement de ces pièces
     * @param  piece La pièce précédente
     * @return       La pièce qu'il va falloir placer (EPnone si plus de pièce à placer)
     */
    EPiece getNextPieceAPlacer(EPiece piece);

    /**
     * Renvoie le nombre de pièces du type demandé qu'il doit y avoir sur le plateau pour un joueur
     * @param  piece La pièce demandée
     * @return       Le nombre de pièces de ce type
     */
    int getNbPieceAPlacer(EPiece piece);

#endif
