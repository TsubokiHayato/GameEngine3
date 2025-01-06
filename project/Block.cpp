#include "Block.h"
#include <assert.h>

#include"imguiManager.h"
void Block::Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map)
{
	object3dCommon = Object3dCommon;
	this->dxCommon = dxCommon;
	this->winApp = winApp;
	mapChipField_ = map;

	assert(map);
	mapChipField_ = map;

	//モデル
	model_ = new Model();
	model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	GenerateBlocks();

}

void Block::Update()
{

	for (Object3d* block : blocks_)
	{
		block->Update();

		block->SetRotation(modelRotation);
		block->SetScale(modelScale);

	}
	ImGui::Begin("Object3D_block");
	ImGui::Text("%d", static_cast<int>(blocks_.size()));
	ImGui::DragFloat3("Rotation", &modelRotation.x);
	ImGui::DragFloat3("Scale", &modelScale.x, 0.1f);
	ImGui::End();


}

void Block::Draw()
{
	for (Object3d* block : blocks_)
	{
		block->Draw();
	}

}

void Block::GenerateBlocks()
{


	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); ++y)
	{
		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x)
		{
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock)
			{

				Object3d* block = new Object3d();
				block->Initialize(object3dCommon, winApp, dxCommon);
				block->SetModel(modelFileNamePath);
				block->SetPosition(mapChipField_->GetMapChipPositionByIndex(x, y));
				blocks_.push_back(block);
			}
		}
	}

}

