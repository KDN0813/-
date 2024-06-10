#include <imgui.h>
#include "EnemyManager.h"
#include "Collision.h"

// [04]
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理[08]
    for (Enemy* remove : removes)
    {
        // std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Enemy*>::iterator it =
            std::find(enemies.begin(), enemies.end(), remove);

        // (自)削除するデータが存在すれば
        // (自)※find関数はデータが見つからないと最後のイテレータを返す
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // 弾丸の破棄処理
        delete remove;
    }
    // 破棄リストをクリア
    removes.clear();


    // 敵同士の衝突処理[05]
    CollisionEnemyVsEnemies();
}


// [04]
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}


// エネミー登録[04]
void EnemyManager::Register(Enemy* enemy)
{
    //[04]
    enemies.emplace_back(enemy);
}

// エネミー全削除[04]
void EnemyManager::Clear()
{
    // [04]
    for (Enemy* enemy : enemies)
    {
        // [04]
        delete(enemy);
    }
    // [04]
    enemies.clear();
}

// デバッグ用GUI描画[org]
void EnemyManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (int i = 0; i < GetEnemyCount(); ++i)
            {
                int  a = 22;
                char n = static_cast<char>(a);

                char no[2];
                snprintf(no, 2, "%d", i);
                if (ImGui::TreeNode(no))
                {
                    // 位置
                    GetEnemy(i)->DrawDebugGUI();

                    ImGui::TreePop();
                }
            }
        }
    }
    ImGui::End();
}

// デバックプリミティブ描画[05]
void EnemyManager::DrawDebugPrimitive()
{
    for (auto enemy : enemies)
    {
        enemy->DarwDebugPrimitive();
    }
}

// エネミー同士の衝突処理[05]
void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = GetEnemy(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = GetEnemy(j);

            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition()
                , enemyA->GetRadius()
                , enemyA->GetHeigh()
                , enemyB->GetPosition()
                , enemyB->GetRadius()
                , enemyB->GetHeigh()
                , outPosition))
            {
                // 押し出しの後の位置設定
                enemyB->SetPosition(outPosition);
            }
        }
    }
}

// エネミー削除[08]
void EnemyManager::Remove(Enemy* enemy)
{
    // 破棄リストに追加[08]
    removes.insert(enemy);
}
