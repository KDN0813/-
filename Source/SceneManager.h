#pragma once

#include "Scene.h"

// �V�[���}�l�[�W���[[10]
class SceneManager
{
private:
    SceneManager(){}
    ~SceneManager(){}
public:
    // �B��̃C���X�^���X�擾[10](�V���O���g��)
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    // �X�V����[10]
    void Update(float elpsedTime);

    // �`�揈��[10]
    void Render();

    // �V�[���N���A[10]
    void Clear();

    // �V�[���؂�ւ�[10]
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene    = nullptr;
};

