#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include "Misc.h"
#include "HighResolutionTimer.h"

#include "Scene/SceneManager.h"


CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"MY_GAME" };
CONST LPCWSTR WINDOW_NAME{ L"GAME_TITLE" };

class Framework
{
public:
	Framework(HWND hwnd);
	~Framework();

	// �R�s�[�E���[�u�R���X�g���N�^(������Z�q)�̋֎~
	Framework(const Framework&) = delete;
	Framework& operator=(const Framework&) = delete;
	Framework(Framework&&) noexcept = delete;
	Framework& operator=(Framework&&) noexcept = delete;
public:
	int Run();
	// ���b�Z�[�W�n���h��
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	bool Initialize();
	bool Finalize();
	void Update(const float elapsed_time);
	void Render();

	// �t���[�����[�g�v�Z
	void CalculateFrameStats();
private:
	const HWND hwnd;
	HighResolutionTimer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };

private:
	SceneManager sceneManager;
};

