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
#ifdef _DEBUG

#include"ImGuiManager.h"

#endif // DEBUG

#include <iostream>
#include <algorithm>
#include"stage.h"
#include"Title.h"
#undef min//minマクロを無効化
#undef max//maxマクロを無効化

#pragma comment(lib,"dxguid.lib")//DirectXのライブラリ
#pragma comment(lib,"dxcompiler.lib")//DirectXのライブラリ


# define PI 3.14159265359f
#ifdef _DEBUG

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // DEBUG

enum Scene
{
	TitleScene,
	Game
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	/*-----------------------------------------------------------------------------------------------------------
	|																											|
	|												初期化処理													|
	|																											|
	-----------------------------------------------------------------------------------------------------------*/
#pragma region 基盤システムの初期化

	//ウィンドウズアプリケーション
	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();

#ifdef DEBUG
	//リークチェッカー
	D3DResourceLeakChecker leakChecker;

#endif // DEBUG

	//DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//スプライト共通部分
	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);



	//オブジェクト3Dの共通部分
	Object3dCommon* object3dCommon = nullptr;
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	//モデル共通部分
	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);


#pragma region AudioCommonの初期化
	//オーディオ共通部
	AudioCommon::GetInstance()->Initialize();
#pragma endregion AudioCommonの初期化


#pragma endregion 基盤システムの初期化


#pragma region ImGuiManagerの初期化
#ifdef _DEBUG

	//ImGuiの初期化
	std::unique_ptr< ImGuiManager> imGuiManager = nullptr;
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // DEBUG

#pragma endregion ImGuiManagerの初期化

#pragma region Inputの初期化
	//入力の初期化
	Input::GetInstance()->Initialize(winApp);
#pragma endregion Inputの初期化


#pragma region Manegerの初期化
	//テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon);
	//モデルマネージャーの初期化
	ModelManager::GetInstance()->initialize(dxCommon);
#pragma endregion Manegerの初期化

	Title* title = new Title();
	title->Initialize(winApp, dxCommon, object3dCommon, modelCommon, spriteCommon);

	Stage* stage = new Stage();
	stage->Initialize(winApp, dxCommon, object3dCommon, modelCommon, spriteCommon);


	Model* modelSkydome_;
	Object3d* skydome_;
	Vector3 rotate = {};
	//モデルディレクトリパス
	const std::string modelDirectoryPath = "Resources";
	//モデルファイルパス2
	const std::string modelFileNamePath = "barrier.obj";

	// モデル
	modelSkydome_ = new Model();
	modelSkydome_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	skydome_ = new Object3d();
	skydome_->Initialize(object3dCommon, winApp, dxCommon);
	skydome_->SetModel(modelFileNamePath);

	Scene scene = Scene::TitleScene;


	//ウィンドウの×ボタンんが押されるまでループ
	while (true) {
		/*-------------------
		 Windowsメッセージ処理
		-------------------*/
		if (winApp->ProcessMessage()) {
			break;
		}

		//ウィンドウにメッセージが来てたら最優先で処理させる


		/*-----------------------------------------------------------------------------------------------------------
		|																											|
		|												更新処理														|
		|																											|
		-----------------------------------------------------------------------------------------------------------*/

		/*-------------------
			 入力の更新
		-------------------*/


#ifdef _DEBUG
		imGuiManager->Begin();
#endif // DEBUG
		Input::GetInstance()->Update();

		/*--------------
		   ゲームの処理
		--------------*/


		

		
			
			
			stage->Update();

			



		


#ifdef _DEBUG
		imGuiManager->End();
#endif // DEBUG
		/*-----------------------------------------------------------------------------------------------------------
		|																											|
		|												描画処理														|
		|																											|
		-----------------------------------------------------------------------------------------------------------*/
#pragma region 描画処理

		/*-------------------
		　　DirectX描画開始
		　-------------------*/
		dxCommon->PreDraw();

		/*-------------------
		　　シーンの描画
	　　-------------------*/
	  //3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dCommon->DrawSettingsCommon();
		//2Dオブジェクトの描画準備。2Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		spriteCommon->DrawSettingsCommon();


		
			stage->Draw();
		

		



#ifdef _DEBUG

		imGuiManager->Draw();
#endif // DEBUG

		/*-------------------
		　　DirectX描画終了
	  　　-------------------*/

		dxCommon->PostDraw();

#pragma endregion 描画処理
	}

#pragma region AllRelease



	//リソースリークチェック

	//WindowsAppの削除
	winApp->Finalize();
	delete winApp;
	winApp = nullptr;

	//DirectX共通部分の削除
	CloseHandle(dxCommon->GetFenceEvent());
	delete dxCommon;

	AudioCommon::GetInstance()->Finalize();
	Input::GetInstance()->Finalize();


	//スプライト共通部分の削除
	delete spriteCommon;



	delete object3dCommon;


	delete modelCommon;

	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	//モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
#ifdef _DEBUG
	imGuiManager->Finalize();
#endif // DEBUG


	//警告時に止まる
	//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

#pragma endregion AllRelease


	return 0;

}