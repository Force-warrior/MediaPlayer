#pragma once
#ifndef SDL_VIDEORENDER_H_
#define SDL_VIDEORENDER_H_

#include "IVideoRender.h"

#include <memory>
#include <thread>
#include <mutex>

#include "sdl/SDL.h"
class SDLVideoRender :
	public IVideoRender{
public:
	SDLVideoRender();
	virtual ~SDLVideoRender();
	virtual int initialization(void* hwnd, int w, int h, int flag) override;
	virtual int release()override;
	virtual int renderFrame() override;
private:
	struct RenderThreadData{
		RenderThreadData(void* hwnd, int flag) {
			this->hWnd = hwnd;
			this->window_w = 0;
			this->window_h = 0;
			this->flags = flag;
			exit_thread = false;
		}
		void *hWnd;
		int window_w;
		int window_h;
		Uint32 flags;
		bool exit_thread;

		std::mutex mutex_renderdata;

	};

	static void ThreadRender(std::shared_ptr<RenderThreadData> data_ptr);

private:
	std::thread *mRenderThread;
	std::shared_ptr<RenderThreadData> mRenderThreadData;

	bool sdl_init = false;
};

#endif // !SDL_VIDEORENDER_H_

