#pragma once

#include "Scene.h"

// �^�C�g���V�[��[10]
class SceneTitle : public Scene
{
public:
    SceneTitle(){}
    ~SceneTitle() override{};

    // ������[10]
    void Initialize() override;

    // �I����[10]
    void Finalize() override;

    // �X�V����[10]
    void Update(float elapsedTime) override;

    // �`�揈��[10]
    void Render() override;

private:
};

