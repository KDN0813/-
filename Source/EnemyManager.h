#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

// エネミーマネージャー[04]
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager(){}

public:
    // 唯一のインスタンス取得[04]
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // 更新処理[04]
    void Update(float elapsedTime);

    // 描画処理[04]
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // エネミー登録:Update(登録する)[04]
    void Register(Enemy* enemy);

    //　エネミー全削除[04]
    void Clear();

    // デバッグ用GUI描画[org]
    void DrawDebugGUI();

    // デバックプリミティブ描画[05]
    void DrawDebugPrimitive();

    // エネミー数取得[05]
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // エネミー取得[05]
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    // エネミー同士の衝突処理[05]
    void CollisionEnemyVsEnemies();

    // エネミー削除[08]
    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    // [08]
    std::set<Enemy*>    removes;
};

