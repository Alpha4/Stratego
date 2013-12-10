#include "stratego.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////
// Dans la librairie
//

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Fabien Picarougne");
}

void StartMatch(const EColor color)
{
	printf("StartMatch\n");
}

void StartGame(EPiece boardInit[4][10])
{
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

void NextMove(const SGameState * const gameState, SMove *move)
{
	printf("NextMove\n");
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	printf("AttackResult\n");
}

void void Penalty()
{
	printf("Penalty\n");
}

//////////////////////////////////////////////////////////
// Dans l'exécutable
//

void deroulement_du_jeu()	
//int main()
{
	//*****// à chaque utilisation de gameState, ne pas oublier de faire une copie de tous les éléments (pas fait ici)
	SGameState gameState;
	// init de l'état de départ

	//*****// à faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions décrites dans "backgammon.h")
	// -> à remplacer par votre code de chargement
	pfInitLibrary j1InitLibrary=InitLibrary;
	pfStartMatch j1StartMatch=StartMatch;
	pfStartGame j1StartGame=StartGame;
	pfEndGame j1EndGame=EndGame;
	pfEndMatch j1EndMatch=EndMatch;
	pfNextMove j1NextMove=NextMove;
	pfAttackResult j1AttackResult=AttackResult;
	// ...
	pfNextMove j2NextMove=NextMove;
	pfAttackResult j2AttackResult=AttackResult;

	// Initialisation de la librairie
	SMove move;
	char name[50];
	j1InitLibrary(name);
	j1StartMatch();

		//*****// à faire pour chaque jeu
		EPiece boardInit[4][10];
		j1StartGame(ECred,boardInit);
		//...
			//*****// pour chaque joueur, tant que ce n'est pas fini ; ne pas oublier de dupliquer gameState
			move=j1NextMove(&gameState);
			// ...

			//*****// si attaque, il faut signaler les données à tous les joueurs
			SPos p1,p2;
			j1AttackResult(p1,EPmarshal,p2,EPbomb);
			j2AttackResult(p2,EPbomb,p1,EPmarshal);
			// ...
				
		j1EndGame();

	j1EndMatch();

	return(0);
}
