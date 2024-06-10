#include "ProjectileStraight.h"
#include "StageManager.h"
#include "Collision.h"

// �R���X�g���N�^[07]
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
    :Projectile(manager)    // ���N���X�̃R���X�g���N�^���Ăяo��
{
    // [07]
    model = new Model("Data/Model/Sword/Sword.mdl");

    // �\���T�C�Y�𒲐�[07]
    scale.x = scale.y = scale.z = 3.0f;
}

// �f�X�g���N�^[07]
ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

// �X�V����[07]
void ProjectileStraight::Update(float elapsedTime)
{
    // ��������[07]
    LifeTImer -= elapsedTime;
    if (LifeTImer < 0)
    {
        // �������폜
        Destroy();
    }

    // �ړ�[07]
    this->UpdateHorizontalMove(elapsedTime);

    // �I�u�W�F�N�g�s����X�V[07]
    UpdateTransform();

    // ���f���s��X�V[07]
    model->UpdateTransform(transform);
}

// �`�揈��[07]
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// ����[07]
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

void ProjectileStraight::UpdateHorizontalMove(float elapsedTime)
{
    // �������͌v�Z[13]
    DirectX::XMFLOAT2 velocity = { this->direction.x * this->speed,this->direction.z * this->speed };
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ�[13]
        float mx = velocity.x * elapsedTime;
        float mz = velocity.y * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu[13]
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,position.y + step0ffset,position.z + mz };

        // ���C�L���X�g�ǔ���[13]
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // �ǂ��烌�C�̏I�_�܂ł̃x�N�g��[13]
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // �ǂ̖@��[13]
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // ���˃x�N�g����@���Ɏˉe[13]
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 2.0f);

            // �␳�ʒu�̌v�Z[13]
            DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 correctionPositon;
            DirectX::XMStoreFloat3(&correctionPositon, CorrectionPositon);

            // �����v�Z
            DirectX::XMVECTOR Direction = DirectX::XMVectorSubtract(CorrectionPositon, Start);
            Direction = DirectX::XMVector3Normalize(Direction);

            // �ǂ�������ւ̃��C�L���X�g
            HitResult hit2;
            if (!StageManager::Instance().RayCact(start, correctionPositon, hit2))
            {
                // �ǂ�������ŕǂɓ�����Ȃ�������
                position.x = correctionPositon.x;
                position.z = correctionPositon.z;

                DirectX::XMStoreFloat3(&this->direction, Direction);
                this->direction.y = 0.0f;
                
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;

                DirectX::XMStoreFloat3(&this->direction, Direction);
                this->direction.y = 0.0f;
            }
        }
        else
        {
            position.x += mx;
            position.z += mz;
        }
    }
}
