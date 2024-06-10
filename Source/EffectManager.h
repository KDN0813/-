#pragma once
#include <DirectXMath.h>

// �G�t�F�N�V�A�����Ŏg��
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

// �G�t�F�N�g�}�l�[�W���[[10]
class EffectManager
{
private:
    EffectManager(){}
    ~EffectManager(){}

public:
    // �B��̃C���X�^���X�擾[10]
    static EffectManager& Instance()
    {
        static EffectManager instance;
        return instance;
    }

    // ������[10]
    void Initialize();

    // �I����[10]
    void Finalize();

    // �X�V����[10]
    void Update(float elapsedTime);

    // �`�揈��[10]
    void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // Effeckseer�}�l�[�W���[�̎擾[10]
    Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }

private:
    Effekseer::ManagerRef           effekseerManager;
    EffekseerRenderer::RendererRef  effekseerRender;
};