#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"

#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"

#include "ProjectileHoming.h"

static Player* instance = nullptr;

// �R���X�g���N�^[03]
Player::Player()
{
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    // �q�b�g�G�t�F�N�g�ǂݍ���[10]
    hitEffect = new Effect("Data/Effect/test02.efk");

    //// �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();

    instance = this;
}

// [03]
Player::~Player()
{
    // �G�t�F�N�g�폜[10]
    delete hitEffect;

    delete model;

    instance = nullptr;
}

Player& Player::Instance()
{
    return *instance;
}

void Player::Update(float elapsedTime)
{
    // �X�e�[�g���̏���[18]
    switch (state)
    {
    case Player::State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case Player::State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case Player::State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case Player::State::Land:
        UpdateLandState(elapsedTime);
        break;
    case Player::State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case Player::State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case Player::State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case Player::State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    }

    // ���x�����X�V[06]
    UpdateVelocity(elapsedTime);

    // ���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    // �e�ۍX�V����
    projectileManager.Update(elapsedTime);

    // �v���C���[�ƓG�Ƃ̏Փˏ���[05]
    CollisionPlayerVsEnemies();

    // �e�ۂƓG�̏Փˏ���[08]
    CollisionProjectilesVsEnemis();

    // ���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    // �I�u�W�F�N�g�s����X�V[03]
    UpdateTransform();

    // ���f���s��X�V[03]
    model->UpdateTransform(transform);
}

// �X�e�B�b�N���͒l����ړ��x�N�g�����擾[03]
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // ���͏����擾[03]
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����[03]
    Camera& camera = Camera::Intance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRIght();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���[03]

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�[03]
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����[03]
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�[03]
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����[03]
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����[03]
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    
    // Y���������͈ړ����Ȃ�[03]
    vec.y = 0.0f;

    return vec;
}

// �ړ����͏���[03]
bool Player::InputMove(float elapsedTime)
{
    // �i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    // ���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���[05
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManaget = EnemyManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManaget.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManaget.GetEnemy(i);
        
        // �Փˏ���[06]
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            position
            , radius
            , height
            , enemy->GetPosition()
            , enemy->GetRadius()
            , enemy->GetHeigh()
            , outPosition))
        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {
                // ���W�����v
                Jump(JumpSpeed * 0.5f);

                // �_���[�W��^����[08]
                enemy->ApplyDamage(1,0.5f);
            }
            else
            {
                // �����o���̌�Ƃ̈ʒu�ݒ�
                enemy->SetPosition(outPosition);
            }
        }
    }
}

// [03]
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
    projectileManager.Render(dc, shader);
}

// �f�o�b�O�pGUI�`��[03]
void Player::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("player", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // �̗�
            ImGui::InputInt("hp", &health);

            // �ʒu
            ImGui::InputFloat3("Position", &position.x);
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            // �p�x
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // �X�P�[��
            ImGui::InputFloat3("Scale",&scale.x);
            // ���x
            ImGui::InputFloat3("Velocity", &velocity.x);
            // �W�����v�J�E���g
            ImGui::Text("jumpCount:%d", jumpCount);
            // �W�����v���~�b�g
            ImGui::InputInt("jumpLimit", &jumpLimit);
        };

        // projectile�̐�
        ImGui::Text("projectileNum:%ld",projectileManager.GetProjectileCount());
    }
    ImGui::End();
}

// �f�o�b�N�v���~�e�B�u�`��[05]
void Player::DarwDebugPrimitive()
{
    // [05]
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�N����`��[05]
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �Փ˔���p�̃f�o�b�O�~����`��[06]
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �e�ۃf�o�b�N�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();

    // �U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRender->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadiuns,
            DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
        );
    }
}

// �e�ۓ��͏���[07]
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // ���i�e�۔���[07]
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        // �O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);
        // ���ˈʒu(�v���C���[����������)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;
        // ����
        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
    }
    // �ǔ��e�۔���[07]
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        // �O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);
        // ���ˈʒu(�v���C���[�̍�������)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + (height * 0.5f);
        pos.z = position.z;
        // �^�[�Q�b�g(�f�t�H���g�ł̓v���C���[�̑O��)
        DirectX::XMFLOAT3 target;
        target.x = position.x + dir.x * 1000.0f;
        target.y = position.y + dir.y * 1000.0f;
        target.z = position.z + dir.z * 1000.0f;

        // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            // �G�Ƃ̋�������
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeigh() * 0.5f;
            }
        }
        // ����
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
}

// �ҋ@�X�e�[�g�֑J��[18]
void Player::TransitionIdleState()
{
    state = State::Idle;

    // �ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

// �ҋ@�X�e�[�g�X�V����[18]
void Player::UpdateIdleState(float elpsedTime)
{
    // �ړ����͏���
    if (InputMove(elpsedTime))
    {
        // �ړ����͂��ꂽ��ړ��X�e�[�g�֑J��
        TransitionMoveState();
    }

    // �W�����v���͏���
    if (InputJump())
    {
        // �W�����v���͂��ꂽ��W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    // �U�����͏���
    if (InputAttack())
    {
        TransitionAattackState();
    }

    // �e�ۓ��͏���
    InputProjectile();
}

// �ړ��X�e�[�g�֑J��[18]
void Player::TransitionMoveState()
{
    state = State::Move;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}

// �ړ��X�e�[�g�X�V����[18]
void Player::UpdateMoveState(float elpsedTime)
{
    // �ړ����͏���
    if (!InputMove(elpsedTime))
    {
        // �ړ����͂���Ȃ�������ړ��X�e�[�g�֑J��
        TransitionIdleState();
    }

    // �W�����v���͏���
    if (InputJump())
    {
        // �W�����v���͂��ꂽ��W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    // �e�ۓ��͏���
    InputProjectile();
}

// �W�����v�X�e�[�g�֑J��[18]
void Player::TransitionJumpState()
{
    state = State::Jump;

    // �W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

// �W�����v�X�e�[�g�X�V����[18]
void Player::UpdateJumpState(float elpsedTime)
{
    // �ړ����͏���
    InputMove(elpsedTime);

    // �W�����v���͏���
    if (InputJump())
    {
        // �ĂуW�����v���͂��ꂽ��W�����v�t���b�v�Đ�
        model->PlayAnimation(Anim_Jump_Flip, false);
    }
    // �W�����v�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        // �����A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Falling,true);
    }

    // �e�ۓ��͏���
    InputProjectile();
}

// ���n�X�e�[�g�֑J��[18]
void Player::TransitionLandState()
{
    state = State::Land;

    jumpCount = 0;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landig, false);
}

// ���n�X�e�[�g�X�V����[18]
void Player::UpdateLandState(float elpsedTime)
{
    // �A�j���[�V�����Đ����I�������ړ��X�e�[�g�֑J��
    if (model->IsPlayAnimation() == false)
    {
            TransitionIdleState();
    }

    // �e�ۓ��͏���
    InputProjectile();
}

// �U�����͏���
bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }

    return false;
}

void Player::TransitionAattackState()
{
    state = State::Attack;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elpsedTime)
{
    // �A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    // �C�ӂ̃A�j���[�V�����Đ���Ԃ̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    float attackStartTime = 0.3f;
    float attackEndTime = 0.4f;
    attackCollisionFlag =
        (animationTime >= attackStartTime) &&
        (animationTime <= attackEndTime);

    if (attackCollisionFlag)
    {
        // ����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemis("mixamorig:LeftHand", leftHandRadiuns);
    }
}

// �_���[�W�X�e�[�g�ɑJ��
void Player::TranitionDamageState()
{
    state = State::Damage;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

// �_���[�W�X�e�[�g�X�V����
void Player::UpdateDamageState(float elapseedTime)
{
    // �_���[�W�A�j���[�V�������I�������
    if(!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

// ���S�X�e�[�g�ɑJ��
void Player::TranitionDeathState()
{
    state = State::Death;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

// ���S�X�e�[�g�X�V����
void Player::UpdateDeathState(float elapseedTime)
{
    // ���S�A�j���[�V�������I�������
    if (!model->IsPlayAnimation())
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        if(gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TranitionRevivehState();
        }
    }
}

// �����X�e�[�g�ɑJ��
void Player::TranitionRevivehState()
{
    state = State::Death;

    // �̗͉�
    health = maxHealth;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Revive, false);
}

// �����X�e�[�g�X�V����
void Player::UpdateReviveState(float elapseedTime)
{
    // �����A�j���[�V�������I�������
    if (!model->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        TransitionIdleState();
    }
}

// �W�����v���͏���[06]
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // �W�����v�񐔐���[06]
        if (jumpCount < jumpLimit)
        {
            // �W�����v[06]
            ++jumpCount;
            Jump(JumpSpeed);

            //�@�W�����v���͂���
            return true;
        }
    }
    return false;
}

// ���n�����Ƃ��ɌĂ΂��[06]
void Player::OnLandig()
{
    // �_���[�W�A���S�X�e�[�g���͒��n�������ɃX�e�[�g�J�ڂ��Ȃ��悤�ɂ���
    if (state != State::Damage && state != State::Death)
    {
        // ���n�X�e�[�g�֑J��
        TransitionLandState();
    }
}

// �_���[�W���󂯂����ɌĂ΂�鏈��
void Player::OnDamaged()
{
    // �_���[�W�X�e�[�g�֑J��
    TranitionDamageState();
}

// ���S�������ɌĂ΂�鏈��
void Player::OnDead()
{
    // ���S�X�e�[�g�֑J��
    TranitionDeathState();
}

// �e�ۂƓG�̏Փˏ���[08]
void Player::CollisionProjectilesVsEnemis()
{
    EnemyManager& enemyManager  = EnemyManager::Instance();

    // �S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓˏ���
    int projectileCount         = projectileManager.GetProjectileCount();
    int enemyCount              = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        // i�Ԗڂ̒e�ۂ��擾
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //// �Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeigh(),
                outPosition
            ))
            {
                // �������Ă����
                // �_���[�W��^����
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    // ������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();

                        // �e����G�܂ł̃x�N�g��
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->AddImpulse(impulse);
                    }

                    // �q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeigh() * 0.5f;
                        hitEffect->Play(e);
                    }

                    // �e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}

// �m�[�h�ƓG�̏Փˏ���[19]
void Player::CollisionNodeVsEnemis(const char* nodeName, float nodeRadiuns)
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �m�[�h�擾
    Model::Node* node = model->FindNode("mixamorig:LeftHand");

    // �m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = { node->worldTransform._41,node->worldTransform._42 ,node->worldTransform._43 };

    int enemyCount = enemyManager.GetEnemyCount();

    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //// �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadiuns,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeigh(),
            outPosition
        ))
        {
            // �������Ă����
            // �_���[�W��^����
            if (enemy->ApplyDamage(1, 0.5f))
            {
                // ������΂�
                {
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    const DirectX::XMFLOAT3& p = nodePosition;

                    // �e����G�܂ł̃x�N�g��
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    enemy->AddImpulse(impulse);
                }

                // �q�b�g�G�t�F�N�g�Đ�
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeigh() * 0.5f;
                    hitEffect->Play(e);
                }
            }
        }
    }
}