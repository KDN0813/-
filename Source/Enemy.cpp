#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

int Enemy::enemuNum = 0;

// �f�o�b�N�v���~�e�B�u�`��[05]
void Enemy::DarwDebugPrimitive()
{
    // [05]
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�N����`��[05]
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �Փ˔���p�̃f�o�b�O�~����`��[06]
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// �j��[08]
void Enemy::Destroy()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    // �폜���X�g�Ɏ��g��ǉ�����[08]
    enemyManager.Remove(this);
}
