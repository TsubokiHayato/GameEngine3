#pragma once
#include"Input.h"
#include"Audio.h"
#include"Camera.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"
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
	//入力初期化
	Input* input = nullptr;
	//オーディオ
	std::unique_ptr<Audio> audio = nullptr;
	//カメラ
	Camera* camera = nullptr;
	Vector3 cameraPosition = { 0.0f,0.0f,-15.0f };
	Vector3 cameraRotation = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };

	//スプライト
	std::vector<Sprite*> sprites;
	//左右反転フラグ
	bool isFlipX_;
	//上下反転フラグ
	bool isFlipY_;
	//テクスチャの左上座標
	Vector2 textureLeftTop;
	//テクスチャから初期サイズを得るフラグ
	bool isAdjustTextureSize;

	//オブジェクト3D
	Object3d* objectBarrier;
	//モデル
	Model* modelBarrier = nullptr;
	Vector3 modelPosition = { -1.0f,0.0f,0.0f };
	Vector3 modelRotation = { 0.0f,0.0f,0.0f };
	Vector3 modelScale = { 1.0f,1.0f,1.0f };


	/*mapChip*/
	MapChipField* mapChipField_ = nullptr;
	//MapChipField* StageMapChip[6];


	

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


};

