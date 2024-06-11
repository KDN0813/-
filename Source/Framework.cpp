#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Framework.h"

// êÇíºìØä˙ä‘äuê›íË
static const int syncInterval = 1;

Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, graphics(hWnd)
{
	SceneManager::Instance().ChangeScene(new SceneGame);
}

Framework::~Framework()
{
	SceneManager::Instance().Clear();
}

void Framework::Update(float elapsed_time)
{
	SceneManager::Instance().Update(elapsed_time);
}

void Framework::Render(float elapsed_time)
{
	std::lock_guard<std::mutex> lock(graphics.GetMutex());

	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	SceneManager::Instance().Render();

	graphics.GetSwapChain()->Present(syncInterval, 0);
}

bool IsWindowActive(HWND hwnd) 
{
	return (GetForegroundWindow() == hwnd);
}

int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!IsWindowActive(hWnd)) continue;

			timer.Tick();

			float elapsed_time = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / 60.0f
				;
			Update(elapsed_time);
			Render(elapsed_time);
		}
	}
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
