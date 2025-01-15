#include "Order.h"

void Order::Initialize()
{
	//基盤システムの初期化
	Framework::Initialize();
}

void Order::Update()
{
	//更新処理
	Framework::Update();
#ifdef _DEBUG
	//ImGuiの受付開始
	Framework::ImGuiPreDraw();
	//ImGuiの描画
	Framework::ImGuiPostDraw();
#endif // DEBUG

}

void Order::Finalize()
{
	//終了処理
	Framework::Finalize();

}

void Order::Draw()
{
	//ループ前処理
	Framework::FrameworkPreDraw();
	//3Dオブジェクト描画
	Framework::Object3dCommonDraw();
	//2Dスプライト描画
	Framework::SpriteCommonDraw();
	//ループ後処理
	Framework::FrameworkPostDraw();
}
