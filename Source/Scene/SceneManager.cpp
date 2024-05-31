#include "SceneManager.h"

void SceneManager::Initialize()
{
}

void SceneManager::Finalize()
{
    this->Clear();
}

void SceneManager::Update(const float elapsed_time)
{
    // ���̃V�[�����p�ӂ���Ă�����
    if (this->nextScene != nullptr)
    {
        this->Clear();
        // �V�����V�[����ݒ�
        this->currentScene = nextScene;
        this->nextScene = nullptr;

        this->currentScene->Initialize();
    }

    // ���݂̃V�[���̍X�V
    if (this->currentScene != nullptr)
    {
        this->currentScene->Update(elapsed_time);
    }
}

void SceneManager::Render(const ID3D11DeviceContext* dc)
{
    if (this->currentScene == nullptr) return;
    this->currentScene->Render(dc);
}

void SceneManager::Clear()
{
    if (this->currentScene != nullptr)
    {
        this->currentScene->Finalize();
        delete this->currentScene;
        this->currentScene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    this->nextScene = scene;
}
