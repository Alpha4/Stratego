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

void placementPiece(SDL_Surface *ecran, EColor color, EPiece side[4][10])
{
    SDL_Event event;
    int continuer = 1;
    int i, j;
    SDL_Surface *plateau = NULL, *bombRED = NULL, *spyRED = NULL, *scoutRED = NULL, *minerRED = NULL, *sergeantRED = NULL, *lieutenantRED = NULL, *captainRED = NULL, *majorRED = NULL, *colonelRED = NULL, *generalRED = NULL, *marshalRED = NULL, *flagRED = NULL, *bombBLUE = NULL, *spyBLUE = NULL, *scoutBLUE = NULL, *minerBLUE = NULL, *sergeantBLUE = NULL, *lieutenantBLUE = NULL, *captainBLUE = NULL, *majorBLUE = NULL, *colonelBLUE = NULL, *generalBLUE = NULL, *marshalBLUE = NULL, *flagBLUE = NULL, *texte = NULL, *impossible= NULL;
    SDL_Rect position;  // Utilisé pour positionner chaque surface
    TTF_Font *policeTitre = NULL, *policeSousTitre = NULL, *policeTexte = NULL;  // Polices d'écriture du texte dans la fenêtre
    SDL_Color couleurNoire = {0, 0, 0};  // Couleur noire pour le texte

    // Position de la surface contenant le plateau
    SDL_Rect positionPlateau;
    positionPlateau.x = 0;
    positionPlateau.y = 0;

    EPiece currentPiece;  // Prochaine pièce que le joueur doit placer
    int nbCurrentPieceLeft;  // Nombre de currentPiece qu'il reste à placer

    policeTitre = TTF_OpenFont("DejaVuSans.ttf", 56);  // Chargement de la police du titres
    policeSousTitre = TTF_OpenFont("DejaVuSans.ttf", 24);  // Chargement de la police des sous-titres
    policeTexte = TTF_OpenFont("DejaVuSans.ttf", 14);  // Chargement de la police du texte

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

    // Surface qui noircit la partie du plateau où l'on ne peut rien placer
    impossible = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE * NB_BLOCS_COTE, TAILLE_CASE * 6, 32, 0, 0, 0, 0);
    SDL_FillRect(impossible, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_SetAlpha(impossible, SDL_SRCALPHA, 192);

    // Première pièce à placer
    currentPiece = EPbomb;
    nbCurrentPieceLeft = getNbPieceAPlacer(currentPiece);

    // Initialisation du plateau
    for (i = 0 ; i < 4 ; i++)
    {
        for (j = 0 ; j < 10 ; j++)
            side[i][j] = EPnone;
    }

    // Boucle qui attends que l'utilisateur ferme le programme pour s'arrêter
    while (continuer)
    {
        // SDL_Delay(30);  // Attente de 30ms entre chaque tour de boucle pour en pas surcharger le CPU
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONUP:  // Clic de la souris
                if (event.button.button == SDL_BUTTON_LEFT)  // Clic gauche
                {
                    // On vérifie que le clic est bien dans une case disponible
                    i = (int) ((HAUTEUR_FENETRE - event.button.y) / TAILLE_CASE);
                    j = (int) (event.button.x / TAILLE_CASE);

                    fprintf(stderr, "clic : i : %d, j : %d, x : %d, y : %d\n", i, j, event.button.x, event.button.y);
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
                                currentPiece = getNextPieceAPlacer(currentPiece);
                                if (currentPiece != EPnone)
                                    nbCurrentPieceLeft = getNbPieceAPlacer(currentPiece);
                                else
                                    continuer = 0;
                            }
                        }
                    }
                }
                break;
        }

        // Effacement de l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        // Affichage du plateau
        SDL_BlitSurface(plateau, NULL, ecran, &positionPlateau);
        SDL_BlitSurface(impossible, NULL, ecran, &positionPlateau);  // On noircit le plateau à l'endroit où le joueur ne peut pas placer de pièces

        for (i = 0 ; i < 4 ; i++)
        {
            for (j = 0 ; j < 10 ; j++)
            {
                position.x = j * TAILLE_CASE;
                position.y = HAUTEUR_FENETRE - (i + 1) * TAILLE_CASE;

                switch(side[i][j])
                {
                    case EPbomb:
                        if (color == ECred)
                            SDL_BlitSurface(bombRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(bombBLUE, NULL, ecran, &position);
                        break;
                    case EPspy:
                        if (color == ECred)
                            SDL_BlitSurface(spyRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(spyBLUE, NULL, ecran, &position);
                        break;
                    case EPscout:
                        if (color == ECred)
                            SDL_BlitSurface(scoutRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(scoutBLUE, NULL, ecran, &position);
                        break;
                    case EPminer:
                        if (color == ECred)
                            SDL_BlitSurface(minerRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(minerBLUE, NULL, ecran, &position);
                        break;
                    case EPsergeant:
                        if (color == ECred)
                            SDL_BlitSurface(sergeantRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(sergeantBLUE, NULL, ecran, &position);
                        break;
                    case EPlieutenant:
                        if (color == ECred)
                            SDL_BlitSurface(lieutenantRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(lieutenantBLUE, NULL, ecran, &position);
                        break;
                    case EPcaptain:
                        if (color == ECred)
                            SDL_BlitSurface(captainRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(captainBLUE, NULL, ecran, &position);
                        break;
                    case EPmajor:
                        if (color == ECred)
                            SDL_BlitSurface(majorRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(majorBLUE, NULL, ecran, &position);
                        break;
                    case EPcolonel:
                        if (color == ECred)
                            SDL_BlitSurface(colonelRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(colonelBLUE, NULL, ecran, &position);
                        break;
                    case EPgeneral:
                        if (color == ECred)
                            SDL_BlitSurface(generalRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(generalBLUE, NULL, ecran, &position);
                        break;
                    case EPmarshal:
                        if (color == ECred)
                            SDL_BlitSurface(marshalRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(marshalBLUE, NULL, ecran, &position);
                        break;
                    case EPflag:
                        if (color == ECred)
                            SDL_BlitSurface(flagRED, NULL, ecran, &position);
                        else
                            SDL_BlitSurface(flagBLUE, NULL, ecran, &position);
                        break;
                }
            }
        }

        // Affichage du nom du jeu
        texte = TTF_RenderText_Blended(policeTitre, "Stratego", couleurNoire);
        position.x = LARGEUR_FENETRE - (500/2) - (texte->w/2);  // On centre le texte dans la surface à droite du plateau
        position.y = 5;
        SDL_BlitSurface(texte, NULL, ecran, &position);

        // Affichage de la pièce à placer
        texte = TTF_RenderText_Blended(policeSousTitre, getNomPieceAPlacer(currentPiece), couleurNoire);
        position.x = LARGEUR_FENETRE - (500/2) - (texte->w/2);  // On centre le texte dans la surface à droite du plateau
        position.y = 150;
        SDL_BlitSurface(texte, NULL, ecran, &position);

        // Affichage de la couleur du joueur
        if (color == ECred)
            texte = TTF_RenderText_Blended(policeSousTitre, "Joueur Rouge : ", couleurNoire);
        else
            texte = TTF_RenderText_Blended(policeSousTitre, "Joueur Bleu : ", couleurNoire);
        position.x = LARGEUR_FENETRE - (500/2) - (texte->w/2);  // On centre le texte dans la surface à droite du plateau
        position.y = 100;
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
    SDL_FreeSurface(impossible);

    TTF_CloseFont(policeTitre);
    TTF_CloseFont(policeSousTitre);
    TTF_CloseFont(policeTexte);
}

char* getNomPieceAPlacer(EPiece piece)
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
    return "";
}

EPiece getNextPieceAPlacer(EPiece piece)
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

int getNbPieceAPlacer(EPiece piece)
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