/*

Fonctions gérant l'interface, notamment les fonctions gérant les interractions avec un joueur humain et non une IA.

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


void UpdateEvents(Input *in)
{
    SDL_Event event;

    in->mousebuttons[SDL_BUTTON_WHEELUP] = 0;
    in->mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                in->key[event.key.keysym.sym] = 1;
                break;
            case SDL_KEYUP:
                in->key[event.key.keysym.sym] = 0;
                break;
            case SDL_MOUSEMOTION:
                in->mousex=event.motion.x;
                in->mousey=event.motion.y;
                in->mousexrel=event.motion.xrel;
                in->mouseyrel=event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                in->mousebuttons[event.button.button] = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button != SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
                    in->mousebuttons[event.button.button] = 0;
                break;
            case SDL_QUIT:
                in->quit = 1;
                break;
            default:
                break;
        }
    }
}

int Init(Context *C, int x, int y, char* title)
{
    if (C == NULL)
        return -1;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());  // Écriture de l'erreur
        return -1;
    }

    if (TTF_Init() == -1)  // Démarragge de SDL_ttf pour pouvoir afficher du texte
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        return -1;
    }

    C->XRES = x;
    C->YRES = y;
    C->screen = SDL_SetVideoMode(C->XRES, C->YRES, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  // Création de la fenêtre du jeu
    if (C->screen == NULL)
    {
        fprintf(stderr, "Erreur lors de la création de la fenêtre");
        return -1;
    }

    SDL_WM_SetCaption(title, NULL);  // Titre de la fenêtre

    LoadImages(C);  // Chargement des images

    LoadFonts(C);

    return 0;  // L'initialisation s'est bien passée
}

void LoadImages(Context *C)
{
    C->images[IMGRED][EPbomb] = IMG_Load("images/bombRED.png");
    C->images[IMGBLUE][EPbomb] = IMG_Load("images/bombBLUE.png");
    C->images[IMGRED][EPspy] = IMG_Load("images/spyRED.png");
    C->images[IMGBLUE][EPspy] = IMG_Load("images/spyBLUE.png");
    C->images[IMGRED][EPscout] = IMG_Load("images/scoutRED.png");
    C->images[IMGBLUE][EPscout] = IMG_Load("images/scoutBLUE.png");
    C->images[IMGRED][EPminer] = IMG_Load("images/minerRED.png");
    C->images[IMGBLUE][EPminer] = IMG_Load("images/minerBLUE.png");
    C->images[IMGRED][EPsergeant] = IMG_Load("images/sergeantRED.png");
    C->images[IMGBLUE][EPsergeant] = IMG_Load("images/sergeantBLUE.png");
    C->images[IMGRED][EPlieutenant] = IMG_Load("images/lieutenantRED.png");
    C->images[IMGBLUE][EPlieutenant] = IMG_Load("images/lieutenantBLUE.png");
    C->images[IMGRED][EPcaptain] = IMG_Load("images/captainRED.png");
    C->images[IMGBLUE][EPcaptain] = IMG_Load("images/captainBLUE.png");
    C->images[IMGRED][EPmajor] = IMG_Load("images/majorRED.png");
    C->images[IMGBLUE][EPmajor] = IMG_Load("images/majorBLUE.png");
    C->images[IMGRED][EPcolonel] = IMG_Load("images/colonelRED.png");
    C->images[IMGBLUE][EPcolonel] = IMG_Load("images/colonelBLUE.png");
    C->images[IMGRED][EPgeneral] = IMG_Load("images/generalRED.png");
    C->images[IMGBLUE][EPgeneral] = IMG_Load("images/generalBLUE.png");
    C->images[IMGRED][EPmarshal] = IMG_Load("images/marshalRED.png");
    C->images[IMGBLUE][EPmarshal] = IMG_Load("images/marshalBLUE.png");
    C->images[IMGRED][EPflag] = IMG_Load("images/flagRED.png");
    C->images[IMGBLUE][EPflag] = IMG_Load("images/flagBLUE.png");

    C->imagesMini[EPbomb] = IMG_Load("images/mini/bomb.png");
    C->imagesMini[EPspy] = IMG_Load("images/mini/spy.png");
    C->imagesMini[EPscout] = IMG_Load("images/mini/scout.png");
    C->imagesMini[EPminer] = IMG_Load("images/mini/miner.png");
    C->imagesMini[EPsergeant] = IMG_Load("images/mini/sergeant.png");
    C->imagesMini[EPlieutenant] = IMG_Load("images/mini/lieutenant.png");
    C->imagesMini[EPcaptain] = IMG_Load("images/mini/captain.png");
    C->imagesMini[EPmajor] = IMG_Load("images/mini/major.png");
    C->imagesMini[EPcolonel] = IMG_Load("images/mini/colonel.png");
    C->imagesMini[EPgeneral] = IMG_Load("images/mini/general.png");
    C->imagesMini[EPmarshal] = IMG_Load("images/mini/marshal.png");
    C->imagesMini[EPflag] = IMG_Load("images/mini/flag.png");

    C->penalty[IMGBLUE][0] = IMG_Load("images/penaltyBLUE0.png");
    C->penalty[IMGBLUE][1] = IMG_Load("images/penaltyBLUE1.png");
    C->penalty[IMGBLUE][2] = IMG_Load("images/penaltyBLUE2.png");
    C->penalty[IMGBLUE][3] = IMG_Load("images/penalty3.png");
    C->penalty[IMGRED][0] = IMG_Load("images/penaltyRED0.png");
    C->penalty[IMGRED][1] = IMG_Load("images/penaltyRED1.png");
    C->penalty[IMGRED][2] = IMG_Load("images/penaltyRED2.png");
    C->penalty[IMGRED][3] = IMG_Load("images/penalty3.png");

    C->mystery = IMG_Load("images/mystery.png");

    C->board = IMG_Load("images/plateau.jpg");
    C->outTable = IMG_Load("images/tableau.png");
}

void LoadFonts(Context *C)
{
    C->fonts[SMALLTEXT] = TTF_OpenFont("OCRAStd.ttf", 20);
    C->fonts[MEDIUMTEXT] = TTF_OpenFont("OCRAStd.ttf", 24);
    C->fonts[BIGTEXT] = TTF_OpenFont("OCRAStd.ttf", 56);
}

void FreeAll(Context *C)
{
    int i, j;

    for (i = 0 ; i < 2 ; i++)
    {
        for (j = 0 ; j < 12 ; j++)
        {
            SDL_FreeSurface(C->images[i][j]);
        }
    }

    for (i = 0 ; i < 11 ; i++)
    {
        SDL_FreeSurface(C->imagesMini[i]);
    }

    SDL_FreeSurface(C->mystery);

    for (i = 0 ; i < 3 ; i++)
        TTF_CloseFont(C->fonts[i]);

    SDL_Quit();
    TTF_Quit();
}

int Blit(SDL_Surface *src, SDL_Surface *dst, int x, int y)
{
    SDL_Rect R;
    R.x = x;
    R.y = y;
    return SDL_BlitSurface(src, NULL, dst, &R);
}

int blitText(SDL_Surface *dst, int x, int y, int centerX, int centerY, char* text, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *T;

    T = TTF_RenderUTF8_Blended(font, text, color);

    // On centre le texte si demandé
    if (centerX)
        x = x - T->w/2;
    if (centerY)
        y = y - T->h/2;

    if (Blit(T, dst, x, y))
    {
        SDL_FreeSurface(T);
        return 0;
    }
    else
    {
        SDL_FreeSurface(T);
        return -1;
    }
}

int PlacePiece(Context *C, EColor color, EPiece side[4][10], int penalty[2])
{
    int continuer = 1;
    int i, j;
    int x, y;

    Input in;
    memset(&in, 0, sizeof(in));

    EPiece currentPiece;  // Prochaine pièce que le joueur doit placer
    int nbCurrentPieceLeft;  // Nombre de currentPiece qu'il reste à placer

    // Surface qui noircit la partie du plateau où l'on ne peut rien placer
    SDL_Surface *noMansLand;
    noMansLand = SDL_CreateRGBSurface(SDL_HWSURFACE, SQUARE_SIZE * SQUARES_BY_SIDE, SQUARE_SIZE * 6, 32, 0, 0, 0, 0);
    SDL_FillRect(noMansLand, NULL, SDL_MapRGB(C->screen->format, 0, 0, 0));
    SDL_SetAlpha(noMansLand, SDL_SRCALPHA, 192);

    // Première pièce à placer
    currentPiece = EPbomb;
    nbCurrentPieceLeft = getNumberOfPiece(currentPiece);

    // Initialisation du plateau
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < 10 ; j++)
            side[i][j] = EPnone;
    }

    while (continuer && !in.quit)
    {
        UpdateEvents(&in);
        if (in.mousebuttons[SDL_BUTTON_LEFT])
        {
            // On vérifie que le clic est bien dans une case disponible
            i = (int) ((WINDOW_HEIGHT - in.mousey) / SQUARE_SIZE);
            j = (int) (in.mousex / SQUARE_SIZE);

            if (i >= 0 && i < 4 && j >= 0 && j < 10)
            {
                if (side[i][j] == EPnone)  // Si la case demandée est libre
                {
                    // On place la pièce et on passe à la suivante
                    side[i][j] = currentPiece;
                    nbCurrentPieceLeft--;

                    if (nbCurrentPieceLeft == 0)  // Si on a placé toutes les pièces de ce type
                    {
                        // On passe au type suivant
                        currentPiece = getNextPiece(currentPiece);
                        if (currentPiece != EPnone)
                            nbCurrentPieceLeft = getNumberOfPiece(currentPiece);
                        else
                            continuer = 0;
                    }
                }
            }
        }

        // Effacement de l'écran
        SDL_FillRect(C->screen, NULL, SDL_MapRGB(C->screen->format, 255, 255, 255));

        // Affichage du plateau
        Blit(C->board, C->screen, 0, 0);

        // On noircit le plateau à l'endroit où le joueur ne peut pas placer de pièces
        Blit(noMansLand, C->screen, 0, 0);

        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < 10 ; j++)
            {
                x = j * SQUARE_SIZE;
                y = WINDOW_HEIGHT - (i + 1) * SQUARE_SIZE;

                if (side[i][j] != EPnone)
                {
                    if (color == ECred)
                        Blit(C->images[IMGRED][side[i][j]], C->screen, x, y);
                    else
                        Blit(C->images[IMGBLUE][side[i][j]], C->screen, x, y);
                }
            }
        }

        DisplayInfo(C, NULL, color, penalty, 0);

        // Affichage de la pièce à placer
        x = WINDOW_WIDTH - (500/2);
        y = 160;
        blitText(C->screen, x, y, 1, 0, getNamePiece(currentPiece), C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        // Affichage de la couleur du joueur
        x = WINDOW_WIDTH - (500/2);  // On centre le texte dans la surface à droite du plateau
        y = 120;
        if (color == ECred)
            blitText(C->screen, x, y, 1, 0, "Joueur Rouge : ", C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});
        else
            blitText(C->screen, x, y, 1, 0, "Joueur Bleu : ", C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        SDL_Flip(C->screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    SDL_FreeSurface(noMansLand);

    if (in.quit)
        return -1;
    else
        return 0;
}

int movePiece(Context *C, EColor currentPlayer, SGameState *gameState, SMove *movement, char name[50], int penalty[2], int nbMoveLeft)
{
    int continuer = 1;
    int i, j;
    int x, y;

    Input in;
    memset(&in, 0, sizeof(in));

    int gameStatus = 0;  // 0 = le joueur doit sélectionner une pièce, 1 = il doit sélectionner une destination

    EColor colorOpponent;

    char text[70];  // Stocke du texte à afficher

    // Surface qui noircit les cases du plateau où la pièce sélectionnée ne peut se déplacer
    SDL_Surface *noMansLand;
    noMansLand = SDL_CreateRGBSurface(SDL_HWSURFACE, SQUARE_SIZE, SQUARE_SIZE, 32, 0, 0, 0, 0);
    SDL_FillRect(noMansLand, NULL, SDL_MapRGB(C->screen->format, 0, 0, 0));
    SDL_SetAlpha(noMansLand, SDL_SRCALPHA, 192);

    if (currentPlayer == ECred)
        colorOpponent = ECblue;
    else
        colorOpponent = ECred;

    while (continuer && !in.quit)
    {
        UpdateEvents(&in);
        if (in.mousebuttons[SDL_BUTTON_LEFT])
        {
            in.mousebuttons[SDL_BUTTON_LEFT] = 0;

            // On vérifie que le clic est bien dans une case disponible
            i = (int) (in.mousey / SQUARE_SIZE);
            j = (int) (in.mousex / SQUARE_SIZE);

            if (i >= 0 && i < 10 && j >= 0 && j < 10)
            {
                if (gameStatus == 0)
                {
                    if (gameState->board[i][j].content == currentPlayer && gameState->board[i][j].piece != EPbomb && gameState->board[i][j].piece != EPflag)  // Il a bien cliqué sur une de ses pièces, autre que le drapeau ou une bomb
                    {
                        movement->start.line = i;
                        movement->start.col = j;
                        gameStatus = 1;
                    }
                }
                else
                {
                    if ((gameState->board[i][j].content == ECnone || gameState->board[i][j].content == colorOpponent) && areValidCoords(movement->start, i, j, gameState, currentPlayer))  // Il a bien cliqué sur une destination possible
                    {
                        movement->end.line = i;
                        movement->end.col = j;
                        continuer = 0;
                    }
                    else if (movement->start.line == i && movement->start.col == j)  // Il a recliqué sur la même pièce, on annule la sélection d'une pièce
                    {
                        gameStatus = 0;
                    }
                }
            }
        }

        // Effacement de l'écran
        SDL_FillRect(C->screen, NULL, SDL_MapRGB(C->screen->format, 255, 255, 255));

        // Affichage du plateau
        Blit(C->board, C->screen, 0, 0);

        for (i = 0 ; i < SQUARES_BY_SIDE ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                x = j * SQUARE_SIZE;
                y = i * SQUARE_SIZE;

                if (gameState->board[i][j].content != ECnone && gameState->board[i][j].content != EClake)
                {
                    if (gameState->board[i][j].content == currentPlayer)
                        Blit(C->images[currentPlayer - 2][gameState->board[i][j].piece], C->screen, x, y);
                    else
                        Blit(C->mystery, C->screen, x, y);
                }

                if (gameStatus == 1)
                {
                    if (!areValidCoords(movement->start, i, j, gameState, currentPlayer) && (movement->start.line != i || movement->start.col != j))
                    {
                        // On noircit la case à l'endroit où le joueur ne peut pas placer de pièces
                        Blit(noMansLand, C->screen, x, y);
                    }
                }
            }
        }

        DisplayInfo(C, gameState, currentPlayer, penalty, nbMoveLeft);

        // Affichage du nom du joueur qui doit jouer
        sprintf(&text, "%s, à vous de jouer !", name);
        blitText(C->screen, WINDOW_WIDTH - (500/2), 150, 1, 0, text, C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        SDL_Flip(C->screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    SDL_FreeSurface(noMansLand);

    if (in.quit)
        return -1;
    else
        return 0;
}

int areValidCoords(SPos origin, int i1, int j1, SGameState *gameState, EColor currentPlayer)
{
     int t;

    // Coordonnées case initiale
    int i0 = origin.line;
    int j0 = origin.col;
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

    return 1;
}

void DisplayInfo(Context *C, SGameState *gameState, EColor currentPlayer, int penalty[2], int nbMoveLeft)
{
    int i, x = SQUARE_SIZE * SQUARES_BY_SIDE + 78, y = 270;
    char out[3];

    // Affichage du nom du jeu
    // blitText(C->screen, WINDOW_WIDTH - (500/2), 5, 1, 0, "Stratego", C->fonts[BIGTEXT], (SDL_Color) {0, 0, 0});

    if (gameState != NULL)
    {
        blitText(C->screen, SQUARE_SIZE * SQUARES_BY_SIDE + 20, 220, 0, 0, "Pièces éliminées :", C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        SDL_Surface *underline;
        underline = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 2, 32, 0, 0, 0, 0);
        SDL_FillRect(underline, NULL, SDL_MapRGB(C->screen->format, 0, 0, 0));
        Blit(underline, C->screen, 660, 250);
        SDL_FreeSurface(underline);

        Blit(C->outTable, C->screen, 710, 268);

        for (i = 0 ; i < 11 ; i++)
        {
            Blit(C->imagesMini[i], C->screen, x, y);
            sprintf(&out, "%d", gameState->redOut[i]);
            blitText(C->screen, x + 51, y + 15, 0, 1, out, C->fonts[SMALLTEXT], (SDL_Color) {255, 0, 0});
            sprintf(&out, "%d", gameState->blueOut[i]);
            blitText(C->screen, x + 92, y + 15, 0, 1, out, C->fonts[SMALLTEXT], (SDL_Color) {0, 0, 255});

            y += 32;
        }

        Blit(C->penalty[IMGRED][penalty[ECred - 2]], C->screen, 900, 400);
        Blit(C->penalty[IMGBLUE][penalty[ECblue - 2]], C->screen, 900, 460);

        sprintf(&out, "%d", nbMoveLeft);
        blitText(C->screen, SQUARE_SIZE * SQUARES_BY_SIDE + 420, 560, 1, 1, out, C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});
    }
}

void DisplayEnd(Context *C, char winner[50])
{
    Input in;
    memset(&in, 0, sizeof(in));

    char text[70];  // Stocke du texte à afficher

    SDL_Surface *frame1, *frame2;
    frame1 = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 32, 0, 0, 0, 0);
    SDL_FillRect(frame1, NULL, SDL_MapRGB(C->screen->format, 0, 0, 0));
    frame2 = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOW_WIDTH / 2 - 4, WINDOW_HEIGHT / 2 - 4, 32, 0, 0, 0, 0);
    SDL_FillRect(frame2, NULL, SDL_MapRGB(C->screen->format, 240, 240, 240));

    while (!in.key[SDLK_RETURN])
    {
        UpdateEvents(&in);

        Blit(frame1, C->screen, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4);
        Blit(frame2, C->screen, WINDOW_WIDTH / 4 + 2, WINDOW_HEIGHT / 4 + 2);

        if (winner != NULL)
        {
            // Affichage du nom du joueur qui doit jouer
            sprintf(&text, "%s a gagné !", winner);
            blitText(C->screen, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 1, text, C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});
        }
        else
            blitText(C->screen, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 1, "Egalité !", C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        blitText(C->screen, WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT / 4 - 20, 1, 1, "Appuyez sur Entrée pour continuer", C->fonts[MEDIUMTEXT], (SDL_Color) {0, 0, 0});

        SDL_Flip(C->screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    SDL_FreeSurface(frame1);
    SDL_FreeSurface(frame2);
}

char* getNamePiece(EPiece piece)
{
    switch(piece)
    {
        case EPbomb:
            return "Placez une bombe";
            break;
        case EPspy:
            return "Placez un espion";
            break;
        case EPscout:
            return "Placez un éclaireur";
            break;
        case EPminer:
            return "Placez un démineur";
            break;
        case EPsergeant:
            return "Placez un sergent";
            break;
        case EPlieutenant:
            return "Placez un lieutenant";
            break;
        case EPcaptain:
            return "Placez un capitaine";
            break;
        case EPmajor:
            return "Placez un commandant";
            break;
        case EPcolonel:
            return "Placez un colonel";
            break;
        case EPgeneral:
            return "Placez le général";
            break;
        case EPmarshal:
            return "Placez le maréchal";
            break;
        case EPflag:
            return "Placez le drapeau";
            break;
    }
    return "Voila !";
}

EPiece getNextPiece(EPiece piece)
{
    switch(piece)
    {
        case EPbomb:
            return EPspy;
            break;
        case EPspy:
            return EPscout;
            break;
        case EPscout:
            return EPminer;
            break;
        case EPminer:
            return EPsergeant;
            break;
        case EPsergeant:
            return EPlieutenant;
            break;
        case EPlieutenant:
            return EPcaptain;
            break;
        case EPcaptain:
            return EPmajor;
            break;
        case EPmajor:
            return EPcolonel;
            break;
        case EPcolonel:
            return EPgeneral;
            break;
        case EPgeneral:
            return EPmarshal;
            break;
        case EPmarshal:
            return EPflag;
            break;
        case EPflag:
            return EPnone;
            break;
    }
    return EPnone;
}

int getNumberOfPiece(EPiece piece)
{
    switch(piece)
    {
        case EPbomb:
            return 6;
            break;
        case EPspy:
            return 1;
            break;
        case EPscout:
            return 8;
            break;
        case EPminer:
            return 5;
            break;
        case EPsergeant:
            return 4;
            break;
        case EPlieutenant:
            return 4;
            break;
        case EPcaptain:
            return 4;
            break;
        case EPmajor:
            return 3;
            break;
        case EPcolonel:
            return 2;
            break;
        case EPgeneral:
            return 1;
            break;
        case EPmarshal:
            return 1;
            break;
        case EPflag:
            return 1;
            break;
    }
    return 0;
}
