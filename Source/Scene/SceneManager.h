#pragma once
#include "Scene.h"

class SceneManager
{
public:
    SceneManager() noexcept = default;
    virtual ~SceneManager() noexcept = default;

    // �R�s�[�E���[�u�R���X�g���N�^(������Z�q)�̋֎~
    SceneManager(const SceneManager& s) noexcept = delete;
    SceneManager& operator=(SceneManager&) noexcept = default;
    SceneManager(const SceneManager&& s) noexcept = delete;
    SceneManager& operator=(SceneManager&&) noexcept = delete;

public:
    /// <summary>
    /// ����������
    /// </summary>
    void Initialize();
    /// <summary>
    /// �I������
    /// </summary>
    void Finalize();
    /// <summary>
    ///  �X�V����
    /// </summary>
    /// <param name="elapsed_time">�o�ߎ���</param>
    void Update(const float elapsed_time);
    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="dc">�f�o�C�X�R���e�L�X�g</param>
    void Render(const ID3D11DeviceContext* dc);
    /// <summary>
    /// �V�[���̃N���A����
    /// </summary>
    void Clear();
    /// <summary>
    /// �V�[���̐؂�ւ�
    /// </summary>
    /// <param name="scene">���̃V�[���̃|�C���^</param>
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
};

