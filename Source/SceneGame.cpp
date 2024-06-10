#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Camera.h"

#include "StageManager.h"

// ������[01]
void SceneGame::Initialize()
{
	// �X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	this->stageMain = new StageMain();
	stageManager.Register(stageMain);

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
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);
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

		shader->End(dc);
	}

	// 3D�G�t�F�N�g�`��
	{
	}

	// 3D�f�o�b�O�`��
	{

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

		// �J�����R���g���[���[�f�o�b�O�`��
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
}