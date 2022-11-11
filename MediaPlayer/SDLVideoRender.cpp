#include "SDLVideoRender.h"
#ifdef WIN32
#include <Windows.h>
#endif // WIN32


SDLVideoRender::SDLVideoRender() {

}

SDLVideoRender::~SDLVideoRender() {

}

int SDLVideoRender::initialization(void* hwnd, int w, int h, int flag) {
	if (sdl_init) return 0;

	if (!hwnd) {
		return -1;
	}

	mRenderThreadData = std::make_shared<RenderThreadData>(hwnd, flag);
	mRenderThreadData->window_h = h;
	mRenderThreadData->window_w = w;
	

	if (SDL_Init(SDL_INIT_VIDEO)) {
		return -1;
	}

	mRenderThread = new std::thread(ThreadRender, mRenderThreadData);
	if (!mRenderThread)
		return -1;
	sdl_init = true;
	return 0;
}

int SDLVideoRender::release() {
	if (!sdl_init) return 0;

	if (mRenderThread) {
		if (!!mRenderThreadData) {
			{
				std::unique_lock<std::mutex> g_lock(mRenderThreadData->mutex_renderdata);
				mRenderThreadData->exit_thread = true;
			}
			mRenderThreadData = nullptr;
		}

		mRenderThread->detach();
	}

	sdl_init = false;
	return 0;
}

void createVitualData() {

}

int SDLVideoRender::renderFrame() {
	if (!sdl_init) return -1;



	return 0;
}


void SDLVideoRender::ThreadRender(std::shared_ptr<RenderThreadData> data_ptr) {

	SDL_Window* sdlWindow = nullptr;
	SDL_Renderer* sdlRender = nullptr;
	SDL_Texture* sdlTexture = nullptr;
	void* mhWnd = nullptr;
	int mWindowWith = 0;
	int mWindowHeight = 0;

	SDL_Rect sdlRect;

	{
		if (data_ptr->exit_thread)
			return;
		mhWnd = data_ptr->hWnd;
		mWindowWith = data_ptr->window_w;
		mWindowHeight = data_ptr->window_h;
	}

	const int mPicWidth = 320;
	const int mPicHeight = 180;

	const int bpp = 12;
	unsigned char buffer[mPicWidth*mPicHeight*bpp / 8];

	FILE* file = nullptr;
	file = fopen("../third_party/res/yuv420p_320x180.yuv", "rb+");
	if (!file) {
		return ;
	}

	while (true) {
		if (!sdlWindow) {
#ifdef WIN32
			if (IsWindowVisible((HWND)mhWnd)) {
#else	
			if (mhWnd) {
#endif // WIN32
				RECT rect;
				GetClientRect((HWND)mhWnd, &rect);
				mWindowWith = rect.right - rect.left;
				mWindowHeight = rect.bottom - rect.top;
				sdlWindow = SDL_CreateWindowFrom(mhWnd);
			}
			else {
				sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindowWith, mWindowHeight, SDL_WINDOW_BORDERLESS);
			}
			if (!sdlWindow) {
				break;
			}
		}
		if (!sdlRender) {
			sdlRender = SDL_CreateRenderer(sdlWindow, -1, 0);
			if (!sdlRender) {
				break;
			}
		}

		if (!sdlTexture) {
			sdlTexture = SDL_CreateTexture(sdlRender, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, mPicWidth, mPicHeight);
			if (!sdlTexture) {
				break;
			}
		}

		if (fread(buffer, 1, mPicWidth*mPicHeight*bpp / 8, file) != mPicWidth * mPicHeight*bpp / 8) {
			// Loop
			fseek(file, 0, SEEK_SET);
			fread(buffer, 1, mPicWidth*mPicHeight*bpp / 8, file);
		}

		sdlRect.x = 0;
		sdlRect.y = 0;
		sdlRect.w = mWindowWith;
		sdlRect.h = mWindowHeight;

		SDL_UpdateTexture(sdlTexture, NULL, buffer, mPicWidth);

		SDL_RenderClear(sdlRender);
		SDL_RenderCopy(sdlRender, sdlTexture, NULL, &sdlRect);
		SDL_RenderPresent(sdlRender);
		//SDL_Delay(40);

	}

	if (sdlTexture) {
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = nullptr;
	}

	if (sdlRender) {
		SDL_DestroyRenderer(sdlRender);
		sdlRender = nullptr;
	}

	if (sdlWindow) {
		SDL_DestroyWindow(sdlWindow);
		sdlWindow = nullptr;
	}

}
