/*

Programme principal (main), qui gère l'interface, l'arbitre et fait appel aux lib des IA quand il le faut.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
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
     * argv[1] = Nombre de coups maximum
     * argv[2] = Nombre de joueurs artificiels
     * argv[3] = Chemin vers la lib de l'IA 1 (si nécessaire)
     * argv[4] = Chemin vers la lib de l'IA 2 (si nécessaire)
     */


    /**
     * Variables concernant le jeu de Stratego
     */

    SGameState gameState;  // Le jeu (plateau + pièces éliminées)
    SGameState gameStateCopy;  // Le jeu (plateau + pièces éliminées)

    int nbHumanPlayers;

    SMove histMove[2][3];  // Les trois derniers mouvement du joueur de chaque joueur
    memset(histMove, -1, sizeof(histMove));  // On met à 0 le contenu de chaque case de histMove

    int penalty[2];  // Le nombre de pénalité pour chaque joueur
    EColor player1 = ECred, player2 = ECblue;

    EColor currentPlayer = ECred;  // Joueur qui est en train de jouer (ECred ou ECblue)
    char p1Name[50], p2Name[50];  // Noms des joueurs
    EPiece p1Side[4][10], p2Side[4][10];  // Tableau de placement initial des pièce par les joueurs

    SMove movement;

    int nbMoveLeft = atoi(argv[1]);

    if (nbMoveLeft < 1)
    {
        fprintf(stderr, "Erreur : nombre maximum de coups incorrect\n");  // Écriture de l'erreur
        return EXIT_FAILURE;
    }


    /**
     * Variables concernant la gestion de la SDL
     */

    Context C;

    Input in;
    memset(&in, 0, sizeof(in));

    int i, j;
    int x, y;
    int result;

    SDL_Color blackColor = {0, 0, 0};  // Couleur noire pour le texte

    /**
     * Variables concernant la gestion des lib
     */

    AI ai1, ai2;

    /**
     * Randomisation de la couleur des joueurs
     */

    srand(time(NULL));
    player1 = (rand() % 2) + 2;
    if (player1 == ECblue)
    {
        player2 = ECred;
        strcpy(p1Name, "Bleus");
        strcpy(p2Name, "Rouges");
    }
    else
    {
        player2 = ECblue;
        strcpy(p1Name, "Rouges");
        strcpy(p2Name, "Bleus");
    }

    /**
     * Chargement des lib si besoin
     */

    // Vérification qu'il y a au moins en paramètre le nombre de joueurs humains
    if (argc < 3)
    {
        fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
        return EXIT_FAILURE;
    }

    nbHumanPlayers = 2 - atoi(argv[2]);  // On stocke le nombre de joueurs humains

    if (nbHumanPlayers < 0 || nbHumanPlayers > 2)
    {
        fprintf(stderr, "Erreur : nombre d'IA incorrect\n");  // Écriture de l'erreur
        return EXIT_FAILURE;
    }
    if (nbHumanPlayers == 1)  // 1 joueur humain
    {
        if (argc < 4)  // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        if (LoadLib(&ai1, argv[3]) == -1)
            return EXIT_FAILURE;
    }

    if (nbHumanPlayers == 0)  // 0 joueur humain
    {
        if (argc < 5)  // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n");  // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        if (LoadLib(&ai2, argv[4]) == -1)
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

    if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
    {
        ai1.StartGame(player1, p1Side);  // On lui demande de placer ses pièces

        if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
        {
            ai2.StartGame(player2, p2Side);  // On lui demande de placer ses pièces
        }
        else
        {
            if (PlacePiece(&C, player2, p2Side) == -1)  // On lui demande de placer ses pièces
            {
                // L'utilisateur a quitté le jeu
                return EXIT_SUCCESS;
            }
        }
    }
    else  // Aucun joueur n'est une IA
    {
        // On demande à chaque joueur humain de placer ses pièces
        if (PlacePiece(&C, player1, p1Side) == -1)
        {
            // L'utilisateur a quitté le jeu
            return EXIT_SUCCESS;
        }
        if (PlacePiece(&C, player2, p2Side) == -1)
        {
            // L'utilisateur a quitté le jeu
            return EXIT_SUCCESS;
        }
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

    // Placement des lacs (ligne 4 et 5, colonnes 2, 3, 6 et 7)
    for (i = 4 ; i <= 5 ; i++)
    {
        for (j = 2 ; j <= 7 ; j++)
        {
            if (j != 4 && j != 5)
            {
                gameState.board[j][j].piece = EPnone;
                gameState.board[i][j].content = EClake;
            }
        }
    }


    if (player1 == ECred)
    {
        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                gameState.board[9 - i][j].piece = p1Side[i][j];
                gameState.board[9 - i][j].content = player1;
            }
        }

        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                gameState.board[i][9 - j].piece = p2Side[i][j];
                gameState.board[i][9 - j].content = player2;
            }
        }
    }
    else
    {
        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                gameState.board[9 - i][j].piece = p2Side[i][j];
                gameState.board[9 - i][j].content = player2;
            }
        }

        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < SQUARES_BY_SIDE ; j++)
            {
                gameState.board[i][9 - j].piece = p1Side[i][j];
                gameState.board[i][9 - j].content = player1;
            }
        }
    }

    // Initialisation du nombre de pièces éliminées pour chaque joueur
    for (i = 0 ; i < 11 ; i++)
    {
        gameState.redOut[i] = 0;
        gameState.blueOut[i] = 0;
    }

    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (!in.quit)
    {
        UpdateEvents(&in);

        manageBoard(&gameState, &gameStateCopy, currentPlayer);

        if (currentPlayer == player1)  // Le joueur 1 doit jouer
        {
            if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
                movement = ai1.NextMove(&gameStateCopy);
            else  // Le joueur 1 est un humain
            {
                if (movePiece(&C, currentPlayer, &gameStateCopy, &movement, p1Name) == -1)
                {
                    // L'utilisateur a quitté le jeu
                    return EXIT_SUCCESS;
                }
            }
        }
        else  // Le joueur 2 doit jouer
        {
            if (nbHumanPlayers == 0)  // Le joueur 2 est une IA
                movement = ai2.NextMove(&gameStateCopy);
            else  // Le joueur 2 est un humain
            {
                if (movePiece(&C, currentPlayer, &gameStateCopy, &movement, p2Name) == -1)
                {
                    // L'utilisateur a quitté le jeu
                    return EXIT_SUCCESS;
                }
            }

            // On diminue le nombre de coups restant
            nbMoveLeft--;
        }

        // Si c'est le joueur bleu qui joue, il voit le plateau à l'envers, donc on doit retourner le mouvement
        if (currentPlayer == ECblue)
        {
            movement.start.line = 9 - movement.start.line;
            movement.start.col = 9 - movement.start.col;
            movement.end.line = 9 - movement.end.line;
            movement.end.col = 9 - movement.end.col;
        }


        // Si le déplacement est valide, on l'effectue
        if (isValidMove(&gameState, movement, currentPlayer, histMove[currentPlayer - 2]))
        {
            if (gameState.board[movement.end.line][movement.end.col].piece != EPnone)  // Il y a une pièce sur la case où le joueur se déplace, c'est donc une attaque
            {
                // On envoie l'info aux IA si nécessaire
                if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
                {
                    if (currentPlayer == player1)
                        ai1.AttackResult(movement.start, gameState.board[movement.start.line][movement.start.col].piece, movement.end, gameState.board[movement.end.line][movement.end.col].piece);
                    else
                        ai1.AttackResult(movement.end, gameState.board[movement.end.line][movement.end.col].piece, movement.start, gameState.board[movement.start.line][movement.start.col].piece);

                    if (nbHumanPlayers == 0)  // Le joueur 2 est aussi une IA
                    {
                        if (currentPlayer == player2)
                            ai2.AttackResult(movement.start, gameState.board[movement.start.line][movement.start.col].piece, movement.end, gameState.board[movement.end.line][movement.end.col].piece);
                        else
                            ai2.AttackResult(movement.end, gameState.board[movement.end.line][movement.end.col].piece, movement.start, gameState.board[movement.start.line][movement.start.col].piece);
                    }
                }

                result = Attack(gameState.board[movement.start.line][movement.start.col].piece, gameState.board[movement.end.line][movement.end.col].piece);

                switch(result)
                {
                    case 0:  // On supprime les deux pièces (elles s'entretuent)
                        // On augmente le nombre de pièces éliminées pour chaque joueur (ils perdent tous les deux la même pièce)
                        gameState.redOut[gameState.board[movement.end.line][movement.end.col].piece]++;
                        gameState.blueOut[gameState.board[movement.end.line][movement.end.col].piece]++;
                        gameState.board[movement.start.line][movement.start.col].content = ECnone;
                        gameState.board[movement.start.line][movement.start.col].piece = EPnone;
                        gameState.board[movement.end.line][movement.end.col].content = ECnone;
                        gameState.board[movement.end.line][movement.end.col].piece = EPnone;
                        break;

                    case 1:  // L'attaquant gagne
                        if (currentPlayer == ECred)
                            gameState.blueOut[gameState.board[movement.end.line][movement.end.col].piece]++;
                        else
                            gameState.redOut[gameState.board[movement.end.line][movement.end.col].piece]++;
                        gameState.board[movement.end.line][movement.end.col].piece = gameState.board[movement.start.line][movement.start.col].piece;
                        gameState.board[movement.end.line][movement.end.col].content = gameState.board[movement.start.line][movement.start.col].content;
                        gameState.board[movement.start.line][movement.start.col].content = ECnone;
                        gameState.board[movement.start.line][movement.start.col].piece = EPnone;
                        break;

                    case 2:  // L'attaquant perd
                        if (currentPlayer == ECred)
                            gameState.redOut[gameState.board[movement.start.line][movement.start.col].piece]++;
                        else
                            gameState.blueOut[gameState.board[movement.start.line][movement.start.col].piece]++;
                        gameState.board[movement.start.line][movement.start.col].content = ECnone;
                        gameState.board[movement.start.line][movement.start.col].piece = EPnone;
                        break;
                }
            }
            else
            {
                gameState.board[movement.end.line][movement.end.col].piece = gameState.board[movement.start.line][movement.start.col].piece;
                gameState.board[movement.end.line][movement.end.col].content = gameState.board[movement.start.line][movement.start.col].content;
                gameState.board[movement.start.line][movement.start.col].content = ECnone;
                gameState.board[movement.start.line][movement.start.col].piece = EPnone;
            }
        }
        else
        {
            penalty[currentPlayer - 2]++;  // On augmente le compteur de pénalité du joueur

            // On envoie l'info à l'IA si besoin
            if (currentPlayer == player1)  // Le joueur 1 a joué
            {
                if (nbHumanPlayers == 1)  // Le joueur 1 est une IA
                    ai1.Penalty();
            }
            else  // Le joueur a joué
            {
                if (nbHumanPlayers == 0)  // Le joueur 2 est une IA
                    ai2.Penalty();
            }
        }

        // C'est maintenant à l'autre joueur de jouer
        if (currentPlayer == player1)
            currentPlayer = player2;
        else
            currentPlayer = player1;

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
        C.text = TTF_RenderUTF8_Blended(C.fonts[BIGTEXT], "Stratego", blackColor);
        x = WINDOW_WIDTH - (500/2) - (C.text->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 5;
        Blit(C.text, C.screen, x, y);

        // Affichage des noms des joueurs (pour afficher en dessous le nombre de pièces perdues par le joueur)
        C.text = TTF_RenderUTF8_Blended(C.fonts[MEDIUMTEXT], p1Name, blackColor);
        x = WINDOW_WIDTH - 500 + 20;
        y = 70;
        Blit(C.text, C.screen, x, y);

        C.text = TTF_RenderUTF8_Blended(C.fonts[MEDIUMTEXT], p2Name, blackColor);
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
