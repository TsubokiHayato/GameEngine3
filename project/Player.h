#pragma once
#include <vector>
#include"Object3d.h"
#include "Model.h"
#include "MapChipField.h"
#include"ModelManager.h"
#include"Input.h"
class Player
{
public:
	Player();
	~Player();

	void Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp);
	void Update();
	void Draw();



private:
	Input* input = nullptr;

	// オブジェクト3D共通部分
	Object3dCommon* object3dCommon = nullptr;
	// DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp = nullptr;

	
	Object3d* object3d;
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	// モデルディレクトリパス
	const std::string modelDirectoryPath = "Resources";
	// モデルファイルパス2
	const std::string modelFileNamePath = "barrier.obj";

	Vector3 modelPosition = {-1.0f, 0.0f, 0.0f};
	Vector3 modelRotation = {0.0f, 0.0f, 0.0f};
	Vector3 modelScale = {1.0f, 1.0f, 1.0f};



};

