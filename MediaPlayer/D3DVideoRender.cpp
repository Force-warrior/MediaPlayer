#include "D3DVideoRender.h"


D3DVideoRender::D3DVideoRender() {

}

D3DVideoRender::~D3DVideoRender() {

}

int D3DVideoRender::initialization(void* hwnd, int w, int h, int flag) {
	m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pDirect3D9) {
		return -1;
	}

	D3DPRESENT_PARAMETERS m_PresentParam;
	memset(&m_PresentParam, 0, sizeof(D3DPRESENT_PARAMETERS));
	m_PresentParam.Windowed = TRUE;
	m_PresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_PresentParam.BackBufferFormat = D3DFMT_UNKNOWN;

	RECT m_rtViewport;
	GetClientRect((HWND)hwnd, &m_rtViewport);

	HRESULT ret = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&m_PresentParam, &m_pDirect3DDevice);
	if (FAILED(ret))
		return -1;
	
	ret = m_pDirect3DDevice->CreateOffscreenPlainSurface(w, h, (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'), D3DPOOL_DEFAULT, &m_pD3dSurface, NULL);
	if (FAILED(ret))
		return -1;


	FILE* pfile = nullptr;
	pfile = fopen("../third_party/res/yuv420p_320x180.yuv", "rb+");
	if (!pfile) {
		return -1;
	}
	const int bpp = 12;
	const int pixel_w = 320;
	const int pixel_h = 180;
	unsigned char buffer[pixel_w*pixel_h*bpp / 8];
	while (true) {
		if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, pfile) != pixel_w * pixel_h*bpp / 8) {
			fseek(pfile, 0, SEEK_SET);
			fread(buffer, 1, pixel_w*pixel_h*bpp / 8, pfile);
		}
		if (m_pD3dSurface == nullptr) {
			break;
		}

		D3DLOCKED_RECT d3dRect;
		HRESULT lRet = m_pD3dSurface->LockRect(&d3dRect, NULL, D3DLOCK_DONOTWAIT);
		if (FAILED(lRet))
			break;
		byte *pSrc = buffer;
		byte * pDest = (BYTE *)d3dRect.pBits;
		int stride = d3dRect.Pitch;
		unsigned long i = 0;


		for (i = 0; i < pixel_h; i++) {
			memcpy(pDest + i * stride, pSrc + i * pixel_w, pixel_w);
		}
		for (i = 0; i < pixel_h / 2; i++) {
			memcpy(pDest + stride * pixel_h + i * stride / 2, pSrc + pixel_w * pixel_h + pixel_w * pixel_h / 4 + i * pixel_w / 2, pixel_w / 2);
		}
		for (i = 0; i < pixel_h / 2; i++) {
			memcpy(pDest + stride * pixel_h + stride * pixel_h / 4 + i * stride / 2, pSrc + pixel_w * pixel_h + i * pixel_w / 2, pixel_w / 2);
		}

		lRet = m_pD3dSurface->UnlockRect();
		if (FAILED(lRet))
			break;

		if (!m_pDirect3DDevice)
			break;

		m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		m_pDirect3DDevice->BeginScene();
		IDirect3DSurface9 * pBackBuffer = NULL;

		m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		m_pDirect3DDevice->StretchRect(m_pD3dSurface, NULL, pBackBuffer, &m_rtViewport, D3DTEXF_LINEAR);
		m_pDirect3DDevice->EndScene();
		m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
		pBackBuffer->Release();
		Sleep(40);
	}


	return 0;
}

int D3DVideoRender::release() {
	return 0;
}

int D3DVideoRender::renderFrame() {
	return 0;
}

void D3DVideoRender::ThreadRender(std::shared_ptr<RenderThreadData> data_ptr) {
	

	return ;
}
