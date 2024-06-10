#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"

// �ǔ��e��[07]
class ProjectileHoming : public Projectile
{
public:
    // [07]
    ProjectileHoming(ProjectileManager* manager);
    // [07]
    ~ProjectileHoming() override;

    // �X�V����[07]
    void Update(float elpsedTime)override;

    // �`�揈��[07]
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    // ����[07]
    void Launch(
        const DirectX::XMFLOAT3& direction
        , const DirectX::XMFLOAT3& position
        , const DirectX::XMFLOAT3& target
    );

private:
    // [07]
    Model* model                = nullptr;
    DirectX::XMFLOAT3 target    = { 1.0f,1.0f,1.0f };
    float moveSpeed             = 10.0f;
    float turnSpeed             = DirectX::XMConvertToRadians(180);
    float LifeTimer             = 3.0f;
};

