#include "Block.h"
#include <assert.h>

void Block::Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map) {
	//object3dCommon = Object3dCommon;
	//this->dxCommon = dxCommon;
	//this->winApp = winApp;


	//assert(map);
	//mapChipField_ = map;

	//// モデル
	//model_ = new Model();
	//model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	//GenerateBlocks();
}

void Block::Update() {
	/*for (Object3d* block : blocks_) {
		block->Update();

		block->SetPosition(modelPosition);
		block->SetRotation(modelRotation);
		block->SetScale(modelScale);
	}*/
}

void Block::Draw() {
	/*for (Object3d* block : blocks_) {
		block->Draw();
	}*/
}

void Block::GenerateBlocks() {
	//const uint32_t numVertical = mapChipField_->GetNumBlockVirtical();
	//const uint32_t numHorizontal = mapChipField_->GetNumBlockHorizontal();

	//for (uint32_t y = 0; y < numVertical; ++y) {
	//	for (uint32_t x = 0; x < numHorizontal; ++x) {
	//		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(x, y);

	//		// マップチップ番号に対応するモデルが存在するか確認
	//		if (modelMappings_.find(mapChipType) == modelMappings_.end()) {
	//			continue; // 未登録のマップチップタイプはスキップ
	//		}

	//		// モデルファイルパスを取得
	//		const std::string& modelFileName = modelMappings_[mapChipType];

	//		// オブジェクト3Dを生成
	//		Object3d* block = new Object3d();
	//		block->Initialize(object3dCommon, winApp, dxCommon);
	//		block->SetModel(modelFileName);                                     // モデルを設定
	//		block->SetPosition(mapChipField_->GetMapChipPositionByIndex(x, y)); // 位置を設定

	//		blocks_.push_back(block); // オブジェクトを登録
	//	}
	//}
}
