#include "Game.h"

int main(int,char**)
{
	//Create the Game
	Game theGame;

	//Initialize the game
	theGame.InitGame();

	//Runs the game until it returns :)
	return theGame.Run();
}