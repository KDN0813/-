#include <imgui.h>
#include "EnemySlime.h"
#include "Mathf.h"
#include "Collision.h"
#include "Graphics/Graphics.h"
#include "Player.h"

// コンストラクタ[04]
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    // 幅、高さ設定[06]
    radius = 0.5f;
    height = 1.0f;

    // 徘徊ステートへ遷移
    TranitionWanderState();
}

// デストラクタ[04]
EnemySlime::~EnemySlime()
{
    delete model;
}

// 更新処理[04]
void EnemySlime::Update(float elapseedTime)
{
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapseedTime);
        break;
    case State::Idel:
        UpdateIdelState(elapseedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapseedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapseedTime);
        break;
    case State::IdelBattle:
        UpdateIdelBattleState(elapseedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapseedTime);
        break;
    case State::Death:
        UpdateDeathState(elapseedTime);
        break;
    }

    // 速度処理更新
    UpdateVelocity(elapseedTime);

    // 無敵時間更新
    UpdateInvincibleTimer(elapseedTime);

    // オブジェクト行列を更新
    UpdateTransform();

    // アニメーション更新
    model->UpdateAnimation(elapseedTime);

    // モデル行列更新
    model->UpdateTransform(transform);
}

// 描画処理[04]
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// デバッグ用GUI描画[org]
void EnemySlime::DrawDebugGUI()
{
    // 位置
    ImGui::InputFloat3("Position", &position.x);
    // 半径
    ImGui::InputFloat("Radius",&radius);
    // 高さ
    ImGui::InputFloat("Height",&height);

    // 体力
    ImGui::InputInt("HP", &health);

    // 無敵時間
    ImGui::InputFloat("invincibleTimer", &invincibleTimer);
}

// 死亡した時に呼ばれる[08]
void EnemySlime::OnDead()
{
    // 死亡ステートへ遷移
    TranitionDeathState();
}

// デバックプリミティブ描画
void EnemySlime::DarwDebugPrimitive()
{
    Enemy::DarwDebugPrimitive();

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // 縄張り範囲をデバック円柱描画
    debugRender->DrawCylinder(territoryOrgin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

    // ターゲット位置をデバック球描画
    debugRender->DrawSphere(targetPositon, radius
        , DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

    // 探索範囲をデバック円柱描画
    debugRender->DrawCylinder(position, searcRange, 1.0f,
        DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

    // 攻撃範囲をデバック描画
    debugRender->DrawCylinder(position, attackRange, 1.0f,
        DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}

// ターゲット位置設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrgin = origin;
    territoryRange = range;
}

// ダメージを受けた時に呼ばれる
void EnemySlime::OnDamaged()
{
    // ダメージステートへ遷移
    TranitionDamageState();
}

// ターゲット位置をランダム指定
void EnemySlime::SetRandaomTargetPosition()
{
    float angle = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);

    targetPositon.x = territoryOrgin.x + sinf(angle) * range;
    targetPositon.y = territoryOrgin.y;
    targetPositon.z = territoryOrgin.y + cosf(angle) * range;
}

// 目標地点へ移動
void EnemySlime::MoveToTarget(float elapseedTime, float speedRate)
{
    // ターゲット方向への進行ベクトル
    float vx = targetPositon.x - position.x;
    float vz = targetPositon.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);

    vx /= dist;
    vz /= dist;

    // 移動処理
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapseedTime, vx, vz, turnSpeed * speedRate);
}

// プレイヤーの探索
bool EnemySlime::SearchPlayer()
{
    const DirectX::XMFLOAT3& playerPositon
        = Player::Instance().GetPosition();
    float vx = playerPositon.x - position.x;
    float vy = playerPositon.y - position.y;
    float vz = playerPositon.z - position.z;
    float dist = sqrtf((vx * vx) + (vy * vy) + (vz * vz));

    if(dist<searcRange)
    {
        float distXZ = sqrtf((vx * vx) + (vz * vz));
        // 単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;
        // 前方ベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);

        // 2つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if(dot>0.0f)
        {
            return true;
        }
    }
    return false;
}

// ノードとプレイヤーの衝突処理
void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float boneRadius)
{
    Player& player = Player::Instance();

    // ノード取得
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        // ノードのワールド座標取得
        DirectX::XMFLOAT3 nodePosition;
            nodePosition =
        {
            node->worldTransform._41,
            node->worldTransform._42 ,
            node->worldTransform._43
        };

            // 当たり判定表示
            Graphics::Instance().GetDebugRenderer()->DrawSphere(
                nodePosition, boneRadius, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
            );

            //// 衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                boneRadius,
                player.GetPosition(),
                player.GetRadius(),
                player.GetHeigh(),
                outPosition
            ))
            {
                // 当たっていれば
                // ダメージを与える
                if (player.ApplyDamage(1, 0.5f))
                {
                    // 吹き飛ばす
                    {
                        DirectX::XMFLOAT3 vec = {};
                        vec.x = outPosition.x -= nodePosition.x;
                        vec.z = outPosition.z -= nodePosition.z;
                        float lenght = sqrtf(vec.x * vec.x + vec.z * vec.z);
                        vec.x / lenght;
                        vec.z / lenght;

                        // XZ平面に吹っ飛ばす力をかける
                        float power = 10.0f;
                        vec.x* power;
                        vec.z* power;
                        // Y方向にも力をかける
                        vec.y = 5.0f;

                        // 吹っ飛ばす
                        player.AddImpulse(vec);
                    }
                }
            }
    }
}

// 徘徊ステートへ遷移
void EnemySlime::TranitionWanderState()
{
    state = State::Wander;

    // 目的地点設定
    SetRandaomTargetPosition();

    // 歩きアニメーション再生
    model->PlayAnimation(Anim_WalkFWD, true);
}

// 徘徊ステート更新処理
void EnemySlime::UpdateWanderState(float elapseedTime)
{
    // 目標地点までXZ平面での距離判定
    float vx = targetPositon.x - position.x;
    float vz = targetPositon.z - position.z;
    float distSp = vx * vx + vz * vz;
    if (distSp < radius * radius)
    {
        // 待機ステートに遷移
        TranitionIdelState();
    }

    // プレイヤー探索
    if(SearchPlayer())
    {
        // 見つかったら追跡ステートへ遷移
        TranitionPursuitState();
    }

    // 目標地点へ移動
    MoveToTarget(elapseedTime, 0.5f);
}

// 徘徊ステートへ遷移
void EnemySlime::TranitionIdelState()
{
    state = State::Idel;

    // タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // 待機アニメーション再生
    model->PlayAnimation(Anim_IdelNormal, true);
}

// 待機ステート更新処理
void EnemySlime::UpdateIdelState(float elapseedTime)
{
    // タイマー処理
    stateTimer -= elapseedTime;
    if(stateTimer<0.0f)
    {
        // 徘徊ステートへ遷移
        TranitionWanderState();
    }

    // プレイヤー探索
    if (SearchPlayer())
    {
        // 見つかったら追跡ステートへ遷移
        TranitionPursuitState();
    }
}

// 待機ステートへ遷移
void EnemySlime::TranitionPursuitState()
{
    state = State::Pursuit;

    // タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // 歩きアニメーション再生
    model->PlayAnimation(Anim_RundFWD, true);
}

// 追跡ステート更新処理
void EnemySlime::UpdatePursuitState(float elapseedTime)
{
    // 目標地点をプレイヤー位置に設定
    targetPositon = Player::Instance().GetPosition();

    // 目標地点へ移動
    MoveToTarget(elapseedTime, 1.0f);

    // タイマー処理
    stateTimer -= elapseedTime;
    if (stateTimer < 0.0f)
    {
        // 徘徊ステートへ遷移
        TranitionWanderState();
    }

    // プレイヤーに近づくと攻撃ステートへ遷移
    float vx = targetPositon.x - position.x;
    float vy = targetPositon.y - position.y;
    float vz = targetPositon.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        // 攻撃ステートへ遷移
        TranitionAttackState();
    }
}

// 攻撃ステートへ遷移
void EnemySlime::TranitionAttackState()
{
    state = State::Attack;

    // 攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack1, false);
}

// 攻撃ステート更新処理
void EnemySlime::UpdateAttackState(float elapseedTime)
{
    // 任意のアニメーション再生区間でのに衝突処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    if(animationTime >=0.1f && animationTime<=0.35f)
    {
        // 目玉ノードとプレイヤーの衝突処理
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    // 攻撃アニメーションが終わったら
    if(!model->IsPlayAnimation())
    {
        // 戦闘待機ステートに遷移
        TranitionBattleState();
    }
}

// 戦闘待機ステートへ遷移
void EnemySlime::TranitionBattleState()
{
    state = State::IdelBattle;

    // タイマーをランダム設定
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    // 戦闘待機アニメーション再生
    model->PlayAnimation(Anim_IdleBattle, true);
}

// 戦闘待機ステート更新処理
void EnemySlime::UpdateIdelBattleState(float elapseedTime)
{
    // 目的地点をプレイヤー位置に設定
    targetPositon = Player::Instance().GetPosition();

    // タイマー処理
    stateTimer -= elapseedTime;
    if (stateTimer < 0.0f)
    {
        // プレイヤーに攻撃範囲にいた場合は攻撃ステートへ遷移
        float vx = targetPositon.x - position.x;
        float vy = targetPositon.y - position.y;
        float vz = targetPositon.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            // 攻撃ステートへ遷移
            TranitionAttackState();
        }
        else
        {
            // 徘徊ステートへ遷移
            TranitionWanderState();

        }
    }

    MoveToTarget(elapseedTime, 0.0f);
}

// ダメージステートに遷移
void EnemySlime::TranitionDamageState()
{
    state = State::Damage;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_GetGit, false);
}

// ダメージステート更新処理
void EnemySlime::UpdateDamageState(float elapseedTime)
{
    // ダメージアニメーションが終わったら
    if(!model->IsPlayAnimation())
    {
        // 戦闘待機ステートへ遷移
        TranitionIdelState();
    }
}

// 死亡ステートに遷移
void EnemySlime::TranitionDeathState()
{
    state = State::Death;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_Die, false);
}

// 死亡ステート更新処理
void EnemySlime::UpdateDeathState(float elapseedTime)
{
    // ダメージアニメーションが終わったら
    if (!model->IsPlayAnimation())
    {
        // 自分を破棄
        Destroy();
    }
}
