#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Camera.h"

#include "EffectManager.h"
#include "StageManager.h"
#include "StageMoveFloor.h"

#include "Input/Input.h"

// 初期化[01]
void SceneGame::Initialize()
{
	// ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	this->stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(.0f, 1.0f, 3.0f));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10.0f, 2.0f, 3.0f));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(.0f, 1.0f, .0f));
	stageManager.Register(stageMoveFloor);

	player = new Player();

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

	// エネミー初期化[04,05]
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 10.0f, 0.0f, 10.0f));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
	    enemyManager.Register(slime);
	}

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

	// エネミー終了化
	EnemyManager::Instance().Clear();

	// プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

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
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;	// プレイヤーの腰当たりをターゲットに設定
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	// ステージ更新処理
	StageManager::Instance().Update(elapsedTime);
	// プレイヤー更新処理
	player->Update(elapsedTime);

	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	// エフェクト更新処理[10]
	EffectManager::Instance().Update(elapsedTime);
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
		// プレイヤー描画
		player->Render(dc, shader);
		// エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		// プレイヤーデバックプリミティブ描画[05]
		player->DarwDebugPrimitive();

		// エネミーデバックプリミティブ描画[05]
		EnemyManager::Instance().DrawDebugPrimitive();

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

		// プレイヤーデバッグ描画
		player->DrawDebugGUI();
		// カメラコントローラーデバッグ描画
		cameraController->DrawDebugGUI();
		// エネミーデバッグ描画
		EnemyManager::Instance().DrawDebugGUI();

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

	// 全ての敵の頭上にHPゲージを表示[13]
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeigh();									// 敵の頭に描画したいので、敵の頭の位置をスクリーン座標に変換する

		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		// ワールド座標からスクリーン座標へ変換[13]
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition														// ワールド座標
			, viewport.TopLeftX													// ビューポート左上野X位置
			, viewport.TopLeftY													// ビューポート左上野X位置
			, viewport.Width													// ビューポート幅
			, viewport.Height													// ビューポート高さ
			, viewport.MinDepth													// 深度値の最小値
			, viewport.MaxDepth													// 深度値の最大値
			, Projection														// プロジェクション行列
			, View																// ビュー行列
			, world																// ワールド行列(単位行列)
		);

		// スクリーン座標
		DirectX::XMFLOAT3 screenPosition;										// スクリーン座標(float3)
		DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);
		if(screenPosition.z < 0.0f || screenPosition.z > 1.0f) continue;
		
		// ゲージの長さ
		float textureWidth	= 50.0f;	// 画像幅
		float textureHeight = 10.0f;	// 画像高さ

		// ゲージの割合
		float health	= static_cast<float>(enemy->GetHealth());
		float Maxhealth = static_cast<float>(enemy->GetMaxHealth());
		float length	= health / Maxhealth;									// 描画の割合

		// 描画位置
		float positionX = screenPosition.x - textureWidth * 0.5f;				// 描画位置X
		float positionY = screenPosition.y - textureWidth;						// 描画位置Y

		// 描画[13]
		gauge->Render(dc
			, positionX, positionY												// 座標
			, textureWidth * length, textureHeight								// 描画する画像の幅・高さ
			, 0, 0																// 画像の切り抜き位置
			, static_cast<float>(gauge->GetTextureWidth())						// 切り抜きサイズ
			, static_cast<float>(gauge->GetTextureWidth())						
			, 0																	// 角度
			, 1, 0, 0, 1														// 色
		);
	}

	// エネミー配置処理[13]
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		// マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPositon;
		screenPositon.x = static_cast<float>(mouse.GetPositionX());
		screenPositon.y = static_cast<float>(mouse.GetPositionY());
		screenPositon.z = 0.0f;
	
		DirectX::XMVECTOR WorldPositionZNear{};
		WorldPositionZNear = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPositon)								// スクリーン座標
			, viewport.TopLeftX													// ビューポート左上野X位置
			, viewport.TopLeftY													// ビューポート左上野X位置
			, viewport.Width													// ビューポート幅
			, viewport.Height													// ビューポート高さ
			, viewport.MinDepth													// 深度値の最小値
			, viewport.MaxDepth													// 深度値の最大値
			, Projection														// プロジェクション行列
			, View																// ビュー行列
			, world																// ワールド行列(単位行列)
		);
		DirectX::XMFLOAT3 worldPositionZNear;
		DirectX::XMStoreFloat3(&worldPositionZNear, WorldPositionZNear);
	
	
		DirectX::XMVECTOR WorldPositionZFar{};
		screenPositon.z = 1.0f;
		WorldPositionZFar = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPositon)								// スクリーン座標
			, viewport.TopLeftX													// ビューポート左上野X位置
			, viewport.TopLeftY													// ビューポート左上野X位置
			, viewport.Width													// ビューポート幅
			, viewport.Height													// ビューポート高さ
			, viewport.MinDepth													// 深度値の最小値
			, viewport.MaxDepth													// 深度値の最大値
			, Projection														// プロジェクション行列
			, View																// ビュー行列
			, world																// ワールド行列(単位行列)
		);
		DirectX::XMFLOAT3 worldPositionZFar;
		DirectX::XMStoreFloat3(&worldPositionZFar, WorldPositionZFar);
	
		HitResult hit;
		if (stageMain->RayCact(worldPositionZNear, worldPositionZFar, hit))
		{
			Enemy* enemy = new EnemySlime();
			enemy->SetPosition(hit.position);
			EnemyManager::Instance().Register(enemy);
		}
	}
}