#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

// �G�l�~�[
class Enemy : public Character
{
public:
    Enemy(){}
    ~Enemy()override{}

    // �X�V����
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    // �f�o�b�O�pGUI�`��[org]
    virtual void DrawDebugGUI() = 0;

    // �f�o�b�N�v���~�e�B�u�`��[05]
    virtual void DarwDebugPrimitive();

    // �j��[08]
    void Destroy();

    // �G�l�~�[�̐�[org]
    static int enemuNum;
};