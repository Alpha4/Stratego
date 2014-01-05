/*

Fonctions utilisées pour la gestion des libs chargées

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

int LoadLib(AI *ai, char* file)
{
    // On charge toutes les fonctions de la lib
    if ((ai->lib = dlopen(file, RTLD_LAZY)) == NULL)
    {
        // Erreur de chargement de la lib
        fprintf(stderr, "Erreur de chargement de la lib : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }

    // Chargement des pointeurs de fonctions pour chaque fonction de la lib
    if ((ai->InitLibrary = (pfInitLibrary) dlsym(ai->lib, "InitLibrary")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->StartMatch = (pfStartMatch) dlsym(ai->lib, "StartMatch")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->StartGame = (pfStartGame) dlsym(ai->lib, "StartGame")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->EndGame = (pfEndGame) dlsym(ai->lib, "EndGame")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->EndMatch = (pfEndMatch) dlsym(ai->lib, "EndMatch")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->NextMove = (pfNextMove) dlsym(ai->lib, "NextMove")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->AttackResult = (pfAttackResult) dlsym(ai->lib, "AttackResult")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }
    if ((ai->Penalty = (pfPenalty) dlsym(ai->lib, "Penalty")) == NULL)
    {
        // Erreur de chargement de la fonction
        fprintf(stderr, "Erreur de chargement de la fonction : %s\n", dlerror());  // Écriture de l'erreur
        return -1;
    }

    return 0;
}