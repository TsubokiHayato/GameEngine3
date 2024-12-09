#include "Block.h"
#include <assert.h>


void Block::Initialize(Object3dCommon* Object3dCommon,ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map)
{
	object3dCommon = Object3dCommon;
	dxCommon = dxCommon;
	winApp = winApp;
	mapChipField_ = map;

	assert(map);
	mapChipField_ = map;

	//ƒ‚ƒfƒ‹
	model_ = new Model();
	model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	GenerateBlocks();

}

void Block::Update()
{

	for (Object3d* block : blocks_)
	{
		block->Update();

		block->SetPosition(modelPosition);
		block->SetRotation(modelRotation);
		block->SetScale(modelScale);

	}
	
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

