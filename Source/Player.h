#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"

#include "Effect.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;

    // �C���X�^���X�擾
    static Player& Instance();

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    // �f�o�b�N�v���~�e�B�u�`��[05]
    void DarwDebugPrimitive();

protected:
    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾[03]
    DirectX::XMFLOAT3 GetMoveVec() const;

    // �ړ����͏���[03]
    bool InputMove(float elapsedTime);

    // �W�����v���͏���[06]
    bool InputJump();

    // ���n�����Ƃ��ɌĂ΂��[06]
    void OnLandig() override;

    // �_���[�W���󂯂����ɌĂ΂�鏈��
    void OnDamaged() override;

    // ���S�������ɌĂ΂�鏈��
    void OnDead() override;

private:
    // �ҋ@�X�e�[�g�֑J��[18]
    void TransitionIdleState();

    // �ҋ@�X�e�[�g�X�V����[18]
    void UpdateIdleState(float elpsedTime);

    // �ړ��X�e�[�g�֑J��[18]
    void TransitionMoveState();

    // �ړ��X�e�[�g�X�V����[18]
    void UpdateMoveState(float elpsedTime);
    
    // �W�����v�X�e�[�g�֑J��[18]
    void TransitionJumpState();

    // �W�����v�X�e�[�g�X�V����[18]
    void UpdateJumpState(float elpsedTime);

    // ���n�X�e�[�g�֑J��[18]
    void TransitionLandState();

    // ���n�X�e�[�g�X�V����[18]
    void UpdateLandState(float elpsedTime);

    // �U�����͏���[19]
    bool InputAttack();

    // �U���X�e�[�g�֑J��[19]
    void TransitionAattackState();

    // �U���X�e�[�g�X�V����[19]
    void UpdateAttackState(float elpsedTime);

    // �_���[�W�X�e�[�g�ɑJ��
    void TranitionDamageState();
    // �_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapseedTime);

    // ���S�X�e�[�g�ɑJ��
    void TranitionDeathState();
    // ���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapseedTime);

    // �����X�e�[�g�ɑJ��
    void TranitionRevivehState();
    // �����X�e�[�g�X�V����
    void UpdateReviveState(float elapseedTime);

private:
    // �X�e�[�g[18]
    enum class State
    {
        Idle
        ,Move
        ,Jump
        ,Land
        ,Attack
        ,Damage
        ,Death
        ,Revive
    };

    // �A�j���[�V����[18]
    enum Anination
    {
        Anim_Attack
        , Anim_Death
        , Anim_Falling
        , Anim_GetHit1
        , Anim_GetHit2
        , Anim_Idle
        , Anim_Jump
        , Anim_Jump_Flip
        , Anim_Landig
        , Anim_Revive
        , Anim_Running
        , Anim_Walking
    };

private:
    Model* model        = nullptr;

    //[03]
    float moveSpeed     = 5.0f;
    float turnSpeed     = DirectX::XMConvertToRadians(720);

    //[06]
    float JumpSpeed     = 20.0f;
    int jumpCount       = 0;
    int jumpLimit       = 3;

    // [10]
    Effect*hitEffect    = nullptr;

    // [18]
    State state         = State::Idle;

    // [19]
    float leftHandRadiuns = 0.4f;
    bool attackCollisionFlag = false;
};