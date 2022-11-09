#pragma once
#ifndef I_VIDEO_RENDER_H_
#define I_VIDEO_RENDER_H_




class IVideoRender {
public:
	virtual ~IVideoRender() {}
	virtual int initialization(void* hwnd, int w, int h, int flag) = 0;
	virtual int release() = 0;
	virtual int renderFrame() = 0;

};

IVideoRender* createVideoRender();
void releaseVideoRender();


#endif // !1

