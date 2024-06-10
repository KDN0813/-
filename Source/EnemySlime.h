#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

// [04]
class EnemySlime: public Enemy
{
public:
    EnemySlime();// [04]
    ~EnemySlime() override;// [04]

    // �X�V����[04]
    void Update(float elapseedTime) override;

    // �`�揈��[04]
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // �f�o�b�O�pGUI�`��[org]
    void DrawDebugGUI();

    // ���S�������ɌĂ΂��[08]
    void OnDead() override;

    // �f�o�b�N�v���~�e�B�u�`��
    void DarwDebugPrimitive() override;

    // �^�[�Q�b�g�ʒu�ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin,float range);

protected:
    // �_���[�W���󂯂����ɌĂ΂��
    void OnDamaged() override;

private:
    // �^�[�Q�b�g�ʒu�������_���w��
    void SetRandaomTargetPosition();

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapseedTime, float speedRate);;

    // �v���C���[�̒T��
    bool SearchPlayer();

    // �m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

    // ==========================
    // �X�e�[�g
    // ==========================

    // �p�j�X�e�[�g�֑J��
    void TranitionWanderState();
    // �p�j�X�e�[�g�X�V����
    void UpdateWanderState(float elapseedTime);

    // �p�j�X�e�[�g�֑J��
    void TranitionIdelState();
    // �p�j�X�e�[�g�X�V����
    void UpdateIdelState(float elapseedTime);

    // �ǐՃX�e�[�g�֑J��
    void TranitionPursuitState();
    // �ǐՃX�e�[�g�X�V����
    void UpdatePursuitState(float elapseedTime);

    // �U���X�e�[�g�֑J��
    void TranitionAttackState();
    // �U���X�e�[�g�X�V����
    void UpdateAttackState(float elapseedTime);

    // �퓬�ҋ@�X�e�[�g�֑J��
    void TranitionBattleState();
    // �퓬�ҋ@�X�e�[�g�X�V����
    void UpdateIdelBattleState(float elapseedTime);

    // �_���[�W�X�e�[�g�ɑJ��
    void TranitionDamageState();
    // �_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapseedTime);

    // ���S�X�e�[�g�ɑJ��
    void TranitionDeathState();
    // ���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapseedTime);

private:
    // �X�e�[�g
    enum class State
    {
        Wander,
        Idel,
        Pursuit,
        Attack,
        IdelBattle,
        Damage,
        Death,

        max,
    };

    // �A�j���[�V����
    enum Animation
    {
        Anim_IdelNormal,
        Anim_IdleBattle,
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalkRight,
        Anim_RundFWD,
        Anim_SceneSomthingST,
        Anim_SceneSomthingPRT,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetGit,
        Anim_Dizzy,
        Anim_Die,

        max,
    };

private:
    // [04]
    Model* model = nullptr;

    State state = State::Wander;
    DirectX::XMFLOAT3 targetPositon = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 territoryOrgin{ 0.0f,0.0f,0.0f };
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360.0f);

    float stateTimer = 0.0f;

    float searcRange = 5.0f;

    float attackRange = 1.5f;
};

