/*

Programme principal (main), qui gère l'interface, l'arbitre et fait appel aux lib des IA quand il le faut.

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

int main(int argc, char *argv[])
{
    /**
     * argv[1] = Nombre de joueurs artificiels
     * argv[2] = Chemin vers la lib de l'IA 1 (si nécessaire)
     * argv[3] = Chemin vers la lib de l'IA 2 (si nécessaire)
     */


    /**
     * Variables concernant le jeu de Stratego
     */

    SGameState gameState;  // Le jeu (plateau + pièces éliminées)

    int nbHumanPlayers;

    EColor player1 = ECred, player2 = ECblue;

    EColor currentPlayer = ECred;  // Joueur qui est en train de jouer (ECred ou ECblue)
    char p1Name[50] = "Joueur 1", p2Name[50] = "Joueur 2";  // Noms des joueurs
    EPiece redSide[4][10], blueSide[4][10];  // Tableau de placement initial des pièce par les joueurs

    SMove movement;


    /**
     * Variables concernant la gestion de la SDL
     */

    Context C;

    Input in;
    memset(&in, 0, sizeof(in));

    int i, j;
    int x, y;

    SDL_Color blackColor = {0, 0, 0};  // Couleur noire pour le texte

    /**
     * Variables concernant la gestion des lib
     */

    AI ai1, ai2;

    /**
     * Chargement des lib si besoin
     */

    // Vérification qu'il y a au moins en paramètre le nombre de joueurs humains
    if (argc < 2)
    {
        fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
        return EXIT_FAILURE;
    }

    nbHumanPlayers = 2 - atoi(argv[1]);  // On stocke le nombre de joueurs humains

    if (nbHumanPlayers == 1) // 1 joueur humain
    {
        if (argc < 3) // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        if (LoadLib(&ai1, argv[2]) == -1)
            return EXIT_FAILURE;
    }

    if (nbHumanPlayers == 0) // 0 joueur humain
    {
        if (argc < 4) // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        if (LoadLib(&ai2, argv[3]) == -1)
            return EXIT_FAILURE;
    }


    /**
     * Initialisation de l'affichage
     */
    if (Init(&C, WINDOW_WIDTH, WINDOW_HEIGHT, "Stratego") == -1)
        return EXIT_FAILURE;


    /**
     * Initialisation de la partie
     */

    if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
    {
        ai1.InitLibrary(p1Name);  // On lui demande son nom
        ai1.StartMatch();  // On l'initialise

        if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
        {
            ai2.InitLibrary(p2Name);  // On lui demande son nom
            ai2.StartMatch();  // On l'initialise
        }
    }

    /**
     * Placement des pièces par les joueurs
     * TODO : random red/blue
     */

    if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
    {
        ai1.StartGame(ECred, redSide);  // On lui demande de placer ses pièces

        if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
        {
            ai2.StartGame(ECblue, blueSide);  // On lui demande de placer ses pièces
        }
        else
        {
            PlacePiece(&C, ECblue, blueSide);  // On lui demande de placer ses pièces
        }
    }
    else  // Aucun joueur n'est une IA
    {
        // On demande à chaque joueur humain de placer ses pièces
        PlacePiece(&C, ECred, redSide);
        PlacePiece(&C, ECblue, blueSide);
    }


    /*
     * Remplissage de gameState
     */

    for (i = 0 ; i < SQUARES_BY_SIDE ; i++)
    {
        for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
        {
            gameState.board[i][j].piece = EPnone;
            gameState.board[i][j].content = ECnone;
        }
    }

    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
        {
            gameState.board[9 - i][j].piece = redSide[i][j];
            gameState.board[9 - i][j].content = ECred;
        }
    }

    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
        {
            gameState.board[i][9 - j].piece = blueSide[i][j];
            gameState.board[i][9 - j].content = ECblue;
        }
    }

    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (!in.quit)
    {
        UpdateEvents(&in);

        /**
         * TODO: gestion des déplacements des pièces par les joueurs
         */

        if (currentPlayer == player1)  // Le joueur 1 doit jouer
        {
            if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
                movement = ai1.NextMove(&gameState);
            else  // Le joueur 1 est un humain
            {
                if (movePiece(&C, currentPlayer, &gameState, &movement) == -1)
                {
                    // L'utilisateur a quitté le jeu
                    return EXIT_SUCCESS;
                }
            }
        }
        else  // Le joueur 2 doit jouer
        {
            if (nbHumanPlayers == 0)  // Le joueur 2 est une IA
                movement = ai2.NextMove(&gameState);
            else  // Le joueur 2 est un humain
            {
                if (movePiece(&C, currentPlayer, &gameState, &movement) == -1)
                {
                    // L'utilisateur a quitté le jeu
                    return EXIT_SUCCESS;
                }
            }
        }

        if (isValidMove(&gameState, movement))
        {
            fprintf(stderr, "SPos: %d, %d  --> %d, %d\n", movement.start.line, movement.start.col, movement.end.line, movement.end.col);
            fprintf(stderr, "end before: piece, content: %d, %d\n", gameState.board[movement.end.line][movement.end.col].piece, gameState.board[movement.end.line][movement.end.col].content);
            fprintf(stderr, "start before: piece, content: %d, %d\n", gameState.board[movement.start.line][movement.start.col].piece, gameState.board[movement.start.line][movement.start.col].content);
            gameState.board[movement.end.line][movement.end.col].piece = gameState.board[movement.start.line][movement.start.col].piece;
            gameState.board[movement.end.line][movement.end.col].content = gameState.board[movement.start.line][movement.start.col].content;
            gameState.board[movement.start.line][movement.start.col].content = ECnone;
            gameState.board[movement.start.line][movement.start.col].piece = EPnone;
            fprintf(stderr, "end after: piece, content: %d, %d\n", gameState.board[movement.end.line][movement.end.col].piece, gameState.board[movement.end.line][movement.end.col].content);
            fprintf(stderr, "start after: piece, content: %d, %d\n", gameState.board[movement.start.line][movement.start.col].piece, gameState.board[movement.start.line][movement.start.col].content);
        }

        // Effacement de l'écran
        SDL_FillRect(C.screen, NULL, SDL_MapRGB(C.screen->format, 255, 255, 255));

        // Affichage du plateau
        Blit(C.board, C.screen, 0, 0);

        for (i = 0 ; i < SQUARES_BY_SIDE ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                x = j * SQUARE_SIZE;
                y = i * SQUARE_SIZE;

                if (gameState.board[i][j].piece != EPnone)
                {
                    if (gameState.board[i][j].content == ECred)
                        Blit(C.images[IMGRED][gameState.board[i][j].piece], C.screen, x, y);
                    else
                        Blit(C.images[IMGBLUE][gameState.board[i][j].piece], C.screen, x, y);
                }
            }
        }

        // Affichage du nom du jeu
        C.text = TTF_RenderText_Blended(C.fonts[BIGTEXT], "Stratego", blackColor);
        x = WINDOW_WIDTH - (500/2) - (C.text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 5;
        Blit(C.text, C.screen, x, y);

        // Affichage des noms des joueurs (pour afficher en dessous le nombre de pièces perdues par le joueur)
        C.text = TTF_RenderText_Blended(C.fonts[MEDIUMTEXT], p1Name, blackColor);
        x = WINDOW_WIDTH - 500 + 20;
        y = 70;
        Blit(C.text, C.screen, x, y);

        C.text = TTF_RenderText_Blended(C.fonts[MEDIUMTEXT], p2Name, blackColor);
        x = WINDOW_WIDTH - 500 + 20;
        y = WINDOW_HEIGHT / 2;
        Blit(C.text, C.screen, x, y);


        SDL_Flip(C.screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }


    FreeAll(&C);  // Libération de toutes les ressources utilisées pour l'affichage


    /**
     * Libération des ressources des libs ouvertes
     */

    if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
    {
        dlclose(ai1.lib);

        if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
            dlclose(ai2.lib);
    }

    return EXIT_SUCCESS;
}
