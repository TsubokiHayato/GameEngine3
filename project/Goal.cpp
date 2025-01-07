#include "Goal.h"

#include"Input.h"
#undef min
#undef max
#include<algorithm>
#include<assert.h>
#include"imguiManager.h"
#include"MapChipField.h"

void Goal::Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map) {

	object3dCommon = Object3dCommon;
	this->dxCommon = dxCommon;
	this->winApp = winApp;
	mapChipField_ = map;

	assert(map);
	mapChipField_ = map;

	// モデル
	model_ = new Model();
	model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	object3d = new Object3d();
	object3d->Initialize(object3dCommon, this->winApp, this->dxCommon);
	object3d->SetModel(modelFileNamePath);

	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); ++y) {
		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x) {
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kGoal) {

				modelPosition = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
		}
	}
}

void Goal::Update() {

	modelRotation.x++;
	modelRotation.y++;
	modelRotation.z++;

	object3d->SetPosition(modelPosition);
	object3d->SetRotation(modelRotation);
	object3d->SetScale(modelScale);
	object3d->Update();

}

void Goal::Draw() {

	object3d->Draw();

}
