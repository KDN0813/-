#include <imgui.h>
#include "EnemyManager.h"
#include "Collision.h"

// [04]
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // �j������[08]
    for (Enemy* remove : removes)
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it =
            std::find(enemies.begin(), enemies.end(), remove);

        // (��)�폜����f�[�^�����݂����
        // (��)��find�֐��̓f�[�^��������Ȃ��ƍŌ�̃C�e���[�^��Ԃ�
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        // �e�ۂ̔j������
        delete remove;
    }
    // �j�����X�g���N���A
    removes.clear();


    // �G���m�̏Փˏ���[05]
    CollisionEnemyVsEnemies();
}


// [04]
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}


// �G�l�~�[�o�^[04]
void EnemyManager::Register(Enemy* enemy)
{
    //[04]
    enemies.emplace_back(enemy);
}

// �G�l�~�[�S�폜[04]
void EnemyManager::Clear()
{
    // [04]
    for (Enemy* enemy : enemies)
    {
        // [04]
        delete(enemy);
    }
    // [04]
    enemies.clear();
}

// �f�o�b�O�pGUI�`��[org]
void EnemyManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (int i = 0; i < GetEnemyCount(); ++i)
            {
                int  a = 22;
                char n = static_cast<char>(a);

                char no[2];
                snprintf(no, 2, "%d", i);
                if (ImGui::TreeNode(no))
                {
                    // �ʒu
                    GetEnemy(i)->DrawDebugGUI();

                    ImGui::TreePop();
                }
            }
        }
    }
    ImGui::End();
}

// �f�o�b�N�v���~�e�B�u�`��[05]
void EnemyManager::DrawDebugPrimitive()
{
    for (auto enemy : enemies)
    {
        enemy->DarwDebugPrimitive();
    }
}

// �G�l�~�[���m�̏Փˏ���[05]
void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = GetEnemy(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = GetEnemy(j);

            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition()
                , enemyA->GetRadius()
                , enemyA->GetHeigh()
                , enemyB->GetPosition()
                , enemyB->GetRadius()
                , enemyB->GetHeigh()
                , outPosition))
            {
                // �����o���̌�̈ʒu�ݒ�
                enemyB->SetPosition(outPosition);
            }
        }
    }
}

// �G�l�~�[�폜[08]
void EnemyManager::Remove(Enemy* enemy)
{
    // �j�����X�g�ɒǉ�[08]
    removes.insert(enemy);
}
