#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"	// [13]

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	// エネミーHP描画[13]
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc
		, const DirectX::XMFLOAT4X4& view
		, const DirectX::XMFLOAT4X4& projection
	);

private:
	CameraController* cameraController	= nullptr;
	DirectX::XMFLOAT3 target{};

	// [13]
	Sprite* gauge						= nullptr;
};
