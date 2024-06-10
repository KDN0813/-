#pragma once

#include <vector>
#include "Projectile.h"
#include <set>

// �e�ۃ}�l�[�W���[[07]
class ProjectileManager
{
public:
    // [07]
    ProjectileManager();
    // [07]
    ~ProjectileManager();

    // �X�V����[07]
    void Update(float elapsedTime);

    // �`�揈��[07]
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �f�o�b�O�v���~�e�B�u�`��[07]
    void DrawDebugPrimitive();

    // �e�ۓo�^[07]
    void Register(Projectile* projectile);

    // �e�ۑS�폜[07]
    void Clear();

    // �e�ې��擾[07]
    int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

    // �e�ێ擾[07]
    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    // �e�ۍ폜[07]
    void Remove(Projectile* projectile);

private:
    // [07]
    std::vector<Projectile*> projectiles;

    // �j���N���X[07]
    std::set<Projectile*>removes;
};

