#ifdef USE_IMGUI
#include <imgui.h>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

#include "Scene/SceneGame.h"
#include "Framework.h"

#pragma region public function
Framework::Framework(HWND hwnd)
	: hwnd(hwnd)
	, sceneManager()
{
	// シーン初期化
	this->sceneManager.ChangeScene(new SceneGame);
}

Framework::~Framework()
{

}

int Framework::Run()
{
	MSG msg{};

	if (!Initialize())
	{
		return 0;
	}

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, immediate_context);
	ImGui::StyleColorsDark();
#endif

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			tictoc.Tick();
			CalculateFrameStats();
			Update(tictoc.TimeInterval());
			Render();
		}
	}

#ifdef USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

#if 0
	BOOL fullscreen = 0;
	swap_chain->GetFullscreenState(&fullscreen, 0);
	if (fullscreen)
	{
		swap_chain->SetFullscreenState(FALSE, 0);
	}
#endif

	return Finalize() ? static_cast<int>(msg.wParam) : 0;
}

#include <wingdi.h>

LRESULT Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
#ifdef USE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_CHAR:
	{

		break;
	}
	case WM_KEYDOWN:
	{
		// WM_KEYDOWNを受け取ると画面に文字を表示する
		{
			HDC hdc;
			hdc = GetDC(hwnd);
			wchar_t string[] = L"Hello, WM_KEYDOWN!";
			TextOut(hdc, 10, 10, string, 18);
			ReleaseDC(hwnd, hdc);
		}

		if (wparam == VK_ESCAPE)
		{
			PostMessage(hwnd, WM_CLOSE, 0, 0);
		}
		break;
	}
	case WM_ENTERSIZEMOVE:
	{
		tictoc.Stop();
		break;
	}
	case WM_EXITSIZEMOVE:
	{
		tictoc.Start();
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	}
	return 0;
}
#pragma endregion


#pragma region private function
bool Framework::Initialize()
{
	this->sceneManager.Initialize();

	return true;
}

bool Framework::Finalize()
{
	this->sceneManager.Finalize();


	return true;
}

void Framework::Update(const float elapsed_time)
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
	// シーン更新処理
	this->sceneManager.Update(elapsed_time);

#ifdef USE_IMGUI
	ImGui::Begin("ImGUI");


	ImGui::End();
#endif
}

void Framework::Render()
{


#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif


}

void Framework::CalculateFrameStats()
{
	if (++frames, (tictoc.TimeStamp() - elapsed_time) >= 1.0f)
	{
		float fps = static_cast<float>(frames);
		std::wostringstream outs;
		outs.precision(6);
		outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
		SetWindowTextW(hwnd, outs.str().c_str());

		frames = 0;
		elapsed_time += 1.0f;
	}
}
#pragma endregion