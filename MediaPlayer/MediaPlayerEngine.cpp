#include "MediaPlayerEngine.h"

#include <stdio.h>

#include "define.h"
#include "sdl/SDL.h"

#define REFRESH_EVENT (SDL_USEREVENT+1)
#define BREAK_EVENT (SDL_USEREVENT+2)

MediaPlayerEngine::MediaPlayerEngine() {

}
MediaPlayerEngine::~MediaPlayerEngine() {

}
MediaPlayerEngine* MediaPlayerEngine::getEngine() {
	static MediaPlayerEngine obj;
	return &obj;
}

int screen_w = 500, screen_h = 500;
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

	SDL_Window* sdl_w = SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
			return 0;
		}
			
	}

	return 0;
}

void onSDLAudio(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

}

LRESULT CALLBACK MediaPlayerEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDB_SDL_AUDIO:
			onSDLAudio(hWnd, message, wParam, lParam);
			break;
		case IDB_SDL_VIDEO:
			onSDLVideo(hWnd, message, wParam, lParam);
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
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
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