#include "main.h"
#include "Classes/Puzzles.h"
#include "cocos2d.h"

USING_NS_CC;

//win32��������,����Ҫ�����޸�//
int WINAPI _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    Puzzles app;
    return Application::getInstance()->run();
}