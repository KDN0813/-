#pragma once

#include <DirectXMath.h>

// カメラ[02]
class Camera
{
private:
    Camera(){}
    ~Camera(){}

public:
    // 唯一のインスタンス取得[02]
    static Camera& Intance()
    {
        static Camera camera;
        return camera;
    }

    // 指定方向を向く[02]
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
        const DirectX::XMFLOAT3& up);

    // パーススペクティブ設定[02]
    void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);

    // ビュー行列取得[02]
    const DirectX::XMFLOAT4X4& GetView()const { return view; }

    // プロジェクション行列取得[02]
    const DirectX::XMFLOAT4X4& getProjection()const { return projection; }

    // 視点取得[03]
    const DirectX::XMFLOAT3& GetEye()const { return eye; }

    // 注視点取得[03]
    const DirectX::XMFLOAT3& GetFocus()const { return focus; }

    // 上方向取得[03]
    const DirectX::XMFLOAT3& GetUp()const { return up; }

    // 前方向取得[03]
    const DirectX::XMFLOAT3& GetFront()const { return front; }

    // 右方向取得[03]
    const DirectX::XMFLOAT3& GetRIght()const { return right; }

private:
    //[02]
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    //[03]
    DirectX::XMFLOAT3 eye;
    DirectX::XMFLOAT3 focus;

    DirectX::XMFLOAT3 up;
    DirectX::XMFLOAT3 front;
    DirectX::XMFLOAT3 right;
};
