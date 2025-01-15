#include "TitleScene.h"

void TitleScene::Initialize(Object3dCommon* object3dCommon, SpriteCommon* spriteCommon)
{
}

void TitleScene::Update()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Object3DDraw()
{
}

void TitleScene::SpriteDraw()
{
}

void TitleScene::ImGuiDraw()
{
	ImGui::Begin("TitleScene");
	ImGui::Text("Hello, TitleScene!");
	ImGui::End();
}
