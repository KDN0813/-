#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "ResourceManager.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// アニメーション更新処理[17]
void Model::UpdateAnimation(float elapsedTIme)
{
	// 再生中でないなら処理しない[17]
	if (!IsPlayAnimation()) return;

	// ブレンド率の計算
	float blendRate = 1.0f;

	if (animationBlendTIme < animationBlendSeconds)
	{
		animationBlendTIme += elapsedTIme;
		if (animationBlendTIme >= animationBlendSeconds)
		{
			animationBlendTIme = animationBlendSeconds;
		}
			//　ブレンド率計算
		blendRate = animationBlendTIme / animationBlendSeconds;
		blendRate *= blendRate;
	}

	// 指定のアニメーションデータを取得[17]
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();		// リソースのアニメーション取得(resource->GetAnimations()を省略するための記述している)
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);			// 指定のアニメーションデータを取得

	// アニメーションデータからキーフレームデータリストを取得[17]
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるのか判定する[17]
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);

		// 経過時間が再生時間内なら
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間かた補間率を算出する[17](自己解)
			float rate = (currentAnimationSeconds - keyframe0.seconds) 
				/ (keyframe1.seconds - keyframe0.seconds);
			
			int nodeCount = static_cast<int>(nodes.size());
			
			// 再生時間とキーフレームの時間から補間率を計算する
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2つのキーフレーム間の補間計算[17]
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				// 自己解

				// ブレンド補間処理[17]
				// 現在は機能していない
				if (blendRate < 1.0f)
				{
					// 自己解[11]


					// 現在の姿勢と次のキーフレームとの姿勢の補間

					// スケールの線形補間
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S  = DirectX::XMVectorLerp(S0, S1, rate);
					
					// 角度の球面線形補間
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);		// 今の姿勢
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);		// 次のキーフレームの姿勢
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// 座標の線形補間
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

					// 補間結果を設定
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// 通常の計算[17]
				else {
					// 前のキーフレームと次のキーフレームの姿勢を補間
					
					// スケールの線形補間
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);

					// 角度の線形補間
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR R  = DirectX::XMQuaternionSlerp(R0, R1, rate);

					// 座標の線形補間
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					DirectX::XMVECTOR T  = DirectX::XMVectorLerp(T0, T1, rate);

					// 補間結果を設定
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	// 最終フレーム
	if (animationEndFlag)
	{
		animationEndFlag		= false;
		currentAnimationIndex	= -1;
		return;
	}

	// 時間経過[17]
	currentAnimationSeconds += elapsedTIme;

	// 再生時間が終端時間を超えたら[17]
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// ループ再生する場合
		if (animationLoopFlag)
		{
			// 再生時間を待ち戻す[17]
			currentAnimationSeconds -= animation.secondsLength;
			return;
		}
		// ループ再生しない場合
		else
		{
			animationEndFlag = true;
		}
	}
}

// アニメーション再生[17]
void Model::PlayAnimation(int index, bool loop,float blendSeconds)
{
	currentAnimationIndex	= index;
	currentAnimationSeconds = 0.0f;

	animationLoopFlag		= loop;
	animationEndFlag		= false;

	animationBlendTIme		= 0.0f;
	animationBlendSeconds	= blendSeconds;
}

// アニメーション再生中か[17]
bool Model::IsPlayAnimation() const
{
	// アニメーション番号が0以下なら
	if (currentAnimationIndex < 0)return false;
	// アニメーション番号が保存されているアニメーションの数以上なら
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;

	return true;
}

// ノード検索[18]
Model::Node* Model::FindNode(const char* name)
{
	// 全てのノードを総当たりで名前比較する
	for (Node& node : nodes)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}

	// 見つからなかった
	return nullptr;
}
