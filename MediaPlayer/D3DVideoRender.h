#pragma once
#include "IVideoRender.h"
#include <memory>
#include <thread>
#include <mutex>
#include <d3d9.h>


class D3DVideoRender :
	public IVideoRender{

public:
	D3DVideoRender();
	virtual ~D3DVideoRender();
	virtual int initialization(void* hwnd, int w, int h, int flag) override;
	virtual int release()override;
	virtual int renderFrame() override;
private:
	struct RenderThreadData {
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
		int flags;
		bool exit_thread;

		std::mutex mutex_renderdata;

	};

	static void ThreadRender(std::shared_ptr<RenderThreadData> data_ptr);

private:
	std::thread *mRenderThread;
	std::shared_ptr<RenderThreadData> mRenderThreadData;
	IDirect3D9 *m_pDirect3D9;
	IDirect3DDevice9* m_pDirect3DDevice;
	IDirect3DSurface9* m_pD3dSurface;
	bool d3d_init = false;

};

