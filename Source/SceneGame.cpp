#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Graphics/RenderContext.h"

void SceneGame::Initialize()
{
}

void SceneGame::Finalize()
{
}

void SceneGame::Update(float elapsedTime)
{
}

void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	RenderContext rc;

	// 3D���f���`��
	{
	}

	// 3D�G�t�F�N�g�`��
	{
	}

	// 3D�f�o�b�O�`��
	{
	}


	// 2D�f�o�b�OGUI�`��
	{
	}
}