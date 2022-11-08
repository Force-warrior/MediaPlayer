#include "MediaPlayerEngine.h"
#include "define.h"
#include "sdl/SDL.h"

MediaPlayerEngine::MediaPlayerEngine() {

}
MediaPlayerEngine::~MediaPlayerEngine() {

}
MediaPlayerEngine* MediaPlayerEngine::getEngine() {
	static MediaPlayerEngine obj;
	return &obj;
}

void onSDLVideo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

}

void onSDLAudio(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
}

LRESULT CALLBACK MediaPlayerEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��:
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
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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