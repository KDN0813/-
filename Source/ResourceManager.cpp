#include "Graphics/Graphics.h"
#include "ResourceManager.h"

// モデルリソース読み込み[21]
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
    // モデル検索
    // 見つかったらそのmodelを返す
    ModelMap::iterator it = models.find(filename);
    // 見つかった場合
    if (it != models.end())
    {
        // リンクが切れていない場合
        if (it->second.expired() == false)
        {
            // 読み込み済のモデルリソースを返す
            // shader_ptrに変換して渡す
            return it->second.lock();
        }
    }

    // 新規モデルリソース作成＆読み込み
    std::shared_ptr<ModelResource>	model;
    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
 
    // マップに登録
    // キーが登録されていなければ挿入し、あれば代入する
    // データの上書き・新規追加が自動で行われている
    models[filename] = std::weak_ptr<ModelResource>(model);
    //models.insert_or_assign(filename, std::weak_ptr<ModelResource>(model));

    return model;
}