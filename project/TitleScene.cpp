#include "TitleScene.h"
#include"GamePlayScene.h"
#include "SceneManager.h"
#include"ImGuiManager.h"
void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//ゲームプレイシーンに遷移する
		BaseScene* scene = new GamePlayScene();
		SceneManager::GetInstance()->SetNextScene(scene);
	}
	ImGui::Begin("TitleScene");
	ImGui::Text("TitleScene");
	ImGui::End();
}

void TitleScene::Finalize()
{
}

void TitleScene::Draw()
{
}
