#include <imgui.h>
#include "EnemySlime.h"
#include "Mathf.h"
#include "Collision.h"
#include "Graphics/Graphics.h"
#include "Player.h"

// �R���X�g���N�^[04]
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    // ���A�����ݒ�[06]
    radius = 0.5f;
    height = 1.0f;

    // �p�j�X�e�[�g�֑J��
    TranitionWanderState();
}

// �f�X�g���N�^[04]
EnemySlime::~EnemySlime()
{
    delete model;
}

// �X�V����[04]
void EnemySlime::Update(float elapseedTime)
{
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapseedTime);
        break;
    case State::Idel:
        UpdateIdelState(elapseedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapseedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapseedTime);
        break;
    case State::IdelBattle:
        UpdateIdelBattleState(elapseedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapseedTime);
        break;
    case State::Death:
        UpdateDeathState(elapseedTime);
        break;
    }

    // ���x�����X�V
    UpdateVelocity(elapseedTime);

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapseedTime);

    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    // �A�j���[�V�����X�V
    model->UpdateAnimation(elapseedTime);

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

// �`�揈��[04]
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// �f�o�b�O�pGUI�`��[org]
void EnemySlime::DrawDebugGUI()
{
    // �ʒu
    ImGui::InputFloat3("Position", &position.x);
    // ���a
    ImGui::InputFloat("Radius",&radius);
    // ����
    ImGui::InputFloat("Height",&height);

    // �̗�
    ImGui::InputInt("HP", &health);

    // ���G����
    ImGui::InputFloat("invincibleTimer", &invincibleTimer);
}

// ���S�������ɌĂ΂��[08]
void EnemySlime::OnDead()
{
    // ���S�X�e�[�g�֑J��
    TranitionDeathState();
}

// �f�o�b�N�v���~�e�B�u�`��
void EnemySlime::DarwDebugPrimitive()
{
    Enemy::DarwDebugPrimitive();

    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // �꒣��͈͂��f�o�b�N�~���`��
    debugRender->DrawCylinder(territoryOrgin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

    // �^�[�Q�b�g�ʒu���f�o�b�N���`��
    debugRender->DrawSphere(targetPositon, radius
        , DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

    // �T���͈͂��f�o�b�N�~���`��
    debugRender->DrawCylinder(position, searcRange, 1.0f,
        DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

    // �U���͈͂��f�o�b�N�`��
    debugRender->DrawCylinder(position, attackRange, 1.0f,
        DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}

// �^�[�Q�b�g�ʒu�ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrgin = origin;
    territoryRange = range;
}

// �_���[�W���󂯂����ɌĂ΂��
void EnemySlime::OnDamaged()
{
    // �_���[�W�X�e�[�g�֑J��
    TranitionDamageState();
}

// �^�[�Q�b�g�ʒu�������_���w��
void EnemySlime::SetRandaomTargetPosition()
{
    float angle = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);

    targetPositon.x = territoryOrgin.x + sinf(angle) * range;
    targetPositon.y = territoryOrgin.y;
    targetPositon.z = territoryOrgin.y + cosf(angle) * range;
}

// �ڕW�n�_�ֈړ�
void EnemySlime::MoveToTarget(float elapseedTime, float speedRate)
{
    // �^�[�Q�b�g�����ւ̐i�s�x�N�g��
    float vx = targetPositon.x - position.x;
    float vz = targetPositon.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);

    vx /= dist;
    vz /= dist;

    // �ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapseedTime, vx, vz, turnSpeed * speedRate);
}

// �v���C���[�̒T��
bool EnemySlime::SearchPlayer()
{
    const DirectX::XMFLOAT3& playerPositon
        = Player::Instance().GetPosition();
    float vx = playerPositon.x - position.x;
    float vy = playerPositon.y - position.y;
    float vz = playerPositon.z - position.z;
    float dist = sqrtf((vx * vx) + (vy * vy) + (vz * vz));

    if(dist<searcRange)
    {
        float distXZ = sqrtf((vx * vx) + (vz * vz));
        // �P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;
        // �O���x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);

        // 2�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if(dot>0.0f)
        {
            return true;
        }
    }
    return false;
}

// �m�[�h�ƃv���C���[�̏Փˏ���
void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float boneRadius)
{
    Player& player = Player::Instance();

    // �m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        // �m�[�h�̃��[���h���W�擾
        DirectX::XMFLOAT3 nodePosition;
            nodePosition =
        {
            node->worldTransform._41,
            node->worldTransform._42 ,
            node->worldTransform._43
        };

            // �����蔻��\��
            Graphics::Instance().GetDebugRenderer()->DrawSphere(
                nodePosition, boneRadius, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
            );

            //// �Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                boneRadius,
                player.GetPosition(),
                player.GetRadius(),
                player.GetHeigh(),
                outPosition
            ))
            {
                // �������Ă����
                // �_���[�W��^����
                if (player.ApplyDamage(1, 0.5f))
                {
                    // ������΂�
                    {
                        DirectX::XMFLOAT3 vec = {};
                        vec.x = outPosition.x -= nodePosition.x;
                        vec.z = outPosition.z -= nodePosition.z;
                        float lenght = sqrtf(vec.x * vec.x + vec.z * vec.z);
                        vec.x / lenght;
                        vec.z / lenght;

                        // XZ���ʂɐ�����΂��͂�������
                        float power = 10.0f;
                        vec.x* power;
                        vec.z* power;
                        // Y�����ɂ��͂�������
                        vec.y = 5.0f;

                        // ������΂�
                        player.AddImpulse(vec);
                    }
                }
            }
    }
}

// �p�j�X�e�[�g�֑J��
void EnemySlime::TranitionWanderState()
{
    state = State::Wander;

    // �ړI�n�_�ݒ�
    SetRandaomTargetPosition();

    // �����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_WalkFWD, true);
}

// �p�j�X�e�[�g�X�V����
void EnemySlime::UpdateWanderState(float elapseedTime)
{
    // �ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPositon.x - position.x;
    float vz = targetPositon.z - position.z;
    float distSp = vx * vx + vz * vz;
    if (distSp < radius * radius)
    {
        // �ҋ@�X�e�[�g�ɑJ��
        TranitionIdelState();
    }

    // �v���C���[�T��
    if(SearchPlayer())
    {
        // ����������ǐՃX�e�[�g�֑J��
        TranitionPursuitState();
    }

    // �ڕW�n�_�ֈړ�
    MoveToTarget(elapseedTime, 0.5f);
}

// �p�j�X�e�[�g�֑J��
void EnemySlime::TranitionIdelState()
{
    state = State::Idel;

    // �^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // �ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdelNormal, true);
}

// �ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdelState(float elapseedTime)
{
    // �^�C�}�[����
    stateTimer -= elapseedTime;
    if(stateTimer<0.0f)
    {
        // �p�j�X�e�[�g�֑J��
        TranitionWanderState();
    }

    // �v���C���[�T��
    if (SearchPlayer())
    {
        // ����������ǐՃX�e�[�g�֑J��
        TranitionPursuitState();
    }
}

// �ҋ@�X�e�[�g�֑J��
void EnemySlime::TranitionPursuitState()
{
    state = State::Pursuit;

    // �^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // �����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_RundFWD, true);
}

// �ǐՃX�e�[�g�X�V����
void EnemySlime::UpdatePursuitState(float elapseedTime)
{
    // �ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPositon = Player::Instance().GetPosition();

    // �ڕW�n�_�ֈړ�
    MoveToTarget(elapseedTime, 1.0f);

    // �^�C�}�[����
    stateTimer -= elapseedTime;
    if (stateTimer < 0.0f)
    {
        // �p�j�X�e�[�g�֑J��
        TranitionWanderState();
    }

    // �v���C���[�ɋ߂Â��ƍU���X�e�[�g�֑J��
    float vx = targetPositon.x - position.x;
    float vy = targetPositon.y - position.y;
    float vz = targetPositon.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        // �U���X�e�[�g�֑J��
        TranitionAttackState();
    }
}

// �U���X�e�[�g�֑J��
void EnemySlime::TranitionAttackState()
{
    state = State::Attack;

    // �U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack1, false);
}

// �U���X�e�[�g�X�V����
void EnemySlime::UpdateAttackState(float elapseedTime)
{
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ɏՓˏ���������
    float animationTime = model->GetCurrentAnimationSeconds();
    if(animationTime >=0.1f && animationTime<=0.35f)
    {
        // �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    // �U���A�j���[�V�������I�������
    if(!model->IsPlayAnimation())
    {
        // �퓬�ҋ@�X�e�[�g�ɑJ��
        TranitionBattleState();
    }
}

// �퓬�ҋ@�X�e�[�g�֑J��
void EnemySlime::TranitionBattleState()
{
    state = State::IdelBattle;

    // �^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    // �퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleBattle, true);
}

// �퓬�ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdelBattleState(float elapseedTime)
{
    // �ړI�n�_���v���C���[�ʒu�ɐݒ�
    targetPositon = Player::Instance().GetPosition();

    // �^�C�}�[����
    stateTimer -= elapseedTime;
    if (stateTimer < 0.0f)
    {
        // �v���C���[�ɍU���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
        float vx = targetPositon.x - position.x;
        float vy = targetPositon.y - position.y;
        float vz = targetPositon.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            // �U���X�e�[�g�֑J��
            TranitionAttackState();
        }
        else
        {
            // �p�j�X�e�[�g�֑J��
            TranitionWanderState();

        }
    }

    MoveToTarget(elapseedTime, 0.0f);
}

// �_���[�W�X�e�[�g�ɑJ��
void EnemySlime::TranitionDamageState()
{
    state = State::Damage;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetGit, false);
}

// �_���[�W�X�e�[�g�X�V����
void EnemySlime::UpdateDamageState(float elapseedTime)
{
    // �_���[�W�A�j���[�V�������I�������
    if(!model->IsPlayAnimation())
    {
        // �퓬�ҋ@�X�e�[�g�֑J��
        TranitionIdelState();
    }
}

// ���S�X�e�[�g�ɑJ��
void EnemySlime::TranitionDeathState()
{
    state = State::Death;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Die, false);
}

// ���S�X�e�[�g�X�V����
void EnemySlime::UpdateDeathState(float elapseedTime)
{
    // �_���[�W�A�j���[�V�������I�������
    if (!model->IsPlayAnimation())
    {
        // ������j��
        Destroy();
    }
}
