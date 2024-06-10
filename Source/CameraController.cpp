#include <imgui.h>//[org]
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

// 更新処理[02]
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    // カメラの回転速度[02]
    float speed = rollSpeed * elapsedTime;
    
    // スティック入力値に合わせてX軸とY軸を回転[02]
    angle.x += ay * speed;
    angle.y += ax * speed;
    
    // X軸のカメラの回転を制限[02]
    if (angle.x > maxAngleX) angle.x = maxAngleX;
    if (angle.x < minAngleX) angle.x = minAngleX;
    
    // Y軸の回転地を-3.14～3.14に終わ丸用にする[02]
    if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI) angle.y -= DirectX::XM_2PI;

    // カメラ回転値を回転行列に変換[02]
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方ベクトルを取り出す[02]
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろのベクトル方向に一定離れたカメラ視点を求める[02]
    DirectX::XMFLOAT3 eye;

    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // カメラの視点と注視点を設定[02]
    Camera::Intance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

// デバッグ用GUI描画[org]
void CameraController::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // 角度[org]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle2", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // ターゲット[org]
            ImGui::InputFloat3("Target", &target.x);
            // 回転速度[org]
            float b = DirectX::XMConvertToDegrees(rollSpeed);
            ImGui::InputFloat("rollSpeed", &b);
            rollSpeed = DirectX::XMConvertToRadians(b);
            // range[org]
            ImGui::InputFloat("range", &range);
            // minAngleX[org]
            float c = DirectX::XMConvertToDegrees(minAngleX);
            ImGui::InputFloat("minAngleX", &c);
            minAngleX = DirectX::XMConvertToRadians(c);
            // maxAngleX[org]
            float d = DirectX::XMConvertToDegrees(maxAngleX);
            ImGui::InputFloat("maxAngleX", &d);
            maxAngleX = DirectX::XMConvertToRadians(d);
        };
    }
    ImGui::End();
}
