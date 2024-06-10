#include "SceneLoading.h"
#include "Graphics/Graphics.h"
#include "SceneLoading.h"
#include "SceneManager.h"

// 初期化[11]
void SceneLoading::Initialize()
{
    // スプライト初期化[10]
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");

    // スレッド開始
    //=(メ)次のシーンのスレッドを立ち上げる(自己解)
    thread = new std::thread(LoadingThred, this);
}

// 終了化[11]
void SceneLoading::Finalize()
{
    // スレッド終了化(自己解)
    if (thread != nullptr)
    {
        thread->join();     // threadの処理が終わるまで待つ
        delete thread;
        thread = nullptr;
    }
    nextScene = nullptr;

    // スプライト終了化[10]
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

// 更新処理[11]
void SceneLoading::Update(float elapsedTime)
{
    // 画像回転
    constexpr float speed = 180;
    angle += speed * elapsedTime;

    // 次のシーンの準備が完了したらシーンを切り替える(自己解)
    
    // シーンの準備が完了したら
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
    }
}

// 描画処理[11]
void SceneLoading::Render()
{
    Graphics& graphics          = Graphics::Instance();
    ID3D11DeviceContext* dc     = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア&レンダーターゲット設定

    // 画面を塗りつぶす
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(
        rtv                                                                         // レンダーターゲットへのポインター
        , color                                                                     // 色
    );

    // 深層ステンシルリソースをクリアする
    dc->ClearDepthStencilView(
        dsv                                                                         // クリアする深度ステンシルへのポインター
        , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL                                   // クリアするタイプ(深度バッファとステンシルバッファをクリアする)
        , 1.0f                                                                      // この値で深度バッファをクリアする
        , 0                                                                         // この値でステンシルバッファをクリアする
    );

    // 1つ以上のレンダーターゲットをアトミックにバインドし、深度ステンシルバッファーを出力マネージャーステージにバインドする
    dc->OMSetRenderTargets(
        1                                                                           // バインドするレンダーターゲットの数
        , &rtv                                                                      // デバイスにバインドするレンダーターゲットの配列へのポインター
        , dsv                                                                       // デバイスにバインドするステンシルビューへのポインター
    );

    // 2Dスプライト描画
    {
        // 画面下にローディングアイコンを描画
        float screenWidth       = static_cast<float>(graphics.GetScreenWidth());    //スクリーン幅
        float screenHeight      = static_cast<float>(graphics.GetScreenHeight());   // スクリーン高さ
        float textureWidth      = static_cast<float>(sprite->GetTextureWidth());    // テクスチャ幅
        float textureHeight     = static_cast<float>(sprite->GetTextureHeight());   // テクスチャ高さ
        float positionX         = screenWidth - textureWidth;                       // 表示する座標(X)
        float positionY         = screenHeight - textureHeight;                     // 表示する座標(Y)
    
        sprite->Render(
            dc
            , positionX, positionY                                                  // 描画位置
            , textureWidth, textureHeight                                           // 描画サイズ
            , 0, 0                                                                  // 描画位置
            , textureWidth, textureHeight                                           // 画像の幅・高さ
            , angle                                                                 //角度
            , 1, 1, 1, 1                                                            //色
        );
    }
}

// ローディングスレッド[11]
void SceneLoading::LoadingThred(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある[11]
    CoInitialize(nullptr);

    // 次のシーンの初期化を行う[11](自己解)
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM寒冷の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定[11](自己解)
    scene->nextScene->SetReady();
}
