#pragma once

#include <vector>
#include "Projectile.h"
#include <set>

// 弾丸マネージャー[07]
class ProjectileManager
{
public:
    // [07]
    ProjectileManager();
    // [07]
    ~ProjectileManager();

    // 更新処理[07]
    void Update(float elapsedTime);

    // 描画処理[07]
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // デバッグプリミティブ描画[07]
    void DrawDebugPrimitive();

    // 弾丸登録[07]
    void Register(Projectile* projectile);

    // 弾丸全削除[07]
    void Clear();

    // 弾丸数取得[07]
    int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

    // 弾丸取得[07]
    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    // 弾丸削除[07]
    void Remove(Projectile* projectile);

private:
    // [07]
    std::vector<Projectile*> projectiles;

    // 破棄クラス[07]
    std::set<Projectile*>removes;
};

