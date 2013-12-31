/*

Programme principal (main), qui gère l'interface, l'arbitre et fait appel aux lib des IA quand il le faut.

*/

#include "stratego.h"
#include "interface.h"
#include "arbitre.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

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


    /**
     * Variables concernant la gestion de la SDL
     */

    SDL_Surface *ecran = NULL; // La fenêtre du jeu
    SDL_Event event;
    int continuer = 1;
    int i, j;
    SDL_Surface *plateau = NULL, *bombRED = NULL, *spyRED = NULL, *scoutRED = NULL, *minerRED = NULL, *sergeantRED = NULL, *lieutenantRED = NULL, *captainRED = NULL, *majorRED = NULL, *colonelRED = NULL, *generalRED = NULL, *marshalRED = NULL, *flagRED = NULL, *bombBLUE = NULL, *spyBLUE = NULL, *scoutBLUE = NULL, *minerBLUE = NULL, *sergeantBLUE = NULL, *lieutenantBLUE = NULL, *captainBLUE = NULL, *majorBLUE = NULL, *colonelBLUE = NULL, *generalBLUE = NULL, *marshalBLUE = NULL, *flagBLUE = NULL, *texte = NULL;
    SDL_Rect position;  // Utilisé pour positionner chaque surface

    // Position de la surface contenant le plateau
    SDL_Rect positionPlateau;
    positionPlateau.x = 0;
    positionPlateau.y = 0;

    /**
     * Variables concernant la gestion des lib
     */

    void *lib1, *lib2;  // Pointeurs vers les lib

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

    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }

    if (TTF_Init() == -1)  // Démarragge de SDL_ttf pour pouvoir afficher du texte
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    TTF_Font *police = NULL;  // Police d'écriture du texte dans la fenêtre
    police = TTF_OpenFont("DejaVuSans.ttf", 56);  // Chargement de la police
    SDL_Color couleurNoire = {0, 0, 0};  // Couleur noire pour le texte

    // SDL_WM_SetIcon(IMG_Load("images/truc.png"), NULL);  // Icone du jeu
    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  // Création de la fenêtre du jeu
    SDL_WM_SetCaption("Stratego", NULL);  // Titre de la fenêtre

    // Chargement des images que l'on va afficher
    bombRED = IMG_Load("images/bombRED.png");
    bombBLUE = IMG_Load("images/bombBLUE.png");
    spyRED = IMG_Load("images/spyRED.png");
    spyBLUE = IMG_Load("images/spyBLUE.png");
    scoutRED = IMG_Load("images/scoutRED.png");
    scoutBLUE = IMG_Load("images/scoutBLUE.png");
    minerRED = IMG_Load("images/minerRED.png");
    minerBLUE = IMG_Load("images/minerBLUE.png");
    sergeantRED = IMG_Load("images/sergeantRED.png");
    sergeantBLUE = IMG_Load("images/sergeantBLUE.png");
    lieutenantRED = IMG_Load("images/lieutenantRED.png");
    lieutenantBLUE = IMG_Load("images/lieutenantBLUE.png");
    captainRED = IMG_Load("images/captainRED.png");
    captainBLUE = IMG_Load("images/captainBLUE.png");
    majorRED = IMG_Load("images/majorRED.png");
    majorBLUE = IMG_Load("images/majorBLUE.png");
    colonelRED = IMG_Load("images/colonelRED.png");
    colonelBLUE = IMG_Load("images/colonelBLUE.png");
    generalRED = IMG_Load("images/generalRED.png");
    generalBLUE = IMG_Load("images/generalBLUE.png");
    marshalRED = IMG_Load("images/marshalRED.png");
    marshalBLUE = IMG_Load("images/marshalBLUE.png");
    flagRED = IMG_Load("images/flagRED.png");
    flagBLUE = IMG_Load("images/flagBLUE.png");
    plateau = SDL_LoadBMP("images/plateau.bmp");

    /*
    Temporaire : remplissage de gameState avec des pièces pour tester l'affichage
     */

    for (i = 0 ; i < NB_BLOCS_COTE ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_COTE ; j++)
        {
            gameState.board[i][j].piece = i;
            gameState.board[i][j].content = ECblue;
        }
    }


    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (continuer)
    {
        SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
        SDL_PollEvent(&event);
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

        // Affichage du nom du jeu
        texte = TTF_RenderText_Blended(police, "Stratego", couleurNoire);
        position.x = LARGEUR_FENETRE - (500/2) - (texte->w/2);  // On centre le texte dans la surface à droite du plateau
        position.y = 20;
        SDL_BlitSurface(texte, NULL, ecran, &position);



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
    SDL_FreeSurface(texte);

    SDL_Quit();
    TTF_CloseFont(police);
    TTF_Quit();

    /**
     * Libération des ressources des libs ouvertes
     */
    if (atoi(argv[1]) == 1) // 1 joueur humain
    {
        // On ferme la librairie du joueur 1
        dlclose(lib1);
    }
    if (atoi(argv[1]) == 0) // 0 joueur humain
    {
        // On ferme aussi la librairie du joueur 1
        dlclose(lib2);
    }

    return EXIT_SUCCESS;
}