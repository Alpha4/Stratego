/*

Fonctions gérant l'interface, notamment les fonctions gérant les interractions avec un joueur humain et non une IA.

*/

void AfficherJeu(SDL_Surface *ecran, SGameState *gameState)
{

    // Tout le contenu de cette fonction sera probablement à mettre dans le main
    // pour éviter de charger toutes les images à chaque appel de la fonction

    int i, j;

    // Chargement des images que l'on va afficher
    bombRED = SDL_LoadBMP("bombRED.bmp");
    bombBLUE = SDL_LoadBMP("bombBLUE.bmp");
    spyRED = SDL_LoadBMP("spyRED.bmp");
    spyBLUE = SDL_LoadBMP("spyBLUE.bmp");
    scoutRED = SDL_LoadBMP("scoutRED.bmp");
    scoutBLUE = SDL_LoadBMP("scoutBLUE.bmp");
    minerRED = SDL_LoadBMP("minerRED.bmp");
    minerBLUE = SDL_LoadBMP("minerBLUE.bmp");
    sergeantRED = SDL_LoadBMP("sergeantRED.bmp");
    sergeantBLUE = SDL_LoadBMP("sergeantBLUE.bmp");
    lieutenantRED = SDL_LoadBMP("lieutenantRED.bmp");
    lieutenantBLUE = SDL_LoadBMP("lieutenantBLUE.bmp");
    captainRED = SDL_LoadBMP("captainRED.bmp");
    captainBLUE = SDL_LoadBMP("captainBLUE.bmp");
    majorRED = SDL_LoadBMP("majorRED.bmp");
    majorBLUE = SDL_LoadBMP("majorBLUE.bmp");
    colonelRED = SDL_LoadBMP("colonelRED.bmp");
    colonelBLUE = SDL_LoadBMP("colonelBLUE.bmp");
    generalRED = SDL_LoadBMP("generalRED.bmp");
    generalBLUE = SDL_LoadBMP("generalBLUE.bmp");
    marshalRED = SDL_LoadBMP("marshalRED.bmp");
    marshalBLUE = SDL_LoadBMP("marshalBLUE.bmp");
    flagRED = SDL_LoadBMP("flagRED.bmp");
    flagBLUE = SDL_LoadBMP("flagBLUE.bmp");


    // Effacement de l'écran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    for (i = 0 ; i < NB_BLOCS_COTE ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_COTE ; j++)
        {
            position.x = i * TAILLE_CASE;
            position.y = j * TAILLE_CASE;

            switch(gameState->board[i][j]->piece)
            {
                case EPbomb:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(bombRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(bombBLUE, NULL, ecran, &position);
                    break;
                case EPspy:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(spyRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(spyBLUE, NULL, ecran, &position);
                    break;
                case EPscout:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(scoutRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(scoutBLUE, NULL, ecran, &position);
                    break;
                case EPminer:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(minerRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(minerBLUE, NULL, ecran, &position);
                    break;
                case EPsergeant:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(sergeantRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(sergeantBLUE, NULL, ecran, &position);
                    break;
                case EPlieutenant:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(lieutenantRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(lieutenantBLUE, NULL, ecran, &position);
                    break;
                case EPcaptain:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(captainRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(captainBLUE, NULL, ecran, &position);
                    break;
                case EPmajor:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(majorRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(majorBLUE, NULL, ecran, &position);
                    break;
                case EPcolonel:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(colonelRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(colonelBLUE, NULL, ecran, &position);
                    break;
                case EPgeneral:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(generalRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(generalBLUE, NULL, ecran, &position);
                    break;
                case EPmarshal:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(marshalRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(marshalBLUE, NULL, ecran, &position);
                    break;
                case EPflag:
                    if (gameState->board[i][j]->content == ECred)
                        SDL_BlitSurface(flagRED, NULL, ecran, &position);
                    else
                        SDL_BlitSurface(flagBLUE, NULL, ecran, &position);
                    break;
            }
        }
    }

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
}