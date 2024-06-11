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

	// 3Dモデル描画
	{
	}

	// 3Dエフェクト描画
	{
	}

	// 3Dデバッグ描画
	{
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}


	// 2DデバッグGUI描画
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.4f, 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}