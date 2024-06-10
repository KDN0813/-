#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"

#include "Effect.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;

    // インスタンス取得
    static Player& Instance();

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // デバッグ用GUI描画
    void DrawDebugGUI();

    // デバックプリミティブ描画[05]
    void DarwDebugPrimitive();

protected:
    // スティック入力値から移動ベクトルを取得[03]
    DirectX::XMFLOAT3 GetMoveVec() const;

    // 移動入力処理[03]
    bool InputMove(float elapsedTime);

    // ジャンプ入力処理[06]
    bool InputJump();

    // 着地したときに呼ばれる[06]
    void OnLandig() override;

    // ダメージを受けた時に呼ばれる処理
    void OnDamaged() override;

    // 死亡した時に呼ばれる処理
    void OnDead() override;

private:
    // 待機ステートへ遷移[18]
    void TransitionIdleState();

    // 待機ステート更新処理[18]
    void UpdateIdleState(float elpsedTime);

    // 移動ステートへ遷移[18]
    void TransitionMoveState();

    // 移動ステート更新処理[18]
    void UpdateMoveState(float elpsedTime);
    
    // ジャンプステートへ遷移[18]
    void TransitionJumpState();

    // ジャンプステート更新処理[18]
    void UpdateJumpState(float elpsedTime);

    // 着地ステートへ遷移[18]
    void TransitionLandState();

    // 着地ステート更新処理[18]
    void UpdateLandState(float elpsedTime);

    // 攻撃入力処理[19]
    bool InputAttack();

    // 攻撃ステートへ遷移[19]
    void TransitionAattackState();

    // 攻撃ステート更新処理[19]
    void UpdateAttackState(float elpsedTime);

    // ダメージステートに遷移
    void TranitionDamageState();
    // ダメージステート更新処理
    void UpdateDamageState(float elapseedTime);

    // 死亡ステートに遷移
    void TranitionDeathState();
    // 死亡ステート更新処理
    void UpdateDeathState(float elapseedTime);

    // 復活ステートに遷移
    void TranitionRevivehState();
    // 復活ステート更新処理
    void UpdateReviveState(float elapseedTime);

private:
    // ステート[18]
    enum class State
    {
        Idle
        ,Move
        ,Jump
        ,Land
        ,Attack
        ,Damage
        ,Death
        ,Revive
    };

    // アニメーション[18]
    enum Anination
    {
        Anim_Attack
        , Anim_Death
        , Anim_Falling
        , Anim_GetHit1
        , Anim_GetHit2
        , Anim_Idle
        , Anim_Jump
        , Anim_Jump_Flip
        , Anim_Landig
        , Anim_Revive
        , Anim_Running
        , Anim_Walking
    };

private:
    Model* model        = nullptr;

    //[03]
    float moveSpeed     = 5.0f;
    float turnSpeed     = DirectX::XMConvertToRadians(720);

    //[06]
    float JumpSpeed     = 20.0f;
    int jumpCount       = 0;
    int jumpLimit       = 3;

    // [10]
    Effect*hitEffect    = nullptr;

    // [18]
    State state         = State::Idle;

    // [19]
    float leftHandRadiuns = 0.4f;
    bool attackCollisionFlag = false;
};