#pragma once
#include <DirectXMath.h>

// キャラクター
class Character
{
public:
    Character() = default;
    virtual ~Character() {};

    // 行列更新処理
    void UpdateTransform();

    // 位置取得
    const DirectX::XMFLOAT3& GetPosition()const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 回転取得
    const DirectX::XMFLOAT3& GetAngle()const { return angle; }

    // 回転設定
    void SerAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // スケール取得
    const DirectX::XMFLOAT3& GetScale()const { return scale; }

    // スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // 半径取得[05]
    float GetRadius() const { return radius; }

    // 地面に接しているか[06]
    bool IsGround() const { return isGround; }

    // 高さ取得[06]
    float GetHeigh() const { return height; }

    // ダメージを与える[07]
    bool ApplyDamage(int damage, float invincbleTime);

    // 衝撃を与える[09]   Impulse(衝撃)
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // 健康状態を取得[13]
    int GetHealth() const { return health; }

    // 最大健康状態を取得[13]
    int GetMaxHealth() const { return maxHealth; }
protected:
    // 移動処理[03][06]
    void Move(float vx, float vz, float speed);


    // 旋回処理[03][06]
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // ジャンプ処理[06]
    void Jump(float speed);

    // 速度処理更新[06]
    void UpdateVelocity(float elaspsedTime);

    // 着地したら呼ばれる[06]
    virtual void OnLandig(){}

    // ダメージを受けた前に呼ばれる[07]
    virtual void OnDamaged(){}

    // 死亡した時に呼ばれる[07]
    virtual void OnDead(){}

    // 無敵時間更新[08]
    void UpdateInvincibleTimer(float elapsedTime);
private:
    // 垂直速力更新処理[09]
    void UpdateVerticalVelocity(float elapsedFrame);

    // 垂直移動更新処理[09]
    void UpdateVerticalMove(float elapsedTime);

    // 水平速力更新処理[09]
    void UpdateHorizontalVelocity(float elapsedFrame);

    // 水平移動更新処理[09]
    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT3 position      = { 0,0,0 };
    DirectX::XMFLOAT3 angle         = { 0,0,0 };
    DirectX::XMFLOAT3 scale         = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform   = {
                                    1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0,
                                    0,0,0,1,
                                    };

    // [05]
    float radius                    = 0.5f;

    //[06]
    float Gravity                   = -1.0f;
    DirectX::XMFLOAT3 velocity      = { 0.0f,0.0f,0.0f };
    bool isGround                   = false;
    float height                    = 2.0f;

    // [08]
    float invincibleTimer           = 0.0f;     // 無敵時間

    // friction(摩擦)[09]
    float friction                  = 0.5f;

    // [09]
    float acceleration              = 1.0f; // acceleration:加速度
    float maxMoveSpeed              = 5.0f;
    float moveVecX                  = 0.0f;
    float moveVecZ                  = 0.0f;
    float airControl                = 0.3f; // 空中でのコントロール力

    // [12]
    float step0ffset                = 1.0f;
    float                           slopeRate = 1.0f;   // 傾斜率

    // [13]
    int health                      = 5;
    int maxHealth                   = 5;
};