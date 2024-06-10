#pragma once
#include <DirectXMath.h>

// �L�����N�^�[
class Character
{
public:
    Character() = default;
    virtual ~Character() {};

    // �s��X�V����
    void UpdateTransform();

    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition()const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ��]�擾
    const DirectX::XMFLOAT3& GetAngle()const { return angle; }

    // ��]�ݒ�
    void SerAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // �X�P�[���擾
    const DirectX::XMFLOAT3& GetScale()const { return scale; }

    // �X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // ���a�擾[05]
    float GetRadius() const { return radius; }

    // �n�ʂɐڂ��Ă��邩[06]
    bool IsGround() const { return isGround; }

    // �����擾[06]
    float GetHeigh() const { return height; }

    // �_���[�W��^����[07]
    bool ApplyDamage(int damage, float invincbleTime);

    // �Ռ���^����[09]   Impulse(�Ռ�)
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // ���N��Ԃ��擾[13]
    int GetHealth() const { return health; }

    // �ő匒�N��Ԃ��擾[13]
    int GetMaxHealth() const { return maxHealth; }
protected:
    // �ړ�����[03][06]
    void Move(float vx, float vz, float speed);


    // ���񏈗�[03][06]
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // �W�����v����[06]
    void Jump(float speed);

    // ���x�����X�V[06]
    void UpdateVelocity(float elaspsedTime);

    // ���n������Ă΂��[06]
    virtual void OnLandig(){}

    // �_���[�W���󂯂��O�ɌĂ΂��[07]
    virtual void OnDamaged(){}

    // ���S�������ɌĂ΂��[07]
    virtual void OnDead(){}

    // ���G���ԍX�V[08]
    void UpdateInvincibleTimer(float elapsedTime);
private:
    // �������͍X�V����[09]
    void UpdateVerticalVelocity(float elapsedFrame);

    // �����ړ��X�V����[09]
    void UpdateVerticalMove(float elapsedTime);

    // �������͍X�V����[09]
    void UpdateHorizontalVelocity(float elapsedFrame);

    // �����ړ��X�V����[09]
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
    float invincibleTimer           = 0.0f;     // ���G����

    // friction(���C)[09]
    float friction                  = 0.5f;

    // [09]
    float acceleration              = 1.0f; // acceleration:�����x
    float maxMoveSpeed              = 5.0f;
    float moveVecX                  = 0.0f;
    float moveVecZ                  = 0.0f;
    float airControl                = 0.3f; // �󒆂ł̃R���g���[����

    // [12]
    float step0ffset                = 1.0f;
    float                           slopeRate = 1.0f;   // �X�Η�

    // [13]
    int health                      = 5;
    int maxHealth                   = 5;
};