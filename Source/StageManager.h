#pragma once

#include <vector>
#include"Stage.h"

// ステージマネージャー
class StageManager
{
private:
    StageManager(){}
    ~StageManager(){}
public:
    // 唯一のインスタンス
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // ステージ登録
    void Register(Stage* stage);

    // ステージ全削除[04]
    void Clear();

    // レイキャスト
    bool RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

private:
    std::vector<Stage*> stages;
};

