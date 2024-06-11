#include "SceneLoading.h"
#include "System/Misc.h"

#include "SceneManager.h"

void SceneLoading::Initialize()
{
    thread = std::make_unique<std::thread>(LoadingThred, this);
}

void SceneLoading::Finalize()
{
    thread->join();
}

void SceneLoading::Update(float elapsed_time)
{
    if (next_scene->IsReady())
    {
        SceneManager::Instance().ChangeScene(next_scene);
    }
}

void SceneLoading::Render()
{
}

void SceneLoading::LoadingThred(SceneLoading* scene)
{
    HRESULT hr = S_OK;
    hr = CoInitialize(nullptr);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    scene->next_scene->Initialize();
    CoUninitialize();
    scene->next_scene->SetReady();
}
