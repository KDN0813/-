#include "Character.h"
#include <algorithm>
#include "Collision.h"
#include "Mathf.h"
#include "StageManager.h"

// �s��X�V����
void Character::UpdateTransform()
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    //DirectX::XMMATRIX R= DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;

    // �ʒu�s����쐬
    DirectX::XMMATRIX T= DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // 3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �_���[�W��^����
bool Character::ApplyDamage(int damage, float invincbleTime)
{
    // �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    // ���G���Ԓ��͌��N��Ԃ��X�V���Ȃ�
    if (invincibleTimer > 0.0f) return false;

    // ���S���Ă�ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;

    // �_���[�W����
    health -= damage;

    // ���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
    }

    // �_���[�W��^���������G���Ԃ�ݒ�
    invincibleTimer = invincbleTime;

    // ���N��Ԃ�ύX�����ꍇ��true��Ԃ�
    return true;
}

// �Ռ���^����[09]   Impulse(�Ռ�)
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // ���͂ɗ͂�^����(���ȉ�)
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// �ړ�����[03]
void Character::Move(float vx, float vz, float speed)
{
    // �ړ������x�N�g����ݒ�[09]
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�ݒ�[09]
    maxMoveSpeed = speed;
}

// ���񏈗�[03]
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    // [03]
    speed *= elapsedTime;

    // �i�s�x�N�g�����[���̏ꍇ�͏�������K�v�Ȃ�[03]
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001) return;

    // �i�s�x�N�g����P�ʃx�N�g����[03]
    vx /= length;
    vz /= length;

    // ���g�̉�]�l����O���������߂�[03]
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    // ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (frontX * vx + frontZ * vz);

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������قǁA1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;

    // ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����[03]
    float cross = (frontZ * vx) - (frontX * vz);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //  ���E������s�����Ƃɂ���č��E��]��I������[03]
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
}

// �W�����v����[06]
void Character::Jump(float speed)
{
    // ������̗͂�ݒ�
    velocity.y = speed;
}

// ���x�����X�V[06]
void Character::UpdateVelocity(float elaspsedTime)
{
    // �o�߃t���[��[06]
    float elapsedFrame = 60.0f * elaspsedTime;  // �Q�[����n�{���œ������Ƃ��ɒ�������킹��ׂ̕ϐ�

    // �������͍X�V����[07]
    UpdateVerticalVelocity(elapsedFrame);

    // �������͍X�V����[09](���ȉ�)
    UpdateHorizontalVelocity(elapsedFrame);

    // �����ړ��X�V����[09]
    UpdateVerticalMove(elaspsedTime);

    // �����ړ��X�V����[09](���ȉ�)
    UpdateHorizontalMove(elaspsedTime);
}

// ���G���ԍX�V[08]
void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

// �������͍X�V����[08]
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    // �d�͍X�V����(���ȉ�)
    velocity.y += Gravity * elapsedFrame;
}

// �����ړ��X�V����[08]
void Character::UpdateVerticalMove(float elapsedTime)
{
    // ���������̈ړ���
    float my = velocity.y * elapsedTime;

    // �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0.0f,1.0f,0.0f };

    // ������
    if (my < 0.0f) 
    {
        // ���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start     = { position.x,position.y + step0ffset,position.z };
        // ���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end       = { position.x,position.y + my,position.z };

        // ���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // �@���x�N�g���擾
            normal = hit.normal;

            // �n�ʂɐڒn���Ă���
            position = hit.position;

            // ��]
            angle.x += hit.rotation.x;
            angle.y += hit.rotation.y;
            angle.z += hit.rotation.y;

            // �X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // ���n����
            if (!isGround)
            {
                OnLandig();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // �󒆂ɂ���
            position.y += my;
            isGround = false;
        }
    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // �n�ʂ̌����ɉ����悤��XZ����]
    {
        // Y�����@���x�N�g�������𐁂��I�C���[�p��]���Z�o����
        float ax = atan2f(normal.z, normal.y);
        float az = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, ax, 0.2f);
        angle.z = Mathf::Lerp(angle.z, az, 0.2f);
    }
}

// �������͍X�V����[09]
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZ���ʂ̑��͂���������[09]
    // (���ȉ�)
    float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
    if (length > 0.0f)
    {
        // ���C��
        float friction = this->friction * elapsedFrame;

        // �󒆂ɂ���Ƃ��͖��C�͂����炷[09]
        // (���ȉ�)
        if (!isGround) friction *= airControl;

        // ���C�ɂ�鉡�����̌�������[09]
        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���[09]
        else
        {
            // (���ȉ�)
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    // XZ���ʂ̑��͂���������[09]
    // (��)���͂̍��v���ő呬�x�ȉ��Ȃ�
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������[09]
        // (��)�����͂̍��v(�����͂����邩���ʂ��邾���Ȃ̂�[sqrtf]�͎g��Ȃ�)
        float moveVecLength = ((moveVecX * moveVecX) + (moveVecZ * moveVecZ)); //(���ȉ� p8)
        if (moveVecLength > 0.0f)
        {
            // ������(acceleration<�G�N�T���[�V����>:������)
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͉����͂����炷[09]
            // (���ȉ�)
            if(!isGround) acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������(���ȉ� p9)
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // �ő呬�x����[09]
            // (��)���x���Z��ɍő呬�x�𒴂��Ă����
            length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            // �����ŃK�^�K�^���Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    // �ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

// �����ړ��X�V����[09]
void Character::UpdateHorizontalMove(float elapsedTime)
{
    // �������͌v�Z[13]
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ�[13]
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu[13]
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffset,position.z };
        DirectX::XMFLOAT3 end   = { position.x + mx,position.y + step0ffset,position.z + mz };

        // ���C�L���X�g�ǔ���[13]
        HitResult hit;
        if (StageManager::Instance().RayCact(start, end, hit))
        {
            // �ǂ��烌�C�̏I�_�܂ł̃x�N�g��[13]
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End   = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec   = DirectX::XMVectorSubtract(End, Start);

            // �ǂ̖@��[13]
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // ���˃x�N�g����@���Ɏˉe[13]
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f);

            // �␳�ʒu�̌v�Z[13]
            DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 correctionPositon;
            DirectX::XMStoreFloat3(&correctionPositon, CorrectionPositon);

            // �ǂ�������ւ̃��C�L���X�g
            HitResult hit2;
            if (!StageManager::Instance().RayCact(start, correctionPositon, hit2))
            {
                // �ǂ�������ŕǂɓ�����Ȃ�������
                position.x = correctionPositon.x;
                position.z = correctionPositon.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            position.x += mx;
            position.z += mz;
        }
    }
}
