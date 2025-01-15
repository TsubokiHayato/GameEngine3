#include "Framework.h"
#include"WinApp.h"

void Framework::Initialize()
{

	//ウィンドウズアプリケーション
	winApp = new WinApp();
	winApp->Initialize();
#ifdef DEBUG
	//リークチェッカー
	D3DResourceLeakChecker leakChecker;
#endif // _DEBUG

	//DirectX共通部分
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);



#ifdef _DEBUG

	//ImGuiの初期化
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // DEBUG

	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	//スプライト共通部分
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(winApp,dxCommon);

	//オブジェクト3Dの共通部分
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(winApp,dxCommon);

	//モデル共通部分
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	//テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);

	//モデルマネージャーの初期化
	ModelManager::GetInstance()->initialize(dxCommon);

	//オーディオ共通部
	AudioCommon::GetInstance()->Initialize();

	//入力初期化
	Input::GetInstance()->Initialize(winApp);

	//シーンマネージャーの初期化
	sceneManager = std::make_unique<SceneManager>();
	sceneManager->Initialize(object3dCommon, spriteCommon, winApp, dxCommon);


}
void Framework::Update()
{
	//メッセージ処理
	if (winApp->ProcessMessage()) {
		endRequest = true;
	}
	//入力の更新
	Input::GetInstance()->Update();
	//シーンマネージャーの更新
	sceneManager->Update();


}

void Framework::Finalize()
{

	//ImGuiManagerの終了
#ifdef _DEBUG
	imGuiManager->Finalize();
#endif // DEBUG

	//スプライト共通部分の削除
	delete spriteCommon;



	AudioCommon::GetInstance()->Finalize();

	Input::GetInstance()->Finalize();

	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	//モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();

	delete object3dCommon;
	delete modelCommon;
	delete srvManager;

	//DirectX共通部分の削除
	CloseHandle(dxCommon->GetFenceEvent());
	delete dxCommon;

	//WindowsAppの削除
	winApp->Finalize();
	delete winApp;
	winApp = nullptr;

}


void Framework::Run()
{
	Initialize();
	while (true)
	{
		Update();
		if (IsEndRequest())
		{
			break;
		}
		Draw();
	}
	Finalize();
}

void Framework::FrameworkPreDraw()
{
	//描画前処理
	dxCommon->PreDraw();
	//ImGuiの描画前処理
	srvManager->PreDraw();
}

void Framework::FrameworkPostDraw()
{
#ifdef _DEBUG
	//ImGuiの描画
	imGuiManager->Draw();
#endif // DEBUG
	//描画
	dxCommon->PostDraw();
}

void Framework::ImGuiPreDraw()
{
	//ImGuiの受付開始
	imGuiManager->Begin();
#ifdef _DEBUG
	sceneManager->ImGuiDraw();
#endif // _DEBUG

}

void Framework::ImGuiPostDraw()
{
	//ImGuiの受付終了
	ImGui::ShowDemoWindow();
	//ImGuiの終了
	imGuiManager->End();

}

void Framework::Object3dCommonDraw()
{
	//オブジェクト3Dの描画
	object3dCommon->DrawSettingsCommon();
	//シーンマネージャーのオブジェクト3D描画
	sceneManager->Object3DDraw();
}

void Framework::SpriteCommonDraw()
{
	//スプライトの描画
	spriteCommon->DrawSettingsCommon();
	//シーンマネージャーのスプライト描画
	sceneManager->SpriteDraw();
}
