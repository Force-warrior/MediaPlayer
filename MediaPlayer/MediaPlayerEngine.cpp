#include "MediaPlayerEngine.h"

#include <stdio.h>

#include "define.h"
#include "sdl/SDL.h"

#include "SDLVideoRender.h"

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
const int pixel_w = 320, pixel_h = 180;
const int bpp = 32;
unsigned char buffer[pixel_w*pixel_h*bpp / 8];

int thread_exit = 0;

int refreshvideo(void *data) {
	thread_exit = 0;
	while (!thread_exit) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	thread_exit = 0;
	//Break
	SDL_Event event;
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);
	return 0;
}

int onSDLVideo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (SDL_Init(SDL_INIT_VIDEO)){
		return -1;
	}

	SDL_Window* sdl_w;
	if (IsWindowVisible(hWnd)) {
		sdl_w = SDL_CreateWindowFrom(hWnd);
	}
	else {
		sdl_w = SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_BORDERLESS);
	}
	if (!sdl_w) {
		return -1;
	}

	SDL_Renderer* sdl_render = SDL_CreateRenderer(sdl_w, -1, 0);
	if (!sdl_render) {
		return -1;
	}

	SDL_Texture* sdl_texture = SDL_CreateTexture(sdl_render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);
	if (!sdl_texture) {
		return -1;
	}

	FILE* file = nullptr;
	file = fopen("../third_party/res/bgra_320x180.rgb", "rb+");
	if (!file) {
		return -1;
	}

	SDL_Thread *sdlthread = SDL_CreateThread(refreshvideo, NULL, NULL);
	SDL_Event sdl_event;

	SDL_Rect sdlRect;
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = screen_w;
	sdlRect.h = screen_h;

	while (1) {
		SDL_WaitEvent(&sdl_event);
		switch (sdl_event.type) {
		case REFRESH_EVENT:
			if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, file) != pixel_w * pixel_h*bpp / 8) {
				// Loop
				fseek(file, 0, SEEK_SET);
				fread(buffer, 1, pixel_w*pixel_h*bpp / 8, file);
			}
			SDL_UpdateTexture(sdl_texture,NULL , buffer, pixel_w*4);
			SDL_RenderClear(sdl_render);
			SDL_RenderCopy(sdl_render, sdl_texture, NULL, &sdlRect);
			SDL_RenderPresent(sdl_render);
			break;
		case BREAK_EVENT:
			break;
		case SDL_QUIT:
			thread_exit = 1;
			break;
		}
		
		if (thread_exit) {
			SDL_Quit();
			break;
		}
			
	}

	return 0;
}

int onSDLAudio(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (SDL_Init(SDL_INIT_AUDIO)) {
		return -1;
	}
}

static SDLVideoRender sdlrender;

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
			hAudioStart = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Start"), WS_VISIBLE | WS_CHILD,
				150, 260, 120, 30, hWnd, (HMENU)IDB_SDL_VIDEO, hInst, 0);
			hAudioStop = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Stop"), WS_VISIBLE | WS_CHILD,
				300, 260, 120, 30, hWnd, (HMENU)IDB_SDL_VIDEO, hInst, 0);
			hAudioPause = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Pause"), WS_VISIBLE | WS_CHILD,
				450, 260, 120, 30, hWnd, (HMENU)IDB_SDL_VIDEO, hInst, 0);

			onSDLAudio(hWnd, message, wParam, lParam);
			break;
		case IDB_SDL_VIDEO:
			hVideoRender = CreateWindowEx(0, TEXT("BUTTON"), TEXT("SDL PlayVideo"), WS_VISIBLE | WS_CHILD,
				150, 10, screen_w, screen_h, hWnd, (HMENU)IDB_SDL_VIDEO, hInst, 0);
			//onSDLVideo(hVideoRender, message, wParam, lParam);
			
			sdlrender.initialization(hVideoRender, 320, 180, 1);
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