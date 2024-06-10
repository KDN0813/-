#pragma once

#include <DirectXMath.h>

// �J����[02]
class Camera
{
private:
    Camera(){}
    ~Camera(){}

public:
    // �B��̃C���X�^���X�擾[02]
    static Camera& Intance()
    {
        static Camera camera;
        return camera;
    }

    // �w�����������[02]
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
        const DirectX::XMFLOAT3& up);

    // �p�[�X�X�y�N�e�B�u�ݒ�[02]
    void SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ);

    // �r���[�s��擾[02]
    const DirectX::XMFLOAT4X4& GetView()const { return view; }

    // �v���W�F�N�V�����s��擾[02]
    const DirectX::XMFLOAT4X4& getProjection()const { return projection; }

    // ���_�擾[03]
    const DirectX::XMFLOAT3& GetEye()const { return eye; }

    // �����_�擾[03]
    const DirectX::XMFLOAT3& GetFocus()const { return focus; }

    // ������擾[03]
    const DirectX::XMFLOAT3& GetUp()const { return up; }

    // �O�����擾[03]
    const DirectX::XMFLOAT3& GetFront()const { return front; }

    // �E�����擾[03]
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
