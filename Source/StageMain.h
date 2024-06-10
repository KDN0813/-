#pragma once
#include "Stage.h"

// メインステージ
class StageMain :public Stage
{
public:
    StageMain();
    ~StageMain();

    // 更新処理
    void Update(float elapasedTIme)override;

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    /// <summary>
    /// レイキャスト
    /// </summary>
    /// <param name="start">レイの始点</param>
    /// <param name="end">レイの終点</param>
    /// <param name="result">ヒットリザルト(結果格納)</param>
    /// <returns>交差判定</returns>
    bool RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

private:
    Model* model = nullptr;
};
