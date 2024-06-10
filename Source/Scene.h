#pragma once

// �V�[��[11]
class Scene
{
public:
    Scene(){}
    virtual ~Scene(){}

    // ������[11]
    virtual void Initialize() = 0;

    // �I����[11]
    virtual void Finalize() = 0;

    // �X�V����[11]
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��[11]
    virtual void Render() = 0;

    // �����������Ă��邩[11]
    bool IsReady() const { return ready; }

    // ���������ݒ�[11]
    void SetReady() { ready = true; }

private:
    // [11]
    bool ready = false; // �������������Ă��邩
};