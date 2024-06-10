#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

// �X�V����[07]
void ProjectileManager::Update(float elapsedTime)
{
    for (Projectile* p : projectiles)
    {
        p->Update(elapsedTime);
    }

    // �j������
    // ��projectiles�͈̔�for������erase()����ƕs����������邽�߁A
    //  �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Projectile* projectile : removes)
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Projectile*>::iterator it = 
            std::find(projectiles.begin(), projectiles.end(), projectile);

        // (��)�폜����f�[�^�����݂����
        // (��)��find�֐��̓f�[�^��������Ȃ��ƍŌ�̃C�e���[�^��Ԃ�
        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        // �e�ۂ̔j������
        delete projectile;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��[07]
void ProjectileManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (auto p : projectiles)
    {
        p->Render(context, shader);
    }
}

// �f�o�b�O�v���~�e�B�u�`��[07]
void ProjectileManager::DrawDebugPrimitive()
{
    for (auto p : projectiles)
    {
        p->DrawDebugPrimitive();
    }
}

// �e�ۓo�^[07]
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

// �e�ۑS�폜[07]
void ProjectileManager::Clear()
{
    for (auto p : projectiles)
    {
        delete(p);
    }
    projectiles.clear();
}

// �e�ۍ폜[07]
void ProjectileManager::Remove(Projectile* projectile)
{
    // �j�����X�g�ɒǉ�
    removes.insert(projectile);
}
