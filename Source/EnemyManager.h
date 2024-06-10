#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

// �G�l�~�[�}�l�[�W���[[04]
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager(){}

public:
    // �B��̃C���X�^���X�擾[04]
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // �X�V����[04]
    void Update(float elapsedTime);

    // �`�揈��[04]
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �G�l�~�[�o�^:Update(�o�^����)[04]
    void Register(Enemy* enemy);

    //�@�G�l�~�[�S�폜[04]
    void Clear();

    // �f�o�b�O�pGUI�`��[org]
    void DrawDebugGUI();

    // �f�o�b�N�v���~�e�B�u�`��[05]
    void DrawDebugPrimitive();

    // �G�l�~�[���擾[05]
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾[05]
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    // �G�l�~�[���m�̏Փˏ���[05]
    void CollisionEnemyVsEnemies();

    // �G�l�~�[�폜[08]
    void Remove(Enemy* enemy);

private:
    std::vector<Enemy*> enemies;
    // [08]
    std::set<Enemy*>    removes;
};

