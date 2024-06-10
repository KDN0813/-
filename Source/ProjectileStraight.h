#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"

// ���i�e��[07]
class ProjectileStraight : public Projectile
{
public:
    // [07]
    ProjectileStraight(ProjectileManager* manager);
    // [07]
    ~ProjectileStraight() override;

    // �X�V����[07]
    void Update(float elapsedTime) override;

    // �`�揈��[07]
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // ����[07]
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    // �ǂƂ̓����蔻��
    void UpdateHorizontalMove(float elapsedTime);
private:
    // [07]
    Model* model    = nullptr;
    float speed     = 10.0f;
    float LifeTImer = 3.0f;
    float step0ffset = 0.0f;
};