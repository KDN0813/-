#pragma once
#include "Graphics/Model.h"
#include "Stage.h"

// �ړ����X�e�[�W
class StageMoveFloor : public Stage
{
public:
    StageMoveFloor();
    ~StageMoveFloor();

    // �X�V����
    void Update(float elapasedTIme)override;

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    /// <summary>
    /// ���C�L���X�g
    /// </summary>
    /// <param name="start">���C�̎n�_</param>
    /// <param name="end">���C�̏I�_</param>
    /// <param name="result">�q�b�g���U���g(���ʊi�[)</param>
    /// <returns>��������</returns>
    bool RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

    // �X�^�[�g�ʒu�ݒ�
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

    // �S�[���ʒu�ݒ�
    void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

    // �g���N�ݒ�
    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

private:
    // �s��X�V����
    void UpdateTransform();

private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 position  = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 angle     = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 scale     = { 1.0f,1.0f,1.0f, };
    DirectX::XMFLOAT4X4 transform = 
    {
        1.0f,.0f,.0f,.0f,
        .0f,1.0f,.0f,.0f,
        .0f,.0f,1.0f,.0f,
        .0f,.0f,.0f,1.0f
    };
    DirectX::XMFLOAT4X4 oldTransform =
    {
        1.0f,.0f,.0f,.0f,
        .0f,1.0f,.0f,.0f,
        .0f,.0f,1.0f,.0f,
        .0f,.0f,.0f,1.0f
    };

    DirectX::XMFLOAT3 torque    = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 start     = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 goal      = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 oldAngle  = {};

    float moveSpeed = 2.0f;
    float moveRate = 0.0f;
};

