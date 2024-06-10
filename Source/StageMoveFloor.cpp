#include "StageMoveFloor.h"
#include "Collision.h"

// コンストラクタ
StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    // ステージモデル読み込み
    model = new Model("Data/Model/Cube/Cube.mdl");
}

// デストラクタ
StageMoveFloor::~StageMoveFloor()
{
    // ステージモデル破棄
    delete model;
}

// 更新処理
void StageMoveFloor::Update(float elapasedTIme)
{
    // 前回の情報を保存
    oldTransform = transform;
    oldAngle = angle;

    // スタートからゴールまでの距離を算出する
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    // スタートからゴールまでの間を1秒間で進む割合(0.0～1.0)を算出する
    float speed = moveSpeed * elapasedTIme;
    float speedRate = speed / length;
    moveRate += speedRate;

    // ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    // 線形補間で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    // 回転
    angle.x += torque.x * elapasedTIme;
    angle.y += torque.y * elapasedTIme;
    angle.z += torque.z * elapasedTIme;

    // 行列更新
    UpdateTransform();

    // レイキャスト用にモデル空間行列にするための単位行列を渡す
    const DirectX::XMFLOAT4X4 transformIdentity = { 1.0f,0.0f,0.0f ,0.0f  ,0.0f ,1.0f ,0.0f ,0.0f  ,0.0f ,0.0f ,1.0f ,0.0f  ,0.0f ,0.0f ,0.0f ,1.0f };
    model->UpdateTransform(transformIdentity);
}

// 描画処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // 表示用のためワールド行列に更新する
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

// レイキャスト
bool StageMoveFloor::RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    // 前回のワールド行列と逆行列を求める
    DirectX::XMMATRIX oldWolrdTransform = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX inverseOldWolrdTransform = DirectX::XMMatrixInverse(nullptr, oldWolrdTransform);
    
    // 前回のローカル空間でのレイに変換
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, inverseOldWolrdTransform);      // WorldStartを前回のローカル行列に合わせた3Dベクトルに変換
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, inverseOldWolrdTransform);        // WorldEndを前回のローカル行列に合わせた3Dベクトルに変換

    // ローカル空間でのレイとの交点を求める
    DirectX::XMFLOAT3 localStart, localEnd;

    DirectX::XMStoreFloat3(&localStart, LocalStart);
    DirectX::XMStoreFloat3(&localEnd, LocalEnd);
    
    HitResult locakHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, locakHit))
    {
        // 前回のローカル空間から今回のワールド空間へ変換
        // 前回から今回にかけて変更された内用が全て乗っているオブジェクトに反映される

        // 現在のワールド座標
        DirectX::XMMATRIX WolrdTransform = DirectX::XMLoadFloat4x4(&this->transform);
        // 交点
        DirectX::XMVECTOR HitPositon = DirectX::XMLoadFloat3(&locakHit.position);

        DirectX::XMVECTOR WolrdHitPositon = XMVector3Transform(HitPositon, WolrdTransform);


        // HitResultに保存
        DirectX::XMStoreFloat3(&locakHit.position, WolrdHitPositon);

        hit = locakHit;

        // 回転差分を算出
        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;
        return true;
    }
    return false;
}

// 行列更新処理
void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
