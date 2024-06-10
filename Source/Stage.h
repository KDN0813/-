#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"

// ステージ
class Stage
{
public:
    Stage() {};
    virtual ~Stage() {};

    // 更新処理
    virtual void Update(float elapasedTIme) = 0;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    /// <summary>
    /// レイキャスト
    /// </summary>
    /// <param name="start">レイの始点</param>
    /// <param name="end">レイの終点</param>
    /// <param name="result">ヒットリザルト(結果格納)</param>
    /// <returns>交差判定</returns>
    virtual bool RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;
};

