#pragma once

#include "Graphics/Shader.h"

// �O���錾
class ProjectileManager;

// �e��[07]
class Projectile
{
public:
    // [07]
    Projectile(ProjectileManager* manager);
    // [07]
    virtual ~Projectile() {}

    // �X�V����[07]
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��[07]
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    // �f�o�b�O�v���~�e�B�u�`��[08]
    virtual void DrawDebugPrimitive();

    // �ʒu���擾[07]
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �����擾[07]
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }

    // �X�P�[���擾[07]
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // �j��
    void Destroy();

    // ���a�擾[08]
    float GetRadius() const { return radius; }

protected:
    // �s��X�V����[07]
    void UpdateTransform();

protected:
    // [07]
    DirectX::XMFLOAT3 position      = { 0,0,0 };
    DirectX::XMFLOAT3 direction     = { 0,0,1 };
    DirectX::XMFLOAT3 scale         = { 0,0,1 };
    DirectX::XMFLOAT4X4 transform   =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    ProjectileManager* manager      = nullptr;

    // [08]
    float radius                    = 0.5f;
};