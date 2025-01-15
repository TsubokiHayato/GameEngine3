#pragma once

#include"DirectXcommon.h"
#include"D3DResourceLeakChecker.h"
#include"SpriteCommon.h"
#include"Object3dCommon.h"
#include"ModelCommon.h"
#include"TextureManager.h"
#include"ModelManager.h"
#include <SrvManager.h>
#include"AudioCommon.h"
#ifdef _DEBUG
#include"ImGuiManager.h"
#include"SceneManager.h"
#endif // DEBUG

#include"Input.h"

class Framework
{
public:
	//デストラクタ
	virtual ~Framework() = default;

	//初期化
	virtual void Initialize();

	//更新処理
	virtual void Update();

	//終了処理
	virtual void Finalize();

	//描画
	virtual void Draw() = 0;

public:

	/// <summary>
	/// フレームワークの描画前処理
	/// </summary>
	void FrameworkPreDraw();

	/// <summary>
	/// フレームワークの描画後処理
	/// </summary>
	void FrameworkPostDraw();

	/// <summary>
	/// ImGuiの描画前処理
	/// </summary>
	void ImGuiPreDraw();

	/// <summary>
	/// ImGuiの描画後処理
	/// </summary>
	void ImGuiPostDraw();

	/// <summary>
	///3Dオブジェクトの共通描画処理
	/// </summary>
	void Object3dCommonDraw();

	/// <summary>
	/// スプライトの共通描画処理
	/// </summary>
	void SpriteCommonDraw();

public:
	
		//終了リクエストがあったかどうか
		virtual bool IsEndRequest() { return endRequest; }

public:
	void Run();

protected:
	bool endRequest = false;

	//基盤システム
	WinApp* winApp = nullptr;
	//DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	//スプライト共通部分
	SpriteCommon* spriteCommon = nullptr;
	//オブジェクト3Dの共通部分
	Object3dCommon* object3dCommon = nullptr;
	//モデル共通部分
	ModelCommon* modelCommon = nullptr;
	//シェーダーリソースビューマネージャ
	SrvManager* srvManager = nullptr;
	//ImGuiマネージャ
	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;
	//シーンマネージャー
	std::unique_ptr<SceneManager> sceneManager = nullptr;
};

