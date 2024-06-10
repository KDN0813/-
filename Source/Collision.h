#pragma once
#include <DirectXMath.h>
#include "Graphics/Model.h"

/// <summary>
/// �q�b�g����[12]
/// </summary>
/// <param name="position">���C�Ƃƃ|���S���̌�_</param>
/// <param name="normal">�Փ˂����|���S���̖@���x�N�g��</param>
/// <param name="rotation">��]��</param>
/// <param name="distance">���C�̎n�_�����_�܂ł̋���</param>
/// <param name="materialIndex">�Փ˂����|���S���̃}�e���A���ԍ�</param>
struct HitResult
{
    DirectX::XMFLOAT3   position        = {};
    DirectX::XMFLOAT3   normal          = {};
    DirectX::XMFLOAT3   rotation          = {};
    float               distance        = 0.0f;
    int                 materialIndex   = -1;
};

// �R���W����[05]
class Collision
{
public:
    // ���Ƌ��̌�������[05]
    static bool IntersectSphareVsSphere(
        const DirectX::XMFLOAT3& positionA
        , float radiusA
        , const DirectX::XMFLOAT3& positionB
        , float radiusB
        , DirectX::XMFLOAT3& outPositionB
    );

    // �~���Ɖ~���̌�������[06]
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA
        , float radiusA
        , float heightA
        , const DirectX::XMFLOAT3& positionB
        , float radiusB
        , float heightB
        , DirectX::XMFLOAT3& outPositionB
    );

    // ���Ɖ~���̌�������[08]
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition
        , float sphereRadius
        , const DirectX::XMFLOAT3& cylinderPosition
        , float cylinderRadius
        , float cylinderHeight
        , DirectX::XMFLOAT3& outCylinderPosition
    );

    /// <summary>
    /// ���C�ƃ��f���̌�������
    /// </summary>
    /// <param name="start">���C�̎n�_</param>
    /// <param name="end">���C�̏I�_</param>
    /// <param name="model">���f���̃|�C���^</param>
    /// <param name="result">�q�b�g���U���g(���ʂ��i�[����)</param>
    /// <returns>��������(����������true)</returns>
    static bool IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};

