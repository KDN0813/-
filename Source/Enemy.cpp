#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

int Enemy::enemuNum = 0;

// デバックプリミティブ描画[05]
void Enemy::DarwDebugPrimitive()
{
    // [05]
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバック球を描画[05]
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 衝突判定用のデバッグ円柱を描画[06]
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// 破棄[08]
void Enemy::Destroy()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    // 削除リストに自身を追加する[08]
    enemyManager.Remove(this);
}
