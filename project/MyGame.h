#pragma once

#include"DirectXcommon.h"
#include"D3DResourceLeakChecker.h"
#include"MT_Matrix.h"
#include "Input.h"
#include"Audio.h"
#include"Camera.h"

#include"SpriteCommon.h"
#include"Sprite.h"
#include"TextureManager.h"

#include"Object3dCommon.h"
#include"Object3d.h"
#include"ModelCommon.h"
#include"Model.h"
#include"ModelManager.h"
#include <SrvManager.h>

#ifdef _DEBUG

#include"ImGuiManager.h"

#endif // DEBUG

#include <iostream>
#include <algorithm>
#undef min//min�}�N���𖳌���
#undef max//max�}�N���𖳌���

#pragma comment(lib,"dxguid.lib")//DirectX�̃��C�u����
#pragma comment(lib,"dxcompiler.lib")//DirectX�̃��C�u����


# define PI 3.14159265359f
#ifdef _DEBUG

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // DEBUG




class MyGame
{
public:
	void Initialize();

	void Update();

	void Finalize();

	void Draw();

private:

	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Object3dCommon* object3dCommon = nullptr;
	ModelCommon* modelCommon = nullptr;
	SrvManager* srvManager = nullptr;
	std::unique_ptr< ImGuiManager> imGuiManager = nullptr;
	Input* input = nullptr;
	std::unique_ptr<Audio> audio = nullptr;

	///Sprite///
	
	//���E���]�t���O
	bool isFlipX_;
	//�㉺���]�t���O
	bool isFlipY_;
	//�e�N�X�`���̍�����W
	Vector2 textureLeftTop;
	//�e�N�X�`�����珉���T�C�Y�𓾂�t���O
	bool isAdjustTextureSize;

	std::vector<Sprite*> sprites;

	///Model///

	Object3d* object3d;
	Vector3 modelPosition = { -1.0f,0.0f,0.0f };
	Vector3 modelRotation = { 0.0f,0.0f,0.0f };
	Vector3 modelScale = { 1.0f,1.0f,1.0f };

	Model* model = nullptr;

	Object3d* object3d2;
	Vector3 modelPosition2 = { 1.0f,0.0f,0.0f };
	Vector3 modelRotation2 = { 0.0f,0.0f,0.0f };
	Vector3 modelScale2 = { 1.0f,1.0f,1.0f };

	Model* model2 = nullptr;


	Camera* camera = nullptr;
	Vector3 cameraPosition = { 0.0f,0.0f,-15.0f };
	Vector3 cameraRotation = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };
};

