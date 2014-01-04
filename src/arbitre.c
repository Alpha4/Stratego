/*

Fonctions utilisées pour l'arbitrage du jeu

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

int coupValide(SGameState *gameState, SMove move)
{
    return 1;
}
