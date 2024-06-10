#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"

#include "Graphics/Graphics.h"
#include "Collision.h"

static Player* instance = nullptr;

// コンストラクタ[03]
Player::Player()
{
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    //// 待機ステートへ遷移
    TransitionIdleState();

    instance = this;
}

// [03]
Player::~Player()
{

    delete model;

    instance = nullptr;
}

Player& Player::Instance()
{
    return *instance;
}

void Player::Update(float elapsedTime)
{
    // ステート毎の処理[18]
    switch (state)
    {
    case Player::State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case Player::State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case Player::State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case Player::State::Land:
        UpdateLandState(elapsedTime);
        break;
    case Player::State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case Player::State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case Player::State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case Player::State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    }

    // 速度処理更新[06]
    UpdateVelocity(elapsedTime);

    // 無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    // モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    // オブジェクト行列を更新[03]
    UpdateTransform();

    // モデル行列更新[03]
    model->UpdateTransform(transform);
}

// スティック入力値から移動ベクトルを取得[03]
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // 入力情報を取得[03]
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する[03]
    Camera& camera = Camera::Intance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRIght();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする[03]

    // カメラ右方向ベクトルをXZ単位ベクトルに変換[03]
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化[03]
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換[03]
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化[03]
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する[03]
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    
    // Y軸務方向は移動しない[03]
    vec.y = 0.0f;

    return vec;
}

// 移動入力処理[03]
bool Player::InputMove(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // 移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    // 旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

// [03]
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// デバッグ用GUI描画[03]
void Player::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("player", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // 体力
            ImGui::InputInt("hp", &health);

            // 位置
            ImGui::InputFloat3("Position", &position.x);
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            // 角度
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // スケール
            ImGui::InputFloat3("Scale",&scale.x);
            // 速度
            ImGui::InputFloat3("Velocity", &velocity.x);
            // ジャンプカウント
            ImGui::Text("jumpCount:%d", jumpCount);
            // ジャンプリミット
            ImGui::InputInt("jumpLimit", &jumpLimit);
        };
    }
    ImGui::End();
}

// デバックプリミティブ描画[05]
void Player::DarwDebugPrimitive()
{
    // [05]
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバック球を描画[05]
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 衝突判定用のデバッグ円柱を描画[06]
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 攻撃衝突用の左手ノードのデバッグ球を描画
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRender->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadiuns,
            DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
        );
    }
}

// 待機ステートへ遷移[18]
void Player::TransitionIdleState()
{
    state = State::Idle;

    // 待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理[18]
void Player::UpdateIdleState(float elpsedTime)
{
    // 移動入力処理
    if (InputMove(elpsedTime))
    {
        // 移動入力されたら移動ステートへ遷移
        TransitionMoveState();
    }

    // ジャンプ入力処理
    if (InputJump())
    {
        // ジャンプ入力されたらジャンプステートへ遷移
        TransitionJumpState();
    }

    // 攻撃入力処理
    if (InputAttack())
    {
        TransitionAattackState();
    }
}

// 移動ステートへ遷移[18]
void Player::TransitionMoveState()
{
    state = State::Move;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}

// 移動ステート更新処理[18]
void Player::UpdateMoveState(float elpsedTime)
{
    // 移動入力処理
    if (!InputMove(elpsedTime))
    {
        // 移動入力されなかったら移動ステートへ遷移
        TransitionIdleState();
    }

    // ジャンプ入力処理
    if (InputJump())
    {
        // ジャンプ入力されたらジャンプステートへ遷移
        TransitionJumpState();
    }
}

// ジャンプステートへ遷移[18]
void Player::TransitionJumpState()
{
    state = State::Jump;

    // ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

// ジャンプステート更新処理[18]
void Player::UpdateJumpState(float elpsedTime)
{
    // 移動入力処理
    InputMove(elpsedTime);

    // ジャンプ入力処理
    if (InputJump())
    {
        // 再びジャンプ入力されたらジャンプフリップ再生
        model->PlayAnimation(Anim_Jump_Flip, false);
    }
    // ジャンプアニメーション終了後
    if (!model->IsPlayAnimation())
    {
        // 落下アニメーション再生
        model->PlayAnimation(Anim_Falling,true);
    }
}

// 着地ステートへ遷移[18]
void Player::TransitionLandState()
{
    state = State::Land;

    jumpCount = 0;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Landig, false);
}

// 着地ステート更新処理[18]
void Player::UpdateLandState(float elpsedTime)
{
    // アニメーション再生が終わったら移動ステートへ遷移
    if (model->IsPlayAnimation() == false)
    {
            TransitionIdleState();
    }
}

// 攻撃入力処理
bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }

    return false;
}

void Player::TransitionAattackState()
{
    state = State::Attack;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elpsedTime)
{
    // アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    // 任意のアニメーション再生区間のみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    float attackStartTime = 0.3f;
    float attackEndTime = 0.4f;
    attackCollisionFlag =
        (animationTime >= attackStartTime) &&
        (animationTime <= attackEndTime);

    if (attackCollisionFlag)
    {
    }
}

// ダメージステートに遷移
void Player::TranitionDamageState()
{
    state = State::Damage;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

// ダメージステート更新処理
void Player::UpdateDamageState(float elapseedTime)
{
    // ダメージアニメーションが終わったら
    if(!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

// 死亡ステートに遷移
void Player::TranitionDeathState()
{
    state = State::Death;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

// 死亡ステート更新処理
void Player::UpdateDeathState(float elapseedTime)
{
    // 死亡アニメーションが終わったら
    if (!model->IsPlayAnimation())
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        if(gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TranitionRevivehState();
        }
    }
}

// 復活ステートに遷移
void Player::TranitionRevivehState()
{
    state = State::Death;

    // 体力回復
    health = maxHealth;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_Revive, false);
}

// 復活ステート更新処理
void Player::UpdateReviveState(float elapseedTime)
{
    // 復活アニメーションが終わったら
    if (!model->IsPlayAnimation())
    {
        // 待機ステートに遷移
        TransitionIdleState();
    }
}

// ジャンプ入力処理[06]
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // ジャンプ回数制限[06]
        if (jumpCount < jumpLimit)
        {
            // ジャンプ[06]
            ++jumpCount;
            Jump(JumpSpeed);

            //　ジャンプ入力した
            return true;
        }
    }
    return false;
}

// 着地したときに呼ばれる[06]
void Player::OnLandig()
{
    // ダメージ、死亡ステート時は着地した時にステート遷移しないようにする
    if (state != State::Damage && state != State::Death)
    {
        // 着地ステートへ遷移
        TransitionLandState();
    }
}

// ダメージを受けた時に呼ばれる処理
void Player::OnDamaged()
{
    // ダメージステートへ遷移
    TranitionDamageState();
}

// 死亡した時に呼ばれる処理
void Player::OnDead()
{
    // 死亡ステートへ遷移
    TranitionDeathState();
}