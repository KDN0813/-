#pragma once
#include "Stage.h"

// ���C���X�e�[�W
class StageMain :public Stage
{
public:
    StageMain();
    ~StageMain();

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

private:
    Model* model = nullptr;
};
