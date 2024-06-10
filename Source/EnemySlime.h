#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

// [04]
class EnemySlime: public Enemy
{
public:
    EnemySlime();// [04]
    ~EnemySlime() override;// [04]

    // 更新処理[04]
    void Update(float elapseedTime) override;

    // 描画処理[04]
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // デバッグ用GUI描画[org]
    void DrawDebugGUI();

    // 死亡した時に呼ばれる[08]
    void OnDead() override;

    // デバックプリミティブ描画
    void DarwDebugPrimitive() override;

    // ターゲット位置設定
    void SetTerritory(const DirectX::XMFLOAT3& origin,float range);

protected:
    // ダメージを受けた時に呼ばれる
    void OnDamaged() override;

private:
    // ターゲット位置をランダム指定
    void SetRandaomTargetPosition();

    // 目標地点へ移動
    void MoveToTarget(float elapseedTime, float speedRate);;

    // プレイヤーの探索
    bool SearchPlayer();

    // ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

    // ==========================
    // ステート
    // ==========================

    // 徘徊ステートへ遷移
    void TranitionWanderState();
    // 徘徊ステート更新処理
    void UpdateWanderState(float elapseedTime);

    // 徘徊ステートへ遷移
    void TranitionIdelState();
    // 徘徊ステート更新処理
    void UpdateIdelState(float elapseedTime);

    // 追跡ステートへ遷移
    void TranitionPursuitState();
    // 追跡ステート更新処理
    void UpdatePursuitState(float elapseedTime);

    // 攻撃ステートへ遷移
    void TranitionAttackState();
    // 攻撃ステート更新処理
    void UpdateAttackState(float elapseedTime);

    // 戦闘待機ステートへ遷移
    void TranitionBattleState();
    // 戦闘待機ステート更新処理
    void UpdateIdelBattleState(float elapseedTime);

    // ダメージステートに遷移
    void TranitionDamageState();
    // ダメージステート更新処理
    void UpdateDamageState(float elapseedTime);

    // 死亡ステートに遷移
    void TranitionDeathState();
    // 死亡ステート更新処理
    void UpdateDeathState(float elapseedTime);

private:
    // ステート
    enum class State
    {
        Wander,
        Idel,
        Pursuit,
        Attack,
        IdelBattle,
        Damage,
        Death,

        max,
    };

    // アニメーション
    enum Animation
    {
        Anim_IdelNormal,
        Anim_IdleBattle,
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalkRight,
        Anim_RundFWD,
        Anim_SceneSomthingST,
        Anim_SceneSomthingPRT,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetGit,
        Anim_Dizzy,
        Anim_Die,

        max,
    };

private:
    // [04]
    Model* model = nullptr;

    State state = State::Wander;
    DirectX::XMFLOAT3 targetPositon = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 territoryOrgin{ 0.0f,0.0f,0.0f };
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360.0f);

    float stateTimer = 0.0f;

    float searcRange = 5.0f;

    float attackRange = 1.5f;
};

