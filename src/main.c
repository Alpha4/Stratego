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

int main(int argc, char *argv[])
{
    /**
     * argv[1] = Nombre de joueurs humain
     * argv[2] = Chemin vers la lib de l'IA 1 (si nécessaire)
     * argv[3] = Chemin vers la lib de l'IA 2 (si nécessaire)
     */

    /**
     * Variables concernant le jeu de Stratego
     */

    SGameState gameState;  // Le jeu (plateau + pièces éliminées)

    /**
     * Etat du jeu pour gérer l'interface graphique (notamment les clics de souris):
     * 0 = partie non commencée
     * 1 = En attente d'une sélection de pièce par le joueur
     * 2 = En attente d'un choix de destination de la pièce pour le joueur
     */
    int gameStatus;

    int currentPlayer;  // Joueur qui est en train de jouer (1 ou 2)
    char j1Name[50] = "Joueur 1", j2Name[50] = "Joueur 2";  // Noms des joueurs


    /**
     * Variables concernant la gestion de la SDL
     */

    Context C;

    Input in;
    memset(&in, 0, sizeof(in));

    int i, j;
    int x, y;
    TTF_Font *policeTitre = NULL, *policeSousTitre = NULL, *policeTexte = NULL;  // Polices d'écriture du texte dans la fenêtre
    SDL_Color couleurNoire = {0, 0, 0};  // Couleur noire pour le texte
    SDL_Surface *texte;
    /**
     * Variables concernant la gestion des lib
     */

    void *lib1 = NULL, *lib2 = NULL;  // Pointeurs vers les lib

    // Pointeurs vers les fonctions des libs pour chaque joueur
    pfInitLibrary j1InitLibrary, j2InitLibrary;
    pfStartMatch j1StartMatch, j2StartMatch;
    pfStartGame j1StartGame, j2StartGame;
    pfEndGame j1EndGame, j2EndGame;
    pfEndMatch j1EndMatch, j2EndMatch;
    pfNextMove j1NextMove, j2NextMove;
    pfAttackResult j1AttackResult, j2AttackResult;
    pfPenalty j1Penalty, j2Penalty;

    /**
     * Chargement des lib si besoin
     */

    // Vérification qu'il y a au moins en paramètre le nombre de joueurs humains
    if (argc < 2)
    {
        fprintf(stderr, "Erreur : pas assez de paramètres\n"); // Écriture de l'erreur
        return EXIT_FAILURE;
    }

    if (atoi(argv[1]) == 1) // 1 joueur humain
    {
        if (argc < 3) // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n"); // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        // On charge une librairie pour le joueur 1
        if ((lib1 = dlopen(argv[2], RTLD_LAZY)) == NULL)
        {
            // Erreur de chargement de la lib
            fprintf(stderr, "Erreur de chargement de la lib : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        // Chargement des pointeurs de fonctions pour chaque fonction de la lib
        if ((j1InitLibrary = (pfInitLibrary) dlsym(lib1, "InitLibrary")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1StartMatch = (pfStartMatch) dlsym(lib1, "StartMatch")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1StartGame = (pfStartGame) dlsym(lib1, "StartGame")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1EndGame = (pfEndGame) dlsym(lib1, "EndGame")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1EndMatch = (pfEndMatch) dlsym(lib1, "EndMatch")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1NextMove = (pfNextMove) dlsym(lib1, "NextMove")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1AttackResult = (pfAttackResult) dlsym(lib1, "AttackResult")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j1Penalty = (pfPenalty) dlsym(lib1, "Penalty")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
    }
    if (atoi(argv[1]) == 0) // 0 joueur humain
    {
        if (argc < 4) // Si le chemin vers la lib n'est pas fourni
        {
            fprintf(stderr, "Erreur : pas assez de paramètres\n"); // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        // On charge également une librairie pour le joueur 2
        if ((lib2 = dlopen(argv[3], RTLD_LAZY)) == NULL)
        {
            // Erreur de chargement de la lib
            fprintf(stderr, "Erreur de chargement de la lib : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }

        // Chargement des pointeurs de fonctions pour chaque fonction de la lib
        if ((j2InitLibrary = (pfInitLibrary) dlsym(lib2, "InitLibrary")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2StartMatch = (pfStartMatch) dlsym(lib2, "StartMatch")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2StartGame = (pfStartGame) dlsym(lib2, "StartGame")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2EndGame = (pfEndGame) dlsym(lib2, "EndGame")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2EndMatch = (pfEndMatch) dlsym(lib2, "EndMatch")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2NextMove = (pfNextMove) dlsym(lib2, "NextMove")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2AttackResult = (pfAttackResult) dlsym(lib2, "AttackResult")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
        if ((j2Penalty = (pfPenalty) dlsym(lib2, "Penalty")) == NULL)
        {
            // Erreur de chargement de la fonction
            fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror()); // Écriture de l'erreur
            return EXIT_FAILURE;
        }
    }


    /**
     * Initialisation de l'affichage
     */
    if (Init(&C, LARGEUR_FENETRE, HAUTEUR_FENETRE, "Stratego") == -1)
        return EXIT_FAILURE;

    policeTitre = TTF_OpenFont("DejaVuSans.ttf", 56);  // Chargement de la police du titres
    policeSousTitre = TTF_OpenFont("DejaVuSans.ttf", 24);  // Chargement de la police des sous-titres
    policeTexte = TTF_OpenFont("DejaVuSans.ttf", 14);  // Chargement de la police du texte

    /**
     * Initialisation de la partie
     */

    if (lib1 != NULL)  // Le joueur 1 est une IA
    {
        j1InitLibrary(j1Name);  // On lui demande son nom
        j1StartMatch();  // On l'initialise

        if (lib2 != NULL)  // Le joueur 2 est aussi une IA
        {
            j2InitLibrary(j2Name);  // On lui demande son nom
            j2StartMatch();  // On l'initialise
        }
    }

    /**
     * Placement des pièces par les joueurs
     */
    EPiece redSide[4][10], blueSide[4][10];  // Placement initial des pièce par les joueurs

    if (lib1 != NULL)  // Le joueur 1 est une IA
    {
        j1StartGame(ECred, redSide);  // On lui demande de placer ses pièces

        if (lib2 != NULL)  // Le joueur 2 est aussi une IA
        {
            j2StartGame(ECblue, blueSide);  // On lui demande de placer ses pièces
        }
        else
        {
            placementPiece(&C, ECblue, blueSide);  // On lui demande de placer ses pièces
        }
    }
    else  // Aucun joueur n'est une IA
    {
        // On demande à chaque joueur humain de placer ses pièces
        placementPiece(&C, ECred, redSide);
        placementPiece(&C, ECblue, blueSide);
    }


    /*
    Temporaire : remplissage de gameState avec des pièces pour tester l'affichage
     */

    for (i = 0 ; i < NB_BLOCS_COTE ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_COTE ; j++)
        {
            gameState.board[i][j].piece = EPspy;
            gameState.board[i][j].content = ECred;
        }
    }
    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (!in.quit)
    {
        UpdateEvents(&in);

        /**
         * TODO: gestion des déplacements des pièces par les joueurs
         */

        // Effacement de l'écran
        SDL_FillRect(C.screen, NULL, SDL_MapRGB(C.screen->format, 255, 255, 255));

        // Affichage du plateau
        Blit(C.plateau, C.screen, 0, 0);

        for (i = 0 ; i < NB_BLOCS_COTE ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_COTE ; j++)
            {
                x = j * TAILLE_CASE;
                y = i * TAILLE_CASE;

                if (gameState.board[i][j].piece != EPnone)
                {
                    if (gameState.board[i][j].content == ECred)
                        Blit(C.images[ImgRed][gameState.board[i][j].piece], C.screen, x, y);
                    else
                        Blit(C.images[ImgBlue][gameState.board[i][j].piece], C.screen, x, y);
                }
            }
        }

        // Affichage du nom du jeu
        texte = TTF_RenderText_Blended(policeTitre, "Stratego", couleurNoire);
        x = LARGEUR_FENETRE - (500/2) - (texte->w/2);  // On centre le texte dans la surface à droite du plateau
        y = 5;
        Blit(texte, C.screen, x, y);

        // Affichage des noms des joueurs (pour afficher en dessous le nombre de pièces perdues par le joueur)
        texte = TTF_RenderText_Blended(policeSousTitre, j1Name, couleurNoire);
        x = LARGEUR_FENETRE - 500 + 20;
        y = 70;
        Blit(texte, C.screen, x, y);
        texte = TTF_RenderText_Blended(policeSousTitre, j2Name, couleurNoire);
        x = LARGEUR_FENETRE - 500 + 20;
        y = HAUTEUR_FENETRE / 2;
        Blit(texte, C.screen, x, y);


        SDL_Flip(C.screen);  // Affichage de l'écran

        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
    }

    FreeImages(&C);

    SDL_FreeSurface(texte);

    SDL_Quit();
    TTF_CloseFont(policeTitre);
    TTF_CloseFont(policeSousTitre);
    TTF_CloseFont(policeTexte);
    TTF_Quit();

    /**
     * Libération des ressources des libs ouvertes
     */
    if (lib1 != NULL)
        dlclose(lib1);
    if (lib2 != NULL)
        dlclose(lib2);

    return EXIT_SUCCESS;
}
