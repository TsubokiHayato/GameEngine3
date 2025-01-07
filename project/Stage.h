#pragma once

#include"Audio.h"
#include"Camera.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"

#include"MapChipField.h"

#include"Block.h"
#include"Player.h"
#include"Goal.h"

class Stage
{
public:

	~Stage();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">ウィンドウアプリケーション</param>
	/// <param name="dxCommon">DirectX共通部分</param>
	/// <param name="object3dCommon">オブジェクト3D共通部分</param>
	/// <param name="modelCommon">モデル共通部分</param>
	/// <param name="spriteCommon">スプライト共通部分</param>
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
	float audio_speed = 2.0f;
	float playbackPosition = 0.0f;
	float volume = 0.1f;
	//カメラ
	Camera* camera = nullptr;
	Vector3 cameraPosition = { 0.0f,0.0f,-15.0f };
	Vector3 cameraRotation = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };

	
	/*mapChip*/
	MapChipField* mapChipField_ = nullptr;
	//MapChipField* StageMapChip[6];

	Block* block = nullptr;
	Player* player_ = nullptr;
	Goal* goal_ = nullptr;

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

