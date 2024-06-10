#include "StageManager.h"

// 更新処理
void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

// 描画処理
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(dc, shader);
    }
}

// ステージ登録
void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

// ステージ全削除
void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
        stage = nullptr;
    }
    stages.clear();
}

// レイキャスト
bool StageManager::RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;
    hit.distance = FLT_MAX;

    for (Stage* stage : stages)
    {
        HitResult tmp;
        if (stage->RayCact(start, end, tmp))
        {
            // 距離が近ければ代入
            if (hit.distance > tmp.distance)
            {
                hit = tmp;
                result = true;
            }
        }
    }
    return result;
}
