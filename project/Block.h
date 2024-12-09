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

	//�I�u�W�F�N�g3D���ʕ���
	Object3dCommon* object3dCommon = nullptr;
	//DirectX���ʕ���
	DirectXCommon* dxCommon = nullptr;
	//�E�B���h�E�Y�A�v���P�[�V����
	WinApp* winApp = nullptr;


	std::vector<std::vector<Vector3*>> translateBlocks_;
	std::vector<Object3d*> blocks_;
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	//���f���f�B���N�g���p�X
	const std::string modelDirectoryPath = "Resources";
	//���f���t�@�C���p�X2
	const std::string modelFileNamePath = "barrier.obj";


	Vector3 modelPosition = { -1.0f,0.0f,0.0f };
	Vector3 modelRotation = { 0.0f,0.0f,0.0f };
	Vector3 modelScale = { 1.0f,1.0f,1.0f };

};

