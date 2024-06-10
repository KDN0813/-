#include "Collision.h"

// 球と球の交差判定[05]
bool Collision::IntersectSphareVsSphere(
    const DirectX::XMFLOAT3& positionA
    , float radiusA
    , const DirectX::XMFLOAT3& positionB
    , float radiusB
    , DirectX::XMFLOAT3& outPositionB)
{
    // AからBの単位ベクトルを算出[05]
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSp  = DirectX::XMVector3Length(Vec);
    DirectX::XMVECTOR LengthSp  = DirectX::XMVector3LengthSq(Vec);
    // Spがついていると√を取る計算がされない

    float lengthSP;
    DirectX::XMStoreFloat(&lengthSP, LengthSp);

    // 距離判定[05]
    float range = radiusA + radiusB;
    // LengthSpが二乗した値なのでrangeも二乗する
    if (lengthSP > range * range)
    {
        return false;
    }
    float Y;
    Y = DirectX::XMVectorGetY(Vec);

    // AがBを押し出す
    DirectX::XMVECTOR OutPositionB = DirectX::XMVector3Normalize(Vec);
    OutPositionB = DirectX::XMVectorScale(OutPositionB, range);
    OutPositionB = DirectX::XMVectorAdd(PositionA, OutPositionB);


    DirectX::XMStoreFloat3(&outPositionB, OutPositionB);

    return true;
}

// 円柱と円柱の交差判定[06]
bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
    // Aの足もとがBの頭の上に当たっていない[06]
    if ((positionA.y) > (positionB.y + heightB))
    {
        return false;
    }

    // Aの頭がBの足もとより下なら当たっていない[06]
    if ((positionA.y + heightB) < (positionB.y))
    {
        return false;
    }

    // XZ平面での範囲チェック[06]
    DirectX::XMFLOAT2 vec = { positionB.x - positionA.x,positionB.z - positionA.z };
    float lengthXZ = sqrtf(vec.x * vec.x) + (vec.y * vec.y);
    float range = radiusA + radiusB;
    if (lengthXZ > range)
    {
        return false;
    }

    // AがBを押し出す
    vec = { vec.x / lengthXZ,vec.y / lengthXZ };
    outPositionB.x = positionA.x + vec.x * range;
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + vec.y * range;
    return true;
}

// 球と円柱の交差判定[08]
bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition
    , float sphereRadius
    , const DirectX::XMFLOAT3& cylinderPosition
    , float cylinderRadius
    , float cylinderHeight
    , DirectX::XMFLOAT3& outCylinderPosition
)
{
    // 球の足もとが円柱の頭の上に当たっていない[06]
    if ((spherePosition.y - sphereRadius) > (cylinderPosition.y + cylinderHeight))
    {
        return false;
    }

    // 球の頭が円柱の足もとより下なら当たっていない[06]
    if ((spherePosition.y + sphereRadius) < (cylinderPosition.y))
    {
        return false;
    }

    // XZ平面での範囲チェック[06]
    DirectX::XMFLOAT2 vec = { cylinderPosition.x - spherePosition.x,cylinderPosition.z - spherePosition.z };
    float lengthXZ = sqrtf(vec.x * vec.x) + (vec.y * vec.y);
    float range = sphereRadius + cylinderRadius;
    if (lengthXZ > range)
    {
        return false;
    }

    // AがBを押し出す
    vec = { vec.x / lengthXZ,vec.y / lengthXZ };
    outCylinderPosition.x = spherePosition.x + vec.x * range;
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + vec.y * range;

    return true;
}

bool Collision::IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result
)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    // ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    // 全てのメッシュを回す
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // メッシュノード取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        // レイをワールド空間からローカル空間へ変更
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);     // ワールド行列の逆行列

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);      // WorldStartをメッシュのローカル行列に合わせた3Dベクトルに変換
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);        // WorldEndをメッシュのローカル行列に合わせた3Dベクトルに変換
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);                                          // SからEまでのベクトルの単位ベクトル
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        // レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        // 三角形(面)との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices; // 頂点バッファ
        const std::vector<UINT> indices = mesh.indices;                     // インデックスバッファ

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                // 三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // 三角形の三辺ベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // 三角形の法線ベクトルを算出
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                // 内積の結果がプラスなら裏向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(N, V);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (0.0f <= dot) continue;

                // レイと平面の交差判定
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                //DirectX::XMVECTOR X     = DirectX::XMVector3Dot(V, SA);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < 0.f || x > neart) continue;     // 交点までの距離が今までに計算した最近距離より大きいときはスキップ

                // 三角形とレイの交点
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
                P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);

                // 交点が三角形の内側にあるか判定
                // 1つめ
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                // 2つめ
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                DirectX::XMFLOAT3 s1, s2;
                DirectX::XMStoreFloat3(&s1, PB);
                DirectX::XMStoreFloat3(&s2, Cross2);

                DirectX::XMFLOAT3 t1, t2;
                // 3つめ
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);

                DirectX::XMStoreFloat3(&t1, PC);
                DirectX::XMStoreFloat3(&t2, Cross3);

                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                // 最短距離を更新
                neart = x;

                // 交点と法線を更新
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            // ローカル座標からワールド空間へ変換
            DirectX::XMVECTOR WorldPositon = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);

            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPositon, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            // ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPositon);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}
