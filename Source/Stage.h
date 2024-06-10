#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"

// �X�e�[�W
class Stage
{
public:
    Stage() {};
    virtual ~Stage() {};

    // �X�V����
    virtual void Update(float elapasedTIme) = 0;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    /// <summary>
    /// ���C�L���X�g
    /// </summary>
    /// <param name="start">���C�̎n�_</param>
    /// <param name="end">���C�̏I�_</param>
    /// <param name="result">�q�b�g���U���g(���ʊi�[)</param>
    /// <returns>��������</returns>
    virtual bool RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;
};

