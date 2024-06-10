#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Camera.h"

#include "EffectManager.h"
#include "StageManager.h"
#include "StageMoveFloor.h"

#include "Input/Input.h"

// ������[01]
void SceneGame::Initialize()
{
	// �X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	this->stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(.0f, 1.0f, 3.0f));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10.0f, 2.0f, 3.0f));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(.0f, 1.0f, .0f));
	stageManager.Register(stageMoveFloor);

	player = new Player();

	// �J���������ݒ�[02]
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

	// �J�����R���g���[���[������[02]
	cameraController = new CameraController();

	// �G�l�~�[������[04,05]
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 10.0f, 0.0f, 10.0f));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
	    enemyManager.Register(slime);
	}

	// �Q�[�W�X�v���C�g[13]
	gauge = new Sprite();
}

// �I����
void SceneGame::Finalize()
{
	// �Q�[�W�X�v���C�g�I����[13]
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	// �X�e�[�W�I����
	StageManager::Instance().Clear();

	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	// �v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	// �J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	// �J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;	// �v���C���[�̍���������^�[�Q�b�g�ɐݒ�
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);
	// �v���C���[�X�V����
	player->Update(elapsedTime);

	// �G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	// �G�t�F�N�g�X�V����[10]
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 1.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����̃p�����[�^�ݒ�
	Camera& camera = Camera::Intance();
	rc.view = camera.GetView();
	rc.projection = camera.getProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();

		shader->Begin(dc, rc);

		// �X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);
		// �v���C���[�`��
		player->Render(dc, shader);
		// �G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		// �v���C���[�f�o�b�N�v���~�e�B�u�`��[05]
		player->DarwDebugPrimitive();

		// �G�l�~�[�f�o�b�N�v���~�e�B�u�`��[05]
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		// HP�Q�[�W�`��[13]
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.4f, 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);

		// �v���C���[�f�o�b�O�`��
		player->DrawDebugGUI();
		// �J�����R���g���[���[�f�o�b�O�`��
		cameraController->DrawDebugGUI();
		// �G�l�~�[�f�o�b�O�`��
		EnemyManager::Instance().DrawDebugGUI();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}

// �G�l�~�[HP�`��[13]
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc
	, const DirectX::XMFLOAT4X4& view
	, const DirectX::XMFLOAT4X4& projection
)
{
	// �r���[�|�[�g[13]
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	// ���X�^���C�U�[�X�e�[�W�Ƀo�C���h���ꂽ�r���[�|�[�g�̔z����擾
	dc->RSGetViewports(
		&numViewports															// �r���[�|�[�g�̐��ւ̃|�C���^
		, &viewport																// D3D11_VIEWPORT�̔z��
	);

	// �ϊ��s��[13]
	DirectX::XMMATRIX View			= DirectX::XMLoadFloat4x4(&view);			// �r���[�s��
	DirectX::XMMATRIX Projection	= DirectX::XMLoadFloat4x4(&projection);		// �v���W�F�N�V�����s��
	DirectX::XMMATRIX world			= DirectX::XMMatrixIdentity();				// ���[���h�s��(ID�s��)

	// �S�Ă̓G�̓����HP�Q�[�W��\��[13]
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeigh();									// �G�̓��ɕ`�悵�����̂ŁA�G�̓��̈ʒu���X�N���[�����W�ɕϊ�����

		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		// ���[���h���W����X�N���[�����W�֕ϊ�[13]
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition														// ���[���h���W
			, viewport.TopLeftX													// �r���[�|�[�g�����X�ʒu
			, viewport.TopLeftY													// �r���[�|�[�g�����X�ʒu
			, viewport.Width													// �r���[�|�[�g��
			, viewport.Height													// �r���[�|�[�g����
			, viewport.MinDepth													// �[�x�l�̍ŏ��l
			, viewport.MaxDepth													// �[�x�l�̍ő�l
			, Projection														// �v���W�F�N�V�����s��
			, View																// �r���[�s��
			, world																// ���[���h�s��(�P�ʍs��)
		);

		// �X�N���[�����W
		DirectX::XMFLOAT3 screenPosition;										// �X�N���[�����W(float3)
		DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);
		if(screenPosition.z < 0.0f || screenPosition.z > 1.0f) continue;
		
		// �Q�[�W�̒���
		float textureWidth	= 50.0f;	// �摜��
		float textureHeight = 10.0f;	// �摜����

		// �Q�[�W�̊���
		float health	= static_cast<float>(enemy->GetHealth());
		float Maxhealth = static_cast<float>(enemy->GetMaxHealth());
		float length	= health / Maxhealth;									// �`��̊���

		// �`��ʒu
		float positionX = screenPosition.x - textureWidth * 0.5f;				// �`��ʒuX
		float positionY = screenPosition.y - textureWidth;						// �`��ʒuY

		// �`��[13]
		gauge->Render(dc
			, positionX, positionY												// ���W
			, textureWidth * length, textureHeight								// �`�悷��摜�̕��E����
			, 0, 0																// �摜�̐؂蔲���ʒu
			, static_cast<float>(gauge->GetTextureWidth())						// �؂蔲���T�C�Y
			, static_cast<float>(gauge->GetTextureWidth())						
			, 0																	// �p�x
			, 1, 0, 0, 1														// �F
		);
	}

	// �G�l�~�[�z�u����[13]
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		// �}�E�X�J�[�\�����W���擾
		DirectX::XMFLOAT3 screenPositon;
		screenPositon.x = static_cast<float>(mouse.GetPositionX());
		screenPositon.y = static_cast<float>(mouse.GetPositionY());
		screenPositon.z = 0.0f;
	
		DirectX::XMVECTOR WorldPositionZNear{};
		WorldPositionZNear = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPositon)								// �X�N���[�����W
			, viewport.TopLeftX													// �r���[�|�[�g�����X�ʒu
			, viewport.TopLeftY													// �r���[�|�[�g�����X�ʒu
			, viewport.Width													// �r���[�|�[�g��
			, viewport.Height													// �r���[�|�[�g����
			, viewport.MinDepth													// �[�x�l�̍ŏ��l
			, viewport.MaxDepth													// �[�x�l�̍ő�l
			, Projection														// �v���W�F�N�V�����s��
			, View																// �r���[�s��
			, world																// ���[���h�s��(�P�ʍs��)
		);
		DirectX::XMFLOAT3 worldPositionZNear;
		DirectX::XMStoreFloat3(&worldPositionZNear, WorldPositionZNear);
	
	
		DirectX::XMVECTOR WorldPositionZFar{};
		screenPositon.z = 1.0f;
		WorldPositionZFar = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPositon)								// �X�N���[�����W
			, viewport.TopLeftX													// �r���[�|�[�g�����X�ʒu
			, viewport.TopLeftY													// �r���[�|�[�g�����X�ʒu
			, viewport.Width													// �r���[�|�[�g��
			, viewport.Height													// �r���[�|�[�g����
			, viewport.MinDepth													// �[�x�l�̍ŏ��l
			, viewport.MaxDepth													// �[�x�l�̍ő�l
			, Projection														// �v���W�F�N�V�����s��
			, View																// �r���[�s��
			, world																// ���[���h�s��(�P�ʍs��)
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