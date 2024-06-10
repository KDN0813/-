#include "StageMain.h"

// コンストラクタ
StageMain::StageMain()
{
    // ステージモデル読み込み
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

StageMain::~StageMain()
{
    // ステージモデル破棄
    delete model;
}

//　更新処理
void StageMain::Update(float elapasedTIme)
{
}

// 描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // シェーダーにモデル描画してもらう
    shader->Draw(dc, model);
}

// レイキャスト
bool StageMain::RayCact(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, this->model, hit);
}
