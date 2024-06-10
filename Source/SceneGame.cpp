#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Camera.h"

#include "StageManager.h"

// 初期化[01]
void SceneGame::Initialize()
{
	// ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	this->stageMain = new StageMain();
	stageManager.Register(stageMain);

	// カメラ初期設定[02]
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Intance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// カメラコントローラー初期化[02]
	cameraController = new CameraController();

	// ゲージスプライト[13]
	gauge = new Sprite();
}

// 終了化
void SceneGame::Finalize()
{
	// ゲージスプライト終了化[13]
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	// ステージ終了化
	StageManager::Instance().Clear();

	// カメラコントローラー終了か
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	// カメラコントローラー更新処理
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	// ステージ更新処理
	StageManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 1.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	// カメラのパラメータ設定
	Camera& camera = Camera::Intance();
	rc.view = camera.GetView();
	rc.projection = camera.getProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();

		shader->Begin(dc, rc);

		// ステージ描画
		StageManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3Dエフェクト描画
	{
	}

	// 3Dデバッグ描画
	{

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		// HPゲージ描画[13]
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.4f, 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);

		// カメラコントローラーデバッグ描画
		cameraController->DrawDebugGUI();

		if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None))
		{
			ImGui::DragFloat3("target", &target.x, 0.01f);
		}
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}

// エネミーHP描画[13]
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc
	, const DirectX::XMFLOAT4X4& view
	, const DirectX::XMFLOAT4X4& projection
)
{
	// ビューポート[13]
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	// ラスタライザーステージにバインドされたビューポートの配列を取得
	dc->RSGetViewports(
		&numViewports															// ビューポートの数へのポインタ
		, &viewport																// D3D11_VIEWPORTの配列
	);

	// 変換行列[13]
	DirectX::XMMATRIX View			= DirectX::XMLoadFloat4x4(&view);			// ビュー行列
	DirectX::XMMATRIX Projection	= DirectX::XMLoadFloat4x4(&projection);		// プロジェクション行列
	DirectX::XMMATRIX world			= DirectX::XMMatrixIdentity();				// ワールド行列(ID行列)
}