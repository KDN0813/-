#pragma once
#include "Graphics/Model.h"
#include "Stage.h"

// 移動床ステージ
class StageMoveFloor : public Stage
{
public:
    StageMoveFloor();
    ~StageMoveFloor();

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

    // スタート位置設定
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

    // ゴール位置設定
    void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

    // トルク設定
    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

private:
    // 行列更新処理
    void UpdateTransform();

private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 position  = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 angle     = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 scale     = { 1.0f,1.0f,1.0f, };
    DirectX::XMFLOAT4X4 transform = 
    {
        1.0f,.0f,.0f,.0f,
        .0f,1.0f,.0f,.0f,
        .0f,.0f,1.0f,.0f,
        .0f,.0f,.0f,1.0f
    };
    DirectX::XMFLOAT4X4 oldTransform =
    {
        1.0f,.0f,.0f,.0f,
        .0f,1.0f,.0f,.0f,
        .0f,.0f,1.0f,.0f,
        .0f,.0f,.0f,1.0f
    };

    DirectX::XMFLOAT3 torque    = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 start     = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 goal      = { .0f,.0f,.0f, };
    DirectX::XMFLOAT3 oldAngle  = {};

    float moveSpeed = 2.0f;
    float moveRate = 0.0f;
};

