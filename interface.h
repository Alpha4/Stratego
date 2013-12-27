/*

Fonctions gérant l'interface, notamment les fonctions gérant les interractions avec un joueur humain et non une IA.
Contient également les constantes nécessaires

*/

#ifndef _INTERFACE_H
    #define _INTERFACE_H

    #define TAILLE_CASE         34 // Taille d'une case du plateau
    #define NB_BLOCS_COTE       10 // Nombre de cases par coté du plateau
    #define LARGEUR_FENETRE     TAILLE_CASE * NB_BLOCS_COTE
    #define HAUTEUR_FENETRE     TAILLE_CASE * NB_BLOCS_COTE

    /**
     * Affichage du plateau du jeu
     * @param ecran     Pointeur vers la surface de l'écran afin d'afficher des choses dessus
     * @param gameState Etat du jeu à afficher
     */
    void AfficherJeu(SDL_Surface *ecran, SGameState *gameState);

#endif