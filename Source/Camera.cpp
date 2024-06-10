#include "Camera.h"

// 指定方向を向く[02]
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus
    , const DirectX::XMFLOAT3& up)
{
    // 視点、中視点、上方向からビューを行列を作成[02]
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye,Focus,Up);
    DirectX::XMStoreFloat4x4(&view, View);

    // ビューを逆行列化じ、ワールド行列に戻す[03]
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    // カメラ方向を取り出す[03]
    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

    // 視点、注視点を保存[03]
    this->eye = eye;
    this->focus = focus;
}

// パーススペクティブ設定[02]
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    // が書く、画面比率、クリップ距離からプロジェクション行列を作成[02]
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearX, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}
