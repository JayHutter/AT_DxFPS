#include "Window.h"
#include "Game.h"

//Entry Point
int CALLBACK WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    try
    {
        return Game{}.Run();
    }
    catch (const Exception& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Stand Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "No information available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}

