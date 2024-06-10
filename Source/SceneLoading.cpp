#include "SceneLoading.h"
#include "Graphics/Graphics.h"
#include "SceneLoading.h"
#include "SceneManager.h"

// ������[11]
void SceneLoading::Initialize()
{
    // �X�v���C�g������[10]
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");

    // �X���b�h�J�n
    //=(��)���̃V�[���̃X���b�h�𗧂��グ��(���ȉ�)
    thread = new std::thread(LoadingThred, this);
}

// �I����[11]
void SceneLoading::Finalize()
{
    // �X���b�h�I����(���ȉ�)
    if (thread != nullptr)
    {
        thread->join();     // thread�̏������I���܂ő҂�
        delete thread;
        thread = nullptr;
    }
    nextScene = nullptr;

    // �X�v���C�g�I����[10]
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

// �X�V����[11]
void SceneLoading::Update(float elapsedTime)
{
    // �摜��]
    constexpr float speed = 180;
    angle += speed * elapsedTime;

    // ���̃V�[���̏���������������V�[����؂�ւ���(���ȉ�)
    
    // �V�[���̏���������������
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
    }
}

// �`�揈��[11]
void SceneLoading::Render()
{
    Graphics& graphics          = Graphics::Instance();
    ID3D11DeviceContext* dc     = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�

    // ��ʂ�h��Ԃ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(
        rtv                                                                         // �����_�[�^�[�Q�b�g�ւ̃|�C���^�[
        , color                                                                     // �F
    );

    // �[�w�X�e���V�����\�[�X���N���A����
    dc->ClearDepthStencilView(
        dsv                                                                         // �N���A����[�x�X�e���V���ւ̃|�C���^�[
        , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL                                   // �N���A����^�C�v(�[�x�o�b�t�@�ƃX�e���V���o�b�t�@���N���A����)
        , 1.0f                                                                      // ���̒l�Ő[�x�o�b�t�@���N���A����
        , 0                                                                         // ���̒l�ŃX�e���V���o�b�t�@���N���A����
    );

    // 1�ȏ�̃����_�[�^�[�Q�b�g���A�g�~�b�N�Ƀo�C���h���A�[�x�X�e���V���o�b�t�@�[���o�̓}�l�[�W���[�X�e�[�W�Ƀo�C���h����
    dc->OMSetRenderTargets(
        1                                                                           // �o�C���h���郌���_�[�^�[�Q�b�g�̐�
        , &rtv                                                                      // �f�o�C�X�Ƀo�C���h���郌���_�[�^�[�Q�b�g�̔z��ւ̃|�C���^�[
        , dsv                                                                       // �f�o�C�X�Ƀo�C���h����X�e���V���r���[�ւ̃|�C���^�[
    );

    // 2D�X�v���C�g�`��
    {
        // ��ʉ��Ƀ��[�f�B���O�A�C�R����`��
        float screenWidth       = static_cast<float>(graphics.GetScreenWidth());    //�X�N���[����
        float screenHeight      = static_cast<float>(graphics.GetScreenHeight());   // �X�N���[������
        float textureWidth      = static_cast<float>(sprite->GetTextureWidth());    // �e�N�X�`����
        float textureHeight     = static_cast<float>(sprite->GetTextureHeight());   // �e�N�X�`������
        float positionX         = screenWidth - textureWidth;                       // �\��������W(X)
        float positionY         = screenHeight - textureHeight;                     // �\��������W(Y)
    
        sprite->Render(
            dc
            , positionX, positionY                                                  // �`��ʒu
            , textureWidth, textureHeight                                           // �`��T�C�Y
            , 0, 0                                                                  // �`��ʒu
            , textureWidth, textureHeight                                           // �摜�̕��E����
            , angle                                                                 //�p�x
            , 1, 1, 1, 1                                                            //�F
        );
    }
}

// ���[�f�B���O�X���b�h[11]
void SceneLoading::LoadingThred(SceneLoading* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������[11]
    CoInitialize(nullptr);

    // ���̃V�[���̏��������s��[11](���ȉ�)
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM����̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�[11](���ȉ�)
    scene->nextScene->SetReady();
}
