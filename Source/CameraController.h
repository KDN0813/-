#pragma once
#include <DirectXMath.h>

// カメラコントローラー[02]
class CameraController
{
public:
    //[02]
    CameraController(){}
    ~CameraController(){}

    // 更新処理[02]
    void Update(float elapsedTime);

    // ターゲット位置設定[02]
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    // デバッグ用GUI描画[org]
    void DrawDebugGUI();

    // 健康状態を取得[13]
    int GetHealth()const { return health; }

    // 最大健康状態を取得[13]
    int GetMaxHealth() const { return maxHealth; }


protected:

    // [13]
    int health                  = 5;
    int maxHealth               = 5;
private:
    //[02]
    DirectX::XMFLOAT3 target    = { 0,0,0 };
    DirectX::XMFLOAT3 angle     = { 0,0,0 };
    float rollSpeed             = DirectX::XMConvertToRadians(90);
    float range                 = 10.0f;

    float maxAngleX             = DirectX::XMConvertToRadians(45);
    float minAngleX             = DirectX::XMConvertToRadians(-45);
};

