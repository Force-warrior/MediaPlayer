#pragma once
#include <Windows.h>
class MediaPlayerEngine{
public:
	MediaPlayerEngine();
	~MediaPlayerEngine();
	static MediaPlayerEngine* getEngine();
public:


public:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

