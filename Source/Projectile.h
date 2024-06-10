#pragma once

#include "Graphics/Shader.h"

// 前方宣言
class ProjectileManager;

// 弾丸[07]
class Projectile
{
public:
    // [07]
    Projectile(ProjectileManager* manager);
    // [07]
    virtual ~Projectile() {}

    // 更新処理[07]
    virtual void Update(float elapsedTime) = 0;

    // 描画処理[07]
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    // デバッグプリミティブ描画[08]
    virtual void DrawDebugPrimitive();

    // 位置情報取得[07]
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 方向取得[07]
    const DirectX::XMFLOAT3& GetDirection() const { return direction; }

    // スケール取得[07]
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // 破棄
    void Destroy();

    // 半径取得[08]
    float GetRadius() const { return radius; }

protected:
    // 行列更新処理[07]
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