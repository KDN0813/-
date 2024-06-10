#include "ProjectileStraight.h"
#include "StageManager.h"
#include "Collision.h"

// コンストラクタ[07]
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
    :Projectile(manager)    // 基底クラスのコンストラクタを呼び出す
{
    // [07]
    model = new Model("Data/Model/Sword/Sword.mdl");

    // 表示サイズを調整[07]
    scale.x = scale.y = scale.z = 3.0f;
}

// デストラクタ[07]
ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

// 更新処理[07]
void ProjectileStraight::Update(float elapsedTime)
{
    // 寿命処理[07]
    LifeTImer -= elapsedTime;
    if (LifeTImer < 0)
    {
        // 自分を削除
        Destroy();
    }

    // 移動[07]
    this->UpdateHorizontalMove(elapsedTime);

    // オブジェクト行列を更新[07]
    UpdateTransform();

    // モデル行列更新[07]
    model->UpdateTransform(transform);
}

// 描画処理[07]
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// 発射[07]
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

void ProjectileStraight::UpdateHorizontalMove(float elapsedTime)
{
    // 水平速力計算[13]
    DirectX::XMFLOAT2 velocity = { this->direction.x * this->speed,this->direction.z * this->speed };
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    if (velocityLengthXZ > 0.0f)
    {
        // 水平移動[13]
        float mx = velocity.x * elapsedTime;
        float mz = velocity.y * elapsedTime;

        // レイの開始位置と終点位置[13]
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,position.y + step0ffset,position.z + mz };

        // レイキャスト壁判定[13]
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // 壁からレイの終点までのベクトル[13]
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // 壁の法線[13]
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // 入射ベクトルを法線に射影[13]
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 2.0f);

            // 補正位置の計算[13]
            DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 correctionPositon;
            DirectX::XMStoreFloat3(&correctionPositon, CorrectionPositon);

            // 方向計算
            DirectX::XMVECTOR Direction = DirectX::XMVectorSubtract(CorrectionPositon, Start);
            Direction = DirectX::XMVector3Normalize(Direction);

            // 壁ずり方向へのレイキャスト
            HitResult hit2;
            if (!StageManager::Instance().RayCact(start, correctionPositon, hit2))
            {
                // 壁ずり方向で壁に当たらなかったら
                position.x = correctionPositon.x;
                position.z = correctionPositon.z;

                DirectX::XMStoreFloat3(&this->direction, Direction);
                this->direction.y = 0.0f;
                
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;

                DirectX::XMStoreFloat3(&this->direction, Direction);
                this->direction.y = 0.0f;
            }
        }
        else
        {
            position.x += mx;
            position.z += mz;
        }
    }
}
