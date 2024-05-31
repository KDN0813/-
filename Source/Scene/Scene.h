#pragma once
#include <d3d11.h>

class Scene
{
public:
    Scene() noexcept = default;
    virtual ~Scene() noexcept = default;

    // �R�s�[�E���[�u�R���X�g���N�^(������Z�q)�̋֎~
    Scene(const Scene& s) noexcept = delete;
    Scene& operator=(Scene&) noexcept = default;
    Scene(const Scene&& s) noexcept = delete;
    Scene& operator=(Scene&&) noexcept = delete;

public:
    /// <summary>
    /// ����������
    /// </summary>
    virtual void Initialize() = 0;
    /// <summary>
    /// �I������
    /// </summary>
    virtual void Finalize() = 0;
    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name="elapsed_time">�o�ߎ���</param>
    virtual void Update(const float elapsed_time) = 0;
    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="dc">�f�o�C�X�R���e�L�X�g</param>
    virtual void Render(const ID3D11DeviceContext* dc) = 0;
};