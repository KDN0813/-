#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"	// [13]
#include "StageMain.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	// �G�l�~�[HP�`��[13]
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc
		, const DirectX::XMFLOAT4X4& view
		, const DirectX::XMFLOAT4X4& projection
	);

private:
	Player* player						= nullptr;
	CameraController* cameraController	= nullptr;

	// [13]
	Sprite* gauge						= nullptr;
	StageMain* stageMain				= nullptr;
};
