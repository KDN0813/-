#include "Character.h"
#include <algorithm>
#include "Collision.h"
#include "Mathf.h"
#include "StageManager.h"

// 行列更新処理
void Character::UpdateTransform()
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    //DirectX::XMMATRIX R= DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;

    // 位置行列を作成
    DirectX::XMMATRIX T= DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // 3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    // 計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

// ダメージを与える
bool Character::ApplyDamage(int damage, float invincbleTime)
{
    // ダメージが0の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    // 無敵時間中は健康状態を更新しない
    if (invincibleTimer > 0.0f) return false;

    // 死亡してる場合は健康状態を変更しない
    if (health <= 0) return false;

    // ダメージ処理
    health -= damage;

    // 死亡通知
    if (health <= 0)
    {
        OnDead();
    }
    // ダメージ通知
    else
    {
        OnDamaged();
    }

    // ダメージを与えた時無敵時間を設定
    invincibleTimer = invincbleTime;

    // 健康状態を変更した場合はtrueを返す
    return true;
}

// 衝撃を与える[09]   Impulse(衝撃)
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // 速力に力を与える(自己解)
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// 移動処理[03]
void Character::Move(float vx, float vz, float speed)
{
    // 移動方向ベクトルを設定[09]
    moveVecX = vx;
    moveVecZ = vz;

    // 最大速度設定[09]
    maxMoveSpeed = speed;
}

// 旋回処理[03]
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    // [03]
    speed *= elapsedTime;

    // 進行ベクトルがゼロの場合は処理する必要なし[03]
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001) return;

    // 進行ベクトルを単位ベクトル化[03]
    vx /= length;
    vz /= length;

    // 自身の回転値から前方向を求める[03]
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    // 回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (frontX * vx + frontZ * vz);

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど、1.0に近づくという性質を利用して回転速度を調整する
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する[03]
    float cross = (frontZ * vx) - (frontX * vz);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    //  左右判定を行うことによって左右回転を選択する[03]
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
}

// ジャンプ処理[06]
void Character::Jump(float speed)
{
    // 上方向の力を設定
    velocity.y = speed;
}

// 速度処理更新[06]
void Character::UpdateVelocity(float elaspsedTime)
{
    // 経過フレーム[06]
    float elapsedFrame = 60.0f * elaspsedTime;  // ゲームがn倍速で動いたときに辻褄を合わせる為の変数

    // 垂直速力更新処理[07]
    UpdateVerticalVelocity(elapsedFrame);

    // 水平速力更新処理[09](自己解)
    UpdateHorizontalVelocity(elapsedFrame);

    // 垂直移動更新処理[09]
    UpdateVerticalMove(elaspsedTime);

    // 水平移動更新処理[09](自己解)
    UpdateHorizontalMove(elaspsedTime);
}

// 無敵時間更新[08]
void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

// 垂直速力更新処理[08]
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    // 重力更新処理(自己解)
    velocity.y += Gravity * elapsedFrame;
}

// 垂直移動更新処理[08]
void Character::UpdateVerticalMove(float elapsedTime)
{
    // 垂直方向の移動量
    float my = velocity.y * elapsedTime;

    // キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0.0f,1.0f,0.0f };

    // 落下中
    if (my < 0.0f) 
    {
        // レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start     = { position.x,position.y + step0ffset,position.z };
        // レイの終点位置は移動語の位置
        DirectX::XMFLOAT3 end       = { position.x,position.y + my,position.z };

        // レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // 法線ベクトル取得
            normal = hit.normal;

            // 地面に接地している
            position = hit.position;

            // 回転
            angle.x += hit.rotation.x;
            angle.y += hit.rotation.y;
            angle.z += hit.rotation.y;

            // 傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // 着地した
            if (!isGround)
            {
                OnLandig();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // 空中にいる
            position.y += my;
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // 地面の向きに沿うようにXZ軸回転
    {
        // Y軸が法線ベクトル方向を吹くオイラー角回転を算出する
        float ax = atan2f(normal.z, normal.y);
        float az = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, ax, 0.2f);
        angle.z = Mathf::Lerp(angle.z, az, 0.2f);
    }
}

// 水平速力更新処理[09]
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZ平面の速力を減速する[09]
    // (自己解)
    float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
    if (length > 0.0f)
    {
        // 摩擦力
        float friction = this->friction * elapsedFrame;

        // 空中にいるときは摩擦力を減らす[09]
        // (自己解)
        if (!isGround) friction *= airControl;

        // 摩擦による横方向の減速処理[09]
        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // 横方向の速力が摩擦力以下になったので速力を無効化[09]
        else
        {
            // (自己解)
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    // XZ平面の速力を加速する[09]
    // (メ)速力の合計が最大速度以下なら
    if (length <= maxMoveSpeed)
    {
        // 移動ベクトルがゼロベクトルでないなら加速する[09]
        // (メ)加速力の合計(加速力があるか判別するだけなので[sqrtf]は使わない)
        float moveVecLength = ((moveVecX * moveVecX) + (moveVecZ * moveVecZ)); //(自己解 p8)
        if (moveVecLength > 0.0f)
        {
            // 加速力(acceleration<エクサレーション>:加速力)
            float acceleration = this->acceleration * elapsedFrame;

            // 空中にいるときは加速力を減らす[09]
            // (自己解)
            if(!isGround) acceleration *= airControl;

            // 移動ベクトルによる加速処理(自己解 p9)
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // 最大速度制限[09]
            // (メ)速度加算後に最大速度を超えていれば
            length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            // 下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    // 移動ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

// 水平移動更新処理[09]
void Character::UpdateHorizontalMove(float elapsedTime)
{
    // 水平速力計算[13]
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // 水平移動[13]
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // レイの開始位置と終点位置[13]
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffset,position.z };
        DirectX::XMFLOAT3 end   = { position.x + mx,position.y + step0ffset,position.z + mz };

        // レイキャスト壁判定[13]
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // 壁からレイの終点までのベクトル[13]
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End   = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec   = DirectX::XMVectorSubtract(End, Start);

            // 壁の法線[13]
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // 入射ベクトルを法線に射影[13]
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f);

            // 補正位置の計算[13]
            DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 correctionPositon;
            DirectX::XMStoreFloat3(&correctionPositon, CorrectionPositon);

            // 壁ずり方向へのレイキャスト
            HitResult hit2;
            if (!StageManager::Instance().RayCact(start, correctionPositon, hit2))
            {
                // 壁ずり方向で壁に当たらなかったら
                position.x = correctionPositon.x;
                position.z = correctionPositon.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            position.x += mx;
            position.z += mz;
        }
    }
}
