#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

	// アニメーション更新処理[17]
	void UpdateAnimation(float elapsedTIme);

	// アニメーション再生[17]
	void PlayAnimation(int index,bool loop,float blendSeconds = 0.2f);

	// アニメーション再生中か[17]
	bool IsPlayAnimation()const;

	// ノード検索[18]
	Model::Node* FindNode(const char* name);

	// 現在のアニメーション待再生時間取得
	float GetCurrentAnimationSeconds()const { return currentAnimationSeconds; }

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	// [17]
	// 現在のアニメーション番号
	int currentAnimationIndex		= -1;
	float currentAnimationSeconds	= 0.0f;	// 経過時間
	bool animationLoopFlag			= false;
	bool animationEndFlag			= false;
	float animationBlendTIme		= 0.0f;
	float animationBlendSeconds		= 0.0f;
};
