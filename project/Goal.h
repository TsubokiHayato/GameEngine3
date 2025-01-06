#pragma once

#include "MapChipField.h"
#include "Model.h"
#include "Object3d.h"
#include "Player.h"
class Goal {

public:
	void Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map);
	void Update();
	void Draw();

	void SetPlayer(Player* player) { player_ = player; }

private:
	// オブジェクト3D共通部分
	Object3dCommon* object3dCommon = nullptr;
	// DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp = nullptr;

	Object3d* object3d;
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	Player* player_;

	// モデルディレクトリパス
	const std::string modelDirectoryPath = "Resources";
	// モデルファイルパス2
	const std::string modelFileNamePath = "barrier.obj";

	Vector3 modelPosition = {0.0f, 0.0f, 0.0f};

	Vector3 modelVector = {0.1f, 0.0f, 0.0f};
	// モデルの回転
	Vector3 modelRotation = {0.0f, 0.0f, 0.0f};
	// モデルの拡大
	Vector3 modelScale = {1.0f, 1.0f, 1.0f};
};
