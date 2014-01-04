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

    // Pour simplifier l'affichage des images
    #define ImgRed 0
    #define ImgBlue 1

    typedef struct Context
    {
        SDL_Surface *screen;  // La fenêtre
        int XRES, YRES;  // Résolution de la fenêtre
        SDL_Surface *plateau;  // Surface du plateau de jeu
        SDL_Surface *texte;
        SDL_Surface *images[2][12];  // Tableau de pointeurs vers les surfaces de chaque image du jeu
    } Context;

    typedef struct Input
    {
        char key[SDLK_LAST];
        int mousex, mousey;
        int mousexrel, mouseyrel;
        char mousebuttons[8];
        char quit;
    } Input;

    /**
     * Permet de gérer les évènements. Mets à jour la structure 'in' en fonction des actions de l'utilisateurs sur le clavier ou la souris
     * @param Input in La structure qui contient l'état du clavier et de la souris
     */
    void UpdateEvents(Input* in);

    /**
     * Initialise l'affichage du jeu.
     * @param  Context *C    Le contexte de l'affichage du jeu
     * @param  int     x     Largeur de la fenêtre
     * @param  int     y     Hauteur de la fenêtre
     * @param  char*   title Titre de la fenêtre
     * @return int           -1 si erreur, 0 sinon
     */
    int Init(Context *C, int x, int y, char* title);

    /**
     * Charge les images dans la structure C
     * @param Context *C Le contexte de l'affichage du jeu
     */
    void LoadImages(Context *C);

    /**
     * Affiche une surface à l'écran
     * @param  SDL_Surface *src Surface à afficher
     * @param  SDL_Surface *dst Surface sur laquelle afficher
     * @param  int         x   Position x
     * @param  int         y   Position y
     * @return int             0 si erreur, -1 sinon
     */
    int Blit(SDL_Surface *src, SDL_Surface *dst, int x, int y);

    /**
     * Libère les surfaces des images
     * @param Context *C Le contexte de l'affichage du jeu
     */
    void FreeImages(Context *C);

    /**
     * Demande à un joueur humain de placer ses pièces une par une
     * @param C Le contexte de l'affichage du jeu
     * @param color La couleur du joueur
     * @param side[4][10] La partie du plateau de jeu sur laquelle le joueur place ses pièces
     */
    void placementPiece(Context *C, EColor color, EPiece side[4][10]);

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
