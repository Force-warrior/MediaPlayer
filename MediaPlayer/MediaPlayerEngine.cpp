#include "MediaPlayerEngine.h"

#include <stdio.h>

#include "define.h"
#include "sdl/SDL.h"

#include "SDLVideoRender.h"
#include "D3DVideoRender.h"

#define REFRESH_EVENT (SDL_USEREVENT+1)
#define BREAK_EVENT (SDL_USEREVENT+2)
#define QUIT_EVENT (SDL_USEREVENT+3)

MediaPlayerEngine::MediaPlayerEngine() {

}
MediaPlayerEngine::~MediaPlayerEngine() {

}
MediaPlayerEngine* MediaPlayerEngine::getEngine() {
	static MediaPlayerEngine obj;
	return &obj;
}

int screen_w = 320, screen_h = 240;

static SDLVideoRender sdlrender;
static D3DVideoRender d3drender;

LRESULT CALLBACK MediaPlayerEngine::WndProc(HWND hWnd, HINSTANCE hInst, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDB_SDL_AUDIO:
			hSdlAudioStart = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Start"), WS_VISIBLE | WS_CHILD,
				150, 260, 120, 30, hWnd, (HMENU)IDB_SDL_AUDIO_START, hInst, 0);
			hSdlAudioStop = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Stop"), WS_VISIBLE | WS_CHILD,
				300, 260, 120, 30, hWnd, (HMENU)IDB_SDL_AUDIO_STOP, hInst, 0);
			hSdlAudioPause = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Pause"), WS_VISIBLE | WS_CHILD,
				450, 260, 120, 30, hWnd, (HMENU)IDB_SDL_AUDIO_PAUSE, hInst, 0);

			break;
		case IDB_SDL_VIDEO:
			hSdlVideoRender = CreateWindowEx(0, TEXT("BUTTON"), TEXT("SDL PlayVideo"), WS_VISIBLE | WS_CHILD,
				150, 10, screen_w, screen_h, hWnd, (HMENU)IDB_SDL_VIDEO_AREA, hInst, 0);
			sdlrender.initialization(hSdlVideoRender, 320, 180, 1);
			break;

		case IDB_D3D_AUDIO:
			break;
		case IDB_D3D_VIDEO:
			hD3dVideoRender = CreateWindowEx(0, TEXT("BUTTON"), TEXT("SDL PlayVideo"), WS_VISIBLE | WS_CHILD,
				750, 10, screen_w, screen_h, hWnd, (HMENU)IDB_D3D_VIDEO_AREA, hInst, 0);
			d3drender.initialization(hD3dVideoRender, 320, 180, 1);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代sdlrender码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}