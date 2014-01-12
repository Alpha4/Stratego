/*

Fonctions gérant l'interface, notamment les fonctions gérant les interractions avec un joueur humain et non une IA.
Contient également les constantes nécessaires

*/

#ifndef _INTERFACE_H
    #define _INTERFACE_H

    #define SQUARE_SIZE     64  // Taille en pixels d'une case du plateau
    #define SQUARES_BY_SIDE 10  // Nombre de cases par coté du plateau
    #define WINDOW_WIDTH    SQUARE_SIZE * SQUARES_BY_SIDE + 500  // On laisse 500px à droite du plateau pour de l'affichage
    #define WINDOW_HEIGHT   SQUARE_SIZE * SQUARES_BY_SIDE

    // Pour simplifier l'affichage des images
    #define IMGRED  0
    #define IMGBLUE 1

    // Pour simplifier la gestion des différentes tailles de police
    #define SMALLTEXT  0
    #define MEDIUMTEXT 1
    #define BIGTEXT    2

    /**
     * Contient toutes les variables qui servent à l'affichage
     */
    typedef struct Context
    {
        SDL_Surface *screen;  // La fenêtre
        int XRES, YRES;  // Résolution de la fenêtre
        SDL_Surface *board;  // Surface du plateau de jeu
        SDL_Surface *text;
        SDL_Surface *images[2][12];  // Tableau de pointeurs vers les surfaces de chaque image du jeu
        SDL_Surface *imagesMini[12];  // Tableau de pointeurs vers les surfaces de chaque image du jeu en mini
        TTF_Font *fonts[3];  // Tableau contenant les 3 tailles de texte que l'on va utiliser
    } Context;

    /**
     * Permet de gérer les évènements souris et clavier
     */
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
     * Charge les polices dans la structure C
     * @param Context *C Le contexte de l'affichage du jeu
     */
    void LoadFonts(Context *C);

    /**
     * Affiche une surface à l'écran
     * @param  SDL_Surface *src Surface à afficher
     * @param  SDL_Surface *dst Surface sur laquelle afficher
     * @param  int         x    Position x
     * @param  int         y    Position y
     * @return int              0 si erreur, -1 sinon
     */
    int Blit(SDL_Surface *src, SDL_Surface *dst, int x, int y);

    /**
     * Affiche un texte à l'écran
     * @param  SDL_Surface *dst    Surface sur laquelle afficher
     * @param  int         x       Position x
     * @param  int         y       Position y
     * @param  int         centerX 1 si le texte doit être centré sur la coordonnée x
     * @param  int         centerY 1 si le texte doit être centré sur la coordonnée y
     * @param  char*       text    Texte à afficher
     * @param  TTF_Font    *font   Police dans laquelle afficher le texte
     * @param  SDL_Color   color   Couleur du texte
     * @return int                 0 si erreur, -1 sinon
     */
    int blitText(SDL_Surface *dst, int x, int y, int centerX, int centerY, char* text, TTF_Font *font, SDL_Color color);

    /**
     * Libère les ressources utilisées
     * @param Context *C Le contexte de l'affichage du jeu
     */
    void FreeAll(Context *C);

    /**
     * Demande à un joueur humain de placer ses pièces une par une
     * @param  Context *C          Le contexte de l'affichage du jeu
     * @param  Ecolor  color       La couleur du joueur
     * @param  EPiece  side[4][10] La partie du plateau de jeu sur laquelle le joueur place ses pièces
     * @return int                 1 si coordonnées valide, 0 sinon
     */
    int PlacePiece(Context *C, EColor color, EPiece side[4][10]);

    /**
     * Permet à un joueur humain de déplacer une pièce
     * @param  Context    *C            Le contexte de l'affichage du jeu
     * @param  EColor     currentPlayer Le joueur qui doit jouer
     * @param  SGameState *gameState    Le plateau du jeu
     * @param  SMove      *movement     Variable qui va stocker le déplacement choisi par le joueur
     * @param  char        name[50]     Nom du joueur
     * @return int                      -1 si le joueur à quitté le jeu, 0 sinon
     */
    int movePiece(Context *C, EColor currentPlayer, SGameState *gameState, SMove *movement, char name[50]);

    /**
     * Vérifie si les coordonnées demandées sont une destination possible pour la pièce (pour l'affichage)
     * @param  SPos       origin     L'origine du déplacement
     * @param  int        i1          Ligne du déplacement demandé
     * @param  int        j1          Colonne du déplacement demandé
     * @param  SGameState *gameState Le plateau du jeu
     * @param  EColor     currentPlayer Le joueur qui doit jouer
     * @return int                   1 si coordonnées valide, 0 sinon
     */
    int areValidCoords(SPos origin, int i1, int j1, SGameState *gameState, EColor currentPlayer);

    /**
     * Affiche les infos à droite du plateau
     * @param Context    *C             Le contexte de l'affichage du jeu
     * @param SGameState *gameState     L'état du jeu
     * @param EColor     currentPlayer  Le joueur qui doit jouer
     */
    void displayInfo(Context *C, SGameState *gameState, EColor currentPlayer);

    /**
     * Permet de connaitre le nom d'une pièce en fonction de son type, pour l'affichage
     * @param  EPiece piece Le type de la pièce
     * @return              Le nom de la pièce
     */
    char* getNamePiece(EPiece piece);

    /**
     * Renvoie la pièce suivante que le joueur doit placer sur le plateau lors du placement de ces pièces
     * @param  EPiece piece La pièce précédente
     * @return              La pièce qu'il va falloir placer (EPnone si plus de pièce à placer)
     */
    EPiece getNextPiece(EPiece piece);

    /**
     * Renvoie le nombre de pièces du type demandé qu'il doit y avoir sur le plateau pour un joueur
     * @param  EPiece piece La pièce demandée
     * @return              Le nombre de pièces de ce type
     */
    int getNumberOfPiece(EPiece piece);

#endif
