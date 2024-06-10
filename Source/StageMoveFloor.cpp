#include "StageMoveFloor.h"
#include "Collision.h"

// �R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    // �X�e�[�W���f���ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
}

// �f�X�g���N�^
StageMoveFloor::~StageMoveFloor()
{
    // �X�e�[�W���f���j��
    delete model;
}

// �X�V����
void StageMoveFloor::Update(float elapasedTIme)
{
    // �O��̏���ۑ�
    oldTransform = transform;
    oldAngle = angle;

    // �X�^�[�g����S�[���܂ł̋������Z�o����
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    // �X�^�[�g����S�[���܂ł̊Ԃ�1�b�ԂŐi�ފ���(0.0�`1.0)���Z�o����
    float speed = moveSpeed * elapasedTIme;
    float speedRate = speed / length;
    moveRate += speedRate;

    // �S�[���ɓ��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    // ���`��Ԃňʒu���Z�o����
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    // ��]
    angle.x += torque.x * elapasedTIme;
    angle.y += torque.y * elapasedTIme;
    angle.z += torque.z * elapasedTIme;

    // �s��X�V
    UpdateTransform();

    // ���C�L���X�g�p�Ƀ��f����ԍs��ɂ��邽�߂̒P�ʍs���n��
    const DirectX::XMFLOAT4X4 transformIdentity = { 1.0f,0.0f,0.0f ,0.0f  ,0.0f ,1.0f ,0.0f ,0.0f  ,0.0f ,0.0f ,1.0f ,0.0f  ,0.0f ,0.0f ,0.0f ,1.0f };
    model->UpdateTransform(transformIdentity);
}

// �`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // �\���p�̂��߃��[���h�s��ɍX�V����
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

// ���C�L���X�g
bool StageMoveFloor::RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    // �O��̃��[���h�s��Ƌt�s������߂�
    DirectX::XMMATRIX oldWolrdTransform = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX inverseOldWolrdTransform = DirectX::XMMatrixInverse(nullptr, oldWolrdTransform);
    
    // �O��̃��[�J����Ԃł̃��C�ɕϊ�
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, inverseOldWolrdTransform);      // WorldStart��O��̃��[�J���s��ɍ��킹��3D�x�N�g���ɕϊ�
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, inverseOldWolrdTransform);        // WorldEnd��O��̃��[�J���s��ɍ��킹��3D�x�N�g���ɕϊ�

    // ���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
    DirectX::XMFLOAT3 localStart, localEnd;

    DirectX::XMStoreFloat3(&localStart, LocalStart);
    DirectX::XMStoreFloat3(&localEnd, LocalEnd);
    
    HitResult locakHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, locakHit))
    {
        // �O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
        // �O�񂩂獡��ɂ����ĕύX���ꂽ���p���S�ď���Ă���I�u�W�F�N�g�ɔ��f�����

        // ���݂̃��[���h���W
        DirectX::XMMATRIX WolrdTransform = DirectX::XMLoadFloat4x4(&this->transform);
        // ��_
        DirectX::XMVECTOR HitPositon = DirectX::XMLoadFloat3(&locakHit.position);

        DirectX::XMVECTOR WolrdHitPositon = XMVector3Transform(HitPositon, WolrdTransform);


        // HitResult�ɕۑ�
        DirectX::XMStoreFloat3(&locakHit.position, WolrdHitPositon);

        hit = locakHit;

        // ��]�������Z�o
        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;
        return true;
    }
    return false;
}

// �s��X�V����
void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
