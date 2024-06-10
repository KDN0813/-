#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>   // [11]

// ���[�f�B���O�V�[��[11]
class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading(){}

    // ������[11]
    void Initialize() override;

    // �I����[11]
    void Finalize() override;

    // �X�V����[11]
    void Update(float elapsedTime) override;

    // �`�揈��[11]
    void Render() override;

private:
    // ���[�f�B���O�X���b�h[11]
    static void LoadingThred(SceneLoading* scene);

private:
    // [11]
    Sprite*         sprite      = nullptr;
    float           angle       = 0.0f;
    Scene*          nextScene   = nullptr;
    std::thread*    thread      = nullptr;
};