#include "GamePCH.h"
#include "Game.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    fw::FWCore* pFramework = new fw::FWCore();
    pFramework->Init(fw::k_Window_Dimensions.x, fw::k_Window_Dimensions.y);

    Game* pGame = new Game( pFramework );
    
    pGame->Init();

    pFramework->Run( pGame );
    pFramework->Shutdown();

    delete pFramework;
    delete pGame;

    MyMemory_ValidateAllocations(true);

    return 0;
}
