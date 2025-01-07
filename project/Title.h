#pragma once

#include"Audio.h"
#include"Camera.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"

#include"MapChipField.h"
class Title
{
public:
	void Initialize(
		WinApp* winApp,
		DirectXCommon* dxCommon,
		Object3dCommon* object3dCommon,
		ModelCommon* modelCommon,
		SpriteCommon* spriteCommon);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


	private:
		//オーディオ
		std::unique_ptr<Audio> audio = nullptr;
		//カメラ
		Camera* camera = nullptr;
		Vector3 cameraPosition = { 0.0f,0.0f,-15.0f };
		Vector3 cameraRotation = { 0.0f,0.0f,0.0f };
		Vector3 cameraScale = { 1.0f,1.0f,1.0f };


		/*mapChip*/
		MapChipField* mapChipField_ = nullptr;
		
		Model* modelTitle_;
		Object3d* title_;
		Vector3 titlePosition = { 0.0f,0.0f,0.0f };
		Vector3 titleRotation = { 0.0f,0.0f,0.0f };
		Vector3 titleScale = { 1.0f,1.0f,1.0f };

		Sprite* info_;
		Vector2 infoPosition = { 0.0f,0.0f };
		Vector2 infoSize = { 1.0f,1.0f };
		Vector2 infoAnchorPoint = { 0.5f,0.5f };
		Vector4 infoColor = { 1.0f,1.0f,1.0f,1.0f };


		//SEファイル名
		const std::string audioFileName = "fanfare.wav";
		//SEディレクトリパス
		const std::string audioDirectoryPath = "Resources/Audio/";

		//テクスチャマネージャに追加する画像ハンドル
		std::string uvCheckerTextureHandle = "Resources/uvChecker.png";
		std::string monsterBallTextureHandle = "Resources/monsterBall.png";

		//モデルディレクトリパス
		const std::string modelDirectoryPath = "Resources";

		//モデルファイルパス2
		const std::string modelFileNamePath = "barrier.obj";
		const std::string modelFileNamePath2 = "cube.obj";


};

