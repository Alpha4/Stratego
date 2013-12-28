/*

Programme principal (main), qui gère l'interface, l'arbitre et fait appel aux lib des IA quand il le faut.

*/

#include "stratego.h"
#include "interface.h"
#include "arbitre.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>

int main(int argc, char *argv[])
{
    // Stratego
    SGameState gameState;

    // SDL
    SDL_Surface *ecran = NULL; // La fenêtre du jeu
    SDL_Event event;
    int continuer = 1;
    int i, j;
    SDL_Surface *plateau = NULL, *bombRED = NULL, *spyRED = NULL, *scoutRED = NULL, *minerRED = NULL, *sergeantRED = NULL, *lieutenantRED = NULL, *captainRED = NULL, *majorRED = NULL, *colonelRED = NULL, *generalRED = NULL, *marshalRED = NULL, *flagRED = NULL, *bombBLUE = NULL, *spyBLUE = NULL, *scoutBLUE = NULL, *minerBLUE = NULL, *sergeantBLUE = NULL, *lieutenantBLUE = NULL, *captainBLUE = NULL, *majorBLUE = NULL, *colonelBLUE = NULL, *generalBLUE = NULL, *marshalBLUE = NULL, *flagBLUE = NULL;
    SDL_Rect position;  // Utilisé pour positionner chaque surface

    // Position de la surface contenant le plateau
    SDL_Rect positionPlateau;
    positionPlateau.x = 0;
    positionPlateau.y = 0;

    SDL_Init(SDL_INIT_VIDEO);  // Init SDL

    // SDL_WM_SetIcon(SDL_LoadBMP("truc.bmp"), NULL);  // Icone du jeu
    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  // Création de la fenêtre du jeu
    SDL_WM_SetCaption("Stratego", NULL);  // Titre de la fenêtre

    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
        }

        // Effacement de l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(plateau, NULL, ecran, &positionPlateau);

        for (i = 0 ; i < NB_BLOCS_COTE ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_COTE ; j++)
            {
                position.x = i * TAILLE_CASE;
                position.y = j * TAILLE_CASE;

                switch(gameState.board[i][j].piece)
                {
                    case EPbomb:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(bombRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(bombBLUE, NULL, ecran, &position);
                        break;
                    case EPspy:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(spyRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(spyBLUE, NULL, ecran, &position);
                        break;
                    case EPscout:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(scoutRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(scoutBLUE, NULL, ecran, &position);
                        break;
                    case EPminer:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(minerRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(minerBLUE, NULL, ecran, &position);
                        break;
                    case EPsergeant:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(sergeantRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(sergeantBLUE, NULL, ecran, &position);
                        break;
                    case EPlieutenant:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(lieutenantRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(lieutenantBLUE, NULL, ecran, &position);
                        break;
                    case EPcaptain:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(captainRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(captainBLUE, NULL, ecran, &position);
                        break;
                    case EPmajor:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(majorRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(majorBLUE, NULL, ecran, &position);
                        break;
                    case EPcolonel:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(colonelRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(colonelBLUE, NULL, ecran, &position);
                        break;
                    case EPgeneral:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(generalRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(generalBLUE, NULL, ecran, &position);
                        break;
                    case EPmarshal:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(marshalRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(marshalBLUE, NULL, ecran, &position);
                        break;
                    case EPflag:
                        if (gameState.board[i][j].content == ECred)
                            SDL_BlitSurface(flagRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(flagBLUE, NULL, ecran, &position);
                        break;
                }
            }
        }

        SDL_Flip(ecran);  // Affichage de l'écran
    }


    // Libération des surfaces
    SDL_FreeSurface(bombRED);
    SDL_FreeSurface(bombBLUE);
    SDL_FreeSurface(spyRED);
    SDL_FreeSurface(spyBLUE);
    SDL_FreeSurface(scoutRED);
    SDL_FreeSurface(scoutBLUE);
    SDL_FreeSurface(minerRED);
    SDL_FreeSurface(minerBLUE);
    SDL_FreeSurface(sergeantRED);
    SDL_FreeSurface(sergeantBLUE);
    SDL_FreeSurface(lieutenantRED);
    SDL_FreeSurface(lieutenantBLUE);
    SDL_FreeSurface(captainRED);
    SDL_FreeSurface(captainBLUE);
    SDL_FreeSurface(majorRED);
    SDL_FreeSurface(majorBLUE);
    SDL_FreeSurface(colonelRED);
    SDL_FreeSurface(colonelBLUE);
    SDL_FreeSurface(generalRED);
    SDL_FreeSurface(generalBLUE);
    SDL_FreeSurface(marshalRED);
    SDL_FreeSurface(marshalBLUE);
    SDL_FreeSurface(flagRED);
    SDL_FreeSurface(flagBLUE);
    SDL_FreeSurface(plateau);

    SDL_Quit();

    return EXIT_SUCCESS;
}