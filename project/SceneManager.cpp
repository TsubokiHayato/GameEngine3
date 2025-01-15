#include "SceneManager.h"
#include"ImGuiManager.h"
#include"DebugScene.h"
#include"TitleScene.h"
#include"StageScene.h"
#include"ClearScene.h"
void SceneManager::Initialize(Object3dCommon* object3dCommon, SpriteCommon* spriteCommon, WinApp* winApp, DirectXCommon* dxCommon)
{
	//情報の受け渡し
	this->object3dCommon = object3dCommon;
	this->spriteCommon = spriteCommon;
	this->winApp = winApp;
	this->dxCommon = dxCommon;

	//デバッグシーンの初期化
	currentScene = std::make_unique<DebugScene>();
	currentScene->Initialize(this->object3dCommon, this->spriteCommon);

	//シーン番号の初期化
	currentSceneNo = 0;
	//前のシーン番号の初期化
	prevSceneNo = -1;
}

void SceneManager::Update()
{
	//前のシーン番号を保存
	prevSceneNo = currentSceneNo;
	//現在のシーン番号を取得
	currentSceneNo = currentScene->GetSceneNo();

	//シーンが変わったら
	if (prevSceneNo != currentSceneNo) {
		//現在のシーンの終了処理
		if (currentScene != nullptr) {
			currentScene->Finalize();
		}
		//シーンの切り替え
		if (currentSceneNo == DEBUG) {
			//デバッグシーンの初期化
			currentScene = std::make_unique<DebugScene>();
			currentScene->Initialize(object3dCommon, spriteCommon);
		}
		else if (currentSceneNo == TITLE) {
			//タイトルシーンの初期化
			currentScene = std::make_unique<TitleScene>();
			currentScene->Initialize(object3dCommon, spriteCommon);
		}
		else if (currentSceneNo == STAGE) {
			//ステージシーンの初期化
			currentScene = std::make_unique<StageScene>();
			currentScene->Initialize(object3dCommon, spriteCommon);
		}
		else if (currentSceneNo == CLEAR) {
			//クリアシーンの初期化
			currentScene = std::make_unique<ClearScene>();
			currentScene->Initialize(object3dCommon, spriteCommon);
		}
		//シーンの初期化
		currentScene->Initialize(object3dCommon, spriteCommon);
	}


	//シーンの更新
	if (currentScene) {
		currentScene->Update();
	}
}

void SceneManager::Finalize()
{
	//現在のシーンの終了処理
	if (currentScene) {
		currentScene->Finalize();
	}

}

void SceneManager::Object3DDraw()
{
	//オブジェクト3Dの描画
	if (currentScene) {
		currentScene->Object3DDraw();
	}
}

void SceneManager::SpriteDraw()
{
	//スプライトの描画
	if (currentScene) {
		currentScene->SpriteDraw();
	}
}

void SceneManager::ImGuiDraw()
{
	//ImGuiの描画
	if (currentScene) {
		currentScene->ImGuiDraw();
	}

	//シーンの切り替え
	ImGui::Begin("Scene");
	if (ImGui::Button("Debug")) {
		currentScene->SetSceneNo(DEBUG);
	}
	if (ImGui::Button("Title")) {
		currentScene->SetSceneNo(TITLE);
	}
	if (ImGui::Button("Stage")) {
		currentScene->SetSceneNo(STAGE);
	}
	if (ImGui::Button("Clear")) {
		currentScene->SetSceneNo(CLEAR);
	}
	ImGui::End();

}
