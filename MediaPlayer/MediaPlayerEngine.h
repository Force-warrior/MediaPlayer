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
//sdl
	HWND hSdlVideoRender;
	HWND hSdlAudioStart;
	HWND hSdlAudioStop;
	HWND hSdlAudioPause;

// d3d
	HWND hD3dVideoRender;
	HWND hD3dAudioStart;
	HWND hD3dAudioStop;
	HWND hD3dAudioPause;
};

