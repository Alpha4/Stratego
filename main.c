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
    SDL_Surface *ecran = NULL; // La fenêtre du jeu

    int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetIcon(IMG_Load("truc.jpg"), NULL);  // Icone du jeu
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

        // Effacement de l'écran (= le remplir de blanc)
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_Flip(ecran); // Affichage de l'écran
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}