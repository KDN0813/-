#include "ProjectileHoming.h"

// コンストラクタ[07]
ProjectileHoming::ProjectileHoming(ProjectileManager* manager)
    :Projectile(manager)
{
    // [07]
    model = new Model("Data/Model/Sword/Sword.mdl");

    // モデルが小さいのでスケーリング
    scale.x = scale.y = scale.z = 3.0f;
}

// デストラクタ[07]
ProjectileHoming::~ProjectileHoming()
{
    delete model;
}

// 更新処理[07]
void ProjectileHoming::Update(float elpsedTime)
{
    // 寿命処理[07]
    LifeTimer -= elpsedTime;
    if (LifeTimer <= 0.0f)
    {
        Destroy();
    }
    // 移動[07]
    {
        float moveSpeed = this->moveSpeed * elpsedTime;
        position.x += direction.x * moveSpeed;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed;
    }
    // 旋回[07]
    {
        float turnSpeed = this->turnSpeed * elpsedTime;
        // ターゲットまでのベクトルを算出
        DirectX::XMVECTOR Position      = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target        = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec           = DirectX::XMVectorSubtract(Target, Position);

        // ゼロベクトルでないなら回転処理
        DirectX::XMVECTOR LengthSq      = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            // ターゲットまでにベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);

            // 向いてる方向のベクトルを算出
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            
            // 前方向ベクトルとターゲットまでのベクトルの内積(角度)を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            // 2つの単位ベクトルの角度が小さいほど
            // 1.0に近づくという性質を利用して回転速度を調整する
            float rot = 1.0f - dot;
            if (rot > turnSpeed)
            {
                rot = turnSpeed;
            }

            // 回転角度があるなら回転処理する
            if (fabs(dot) > 0.0001)
            {
                // 回転軸を算出
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                Axis = DirectX::XMVector3Normalize(Axis);

                // 回転軸と回転量から回転行列を算出
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

                // 現在の行列を回転させる
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
                //Transform *= Rotation;

                // 回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }
    }

    // オブジェクト行列を更新
    UpdateTransform();

    // モデル行列更新
    model->UpdateTransform(transform);
}


// 描画処理[07]
void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// 発射[07]
void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction,
                              const DirectX::XMFLOAT3& position,
                              const DirectX::XMFLOAT3& target)
{
    this->direction = direction;
    this->position = position;
    this->target = target;

    UpdateTransform();
}
