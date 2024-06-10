#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

// 更新処理[07]
void ProjectileManager::Update(float elapsedTime)
{
    for (Projectile* p : projectiles)
    {
        p->Update(elapsedTime);
    }

    // 破棄処理
    // ※projectilesの範囲for分中でerase()すると不具合が発生するため、
    //  更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Projectile* projectile : removes)
    {
        // std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Projectile*>::iterator it = 
            std::find(projectiles.begin(), projectiles.end(), projectile);

        // (自)削除するデータが存在すれば
        // (自)※find関数はデータが見つからないと最後のイテレータを返す
        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        // 弾丸の破棄処理
        delete projectile;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理[07]
void ProjectileManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (auto p : projectiles)
    {
        p->Render(context, shader);
    }
}

// デバッグプリミティブ描画[07]
void ProjectileManager::DrawDebugPrimitive()
{
    for (auto p : projectiles)
    {
        p->DrawDebugPrimitive();
    }
}

// 弾丸登録[07]
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

// 弾丸全削除[07]
void ProjectileManager::Clear()
{
    for (auto p : projectiles)
    {
        delete(p);
    }
    projectiles.clear();
}

// 弾丸削除[07]
void ProjectileManager::Remove(Projectile* projectile)
{
    // 破棄リストに追加
    removes.insert(projectile);
}
