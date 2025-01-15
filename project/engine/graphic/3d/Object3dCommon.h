#pragma once
#include"DirectXCommon.h"
#include"WinApp.h"
class PSO;
class Camera;
class Object3dCommon
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon">DirectX共通部分</param>
	void Initialize(WinApp* winApp,DirectXCommon* dxCommon);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void DrawSettingsCommon();

	/*---------------------------------------------------
			GETTER & SETTER
	---------------------------------------------------*/
	DirectXCommon* GetDxCommon()const { return dxCommon_; }
	WinApp* GetWinApp()const { return winApp_; }

	void SetDefaultCamera(Camera* camera) { defaultCamera = camera; }
	Camera* GetDefaultCamera()const { return defaultCamera; }

private:
	WinApp* winApp_ = nullptr;//Object3dに引き渡すためのWinApp
	DirectXCommon* dxCommon_ = nullptr;//DirectX共通部分
	PSO* pso = nullptr;//PSO
	Camera* defaultCamera = nullptr;//デフォルトカメラ

};

