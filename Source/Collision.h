#pragma once
#include <DirectXMath.h>
#include "Graphics/Model.h"

/// <summary>
/// ヒット結果[12]
/// </summary>
/// <param name="position">レイととポリゴンの交点</param>
/// <param name="normal">衝突したポリゴンの法線ベクトル</param>
/// <param name="rotation">回転量</param>
/// <param name="distance">レイの始点から交点までの距離</param>
/// <param name="materialIndex">衝突したポリゴンのマテリアル番号</param>
struct HitResult
{
    DirectX::XMFLOAT3   position        = {};
    DirectX::XMFLOAT3   normal          = {};
    DirectX::XMFLOAT3   rotation          = {};
    float               distance        = 0.0f;
    int                 materialIndex   = -1;
};

// コリジョン[05]
class Collision
{
public:
    // 球と球の交差判定[05]
    static bool IntersectSphareVsSphere(
        const DirectX::XMFLOAT3& positionA
        , float radiusA
        , const DirectX::XMFLOAT3& positionB
        , float radiusB
        , DirectX::XMFLOAT3& outPositionB
    );

    // 円柱と円柱の交差判定[06]
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA
        , float radiusA
        , float heightA
        , const DirectX::XMFLOAT3& positionB
        , float radiusB
        , float heightB
        , DirectX::XMFLOAT3& outPositionB
    );

    // 球と円柱の交差判定[08]
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition
        , float sphereRadius
        , const DirectX::XMFLOAT3& cylinderPosition
        , float cylinderRadius
        , float cylinderHeight
        , DirectX::XMFLOAT3& outCylinderPosition
    );

    /// <summary>
    /// レイとモデルの交差判定
    /// </summary>
    /// <param name="start">レイの始点</param>
    /// <param name="end">レイの終点</param>
    /// <param name="model">モデルのポインタ</param>
    /// <param name="result">ヒットリザルト(結果を格納する)</param>
    /// <returns>交差判定(交差したらtrue)</returns>
    static bool IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};

