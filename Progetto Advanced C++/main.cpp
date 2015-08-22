#include "MyDirectXWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MyDirectXWindow window(hInstance, nCmdShow);
	return window.run();
}