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

    C->board = SDL_LoadBMP("images/plateau.bmp");
}

void LoadFonts(Context *C)
{
    C->fonts[SMALLTEXT] = TTF_OpenFont("DejaVuSans.ttf", 14);
    C->fonts[MEDIUMTEXT] = TTF_OpenFont("DejaVuSans.ttf", 24);
    C->fonts[BIGTEXT] = TTF_OpenFont("DejaVuSans.ttf", 56);
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

void PlacePiece(Context *C, EColor color, EPiece side[4][10])
{
    int continuer = 1;
    int i, j;
    int x, y;

    Input in;
    memset(&in, 0, sizeof(in));

    SDL_Color blackColor = {0, 0, 0};  // Couleur noire pour le texte

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

        // Affichage du nom du jeu
        C->text = TTF_RenderText_Blended(C->fonts[BIGTEXT], "Stratego", blackColor);
        x = WINDOW_WIDTH - (500/2) - (C->text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 5;
        Blit(C->text, C->screen, x, y);

        // Affichage de la pièce à placer
        C->text = TTF_RenderText_Blended(C->fonts[MEDIUMTEXT], getNamePiece(currentPiece), blackColor);
        x = WINDOW_WIDTH - (500/2) - (C->text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 150;
        Blit(C->text, C->screen, x, y);

        // Affichage de la couleur du joueur
        if (color == ECred)
            C->text = TTF_RenderText_Blended(C->fonts[MEDIUMTEXT], "Joueur Rouge : ", blackColor);
        else
            C->text = TTF_RenderText_Blended(C->fonts[MEDIUMTEXT], "Joueur Bleu : ", blackColor);
        x = WINDOW_WIDTH - (500/2) - (C->text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 100;
        Blit(C->text, C->screen, x, y);


        SDL_Flip(C->screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    SDL_FreeSurface(noMansLand);
}

int movePiece(Context *C, EColor currentPlayer, SGameState *gameState, SMove *movement)
{
    int continuer = 1;
    int i, j;
    int x, y;

    Input in;
    memset(&in, 0, sizeof(in));

    int gameStatus = 0;  // 0 = le joueur doit sélectionner une pièce, 1 = il doit sélectionner une destination

    EColor colorOpponent;

    SDL_Color blackColor = {0, 0, 0};  // Couleur noire pour le texte

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
            fprintf(stderr, "i, j: %d, %d \n", i, j);
            if (i >= 0 && i < 10 && j >= 0 && j < 10)
            {
                if (gameStatus == 0)
                {
                    if (gameState->board[i][j].content == currentPlayer)  // Il a bien cliqué sur une de ses pièces
                    {
                        movement->start.line = i;
                        movement->start.col = j;
                        gameStatus = 1;
                    }
                }
                else
                {
                    if (gameState->board[i][j].content == ECnone || gameState->board[i][j].content == colorOpponent)  // Il a bien cliqué sur une destination possible
                    {
                        movement->end.line = i;
                        movement->end.col = j;
                        continuer = 0;
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

                if (gameState->board[i][j].piece != EPnone)
                {
                    if (gameState->board[i][j].content == ECred)
                        Blit(C->images[IMGRED][gameState->board[i][j].piece], C->screen, x, y);
                    else
                        Blit(C->images[IMGBLUE][gameState->board[i][j].piece], C->screen, x, y);
                }
            }
        }

        // Affichage du nom du jeu
        C->text = TTF_RenderText_Blended(C->fonts[BIGTEXT], "Stratego", blackColor);
        x = WINDOW_WIDTH - (500/2) - (C->text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 5;
        Blit(C->text, C->screen, x, y);

        SDL_Flip(C->screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    if (in.quit)
        return -1;
    else
        return 0;
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