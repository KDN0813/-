#pragma once
#include <memory>
#include <string>
#include <map>
#include "Graphics/ModelResource.h"

// リソースマネージャー[21]
class ResourceManager
{
private:
    ResourceManager(){}
    ~ResourceManager(){}
    
public:
    // 唯一のインスタンス取得[21]
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    // モデルリソース読み込み[21]
    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
    // 用は新しい型作ってる
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};