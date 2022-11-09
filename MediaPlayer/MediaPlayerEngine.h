#pragma once
#include <Windows.h>
class MediaPlayerEngine{
public:
	MediaPlayerEngine();
	~MediaPlayerEngine();
	static MediaPlayerEngine* getEngine();
public:


public:
	LRESULT CALLBACK WndProc(HWND hWnd, HINSTANCE hInst, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND hVideoRender;
	HWND hAudioStart;
	HWND hAudioStop;
	HWND hAudioPause;
};

