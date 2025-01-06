#pragma once
#include <vector>
#include"Object3d.h"
#include "Model.h"
#include "MapChipField.h"
class Block
{

public:
	void Initialize(Object3dCommon* Object3dCommon,ModelCommon* modelCommon,DirectXCommon* dxCommon,WinApp* winApp ,MapChipField* map);
	void Update();
	void Draw();
	void GenerateBlocks();

	


private:

	//オブジェクト3D共通部分
	Object3dCommon* object3dCommon = nullptr;
	//DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	//ウィンドウズアプリケーション
	WinApp* winApp = nullptr;


	std::vector<std::vector<Vector3*>> translateBlocks_;
	std::vector<Object3d*> blocks_;
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	//モデルディレクトリパス
	const std::string modelDirectoryPath = "Resources";
	//モデルファイルパス2
	const std::string modelFileNamePath = "cube.obj";

	//モデルの回転
	Vector3 modelRotation = { 0.0f,0.0f,0.0f };
	//モデルの拡大
	Vector3 modelScale = { 1.0f,1.0f,1.0f };

};

