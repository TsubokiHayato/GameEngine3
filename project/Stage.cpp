#include "Stage.h"
#include"TextureManager.h"
#include"ModelManager.h"
#include"Object3dCommon.h"
#include"SpriteCommon.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"
#include"Input.h"
#include"MapChipField.h"
Stage::~Stage()
{
	//入力の削除


	delete camera;
	delete player_;
	delete block;
	delete goal_;
	delete mapChipField_;
	delete modelSkydome_;
	delete skydome_;
	delete skydome2_;
	delete modelSkydome2_;
	delete infoSprite;
	delete title_;
	audio->~Audio();
	
}
void Stage::Initialize(WinApp* winApp, DirectXCommon* dxCommon, Object3dCommon* object3dCommon, ModelCommon* modelCommon, SpriteCommon* spriteCommon)
{


#pragma region テクスチャの読み込み

	//画像ハンドルをテクスチャマネージャに挿入する
	TextureManager::GetInstance()->LoadTexture(uvCheckerTextureHandle);
	TextureManager::GetInstance()->LoadTexture(monsterBallTextureHandle);
	TextureManager::GetInstance()->LoadTexture(infoTextureHandle);

#pragma endregion テクスチャの読み込み

#pragma region ModelManagerの読み込み

	ModelManager::GetInstance()->LoadModel(modelFileNamePath);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath2);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath3);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath4);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath5);
#pragma endregion ModelManagerの読み込み


#pragma region Audioの初期化


	audio = std::make_unique<Audio>();
	audio->Initialize(audioFileName, audioDirectoryPath);
	audio->Play(true);

#pragma endregion Audioの初期化


#pragma region cameraの初期化

	camera = new Camera();
	cameraPosition = { 0.0f,15.0f,-20.0f };
	camera->SetTranslate(cameraPosition);
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	object3dCommon->SetDefaultCamera(camera);


#pragma endregion cameraの初期化

	modelTitle_ = new Model();
	modelTitle_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath4);
	title_ = new Object3d();
	title_->Initialize(object3dCommon, winApp, dxCommon);
	title_->SetModel(modelFileNamePath4);
	title_->SetPosition({ 0.0f,18.0f });

	infoSprite = new Sprite();
	infoSprite->Initialize(spriteCommon, winApp, dxCommon, infoTextureHandle);
	static Vector2 infoPosition = { 0.0f,0.0f };
	infoSprite->SetPosition(infoPosition);
	infoSprite->SetAnchorPoint({ 0.5f,0.5f });

	infoSprite->SetGetIsAdjustTextureSize(true);





	// Mapの生成
	mapChipField_ = new MapChipField;
	// Mapのよみこみ
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	block = new Block();
	block->Initialize(object3dCommon, modelCommon, dxCommon, winApp, mapChipField_);

	player_ = new Player();
	player_->Initialize(object3dCommon, modelCommon, dxCommon, winApp, mapChipField_);
	player_->SetCamera(camera);

	goal_ = new Goal;
	goal_->Initialize(object3dCommon, modelCommon, dxCommon, winApp, mapChipField_);

	audio_speed = 0.0f;




	// モデル
	modelSkydome_ = new Model();
	modelSkydome_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	skydome_ = new Object3d();
	skydome_->Initialize(object3dCommon, winApp, dxCommon);
	skydome_->SetModel(modelFileNamePath);


	// モデル
	modelSkydome2_ = new Model();
	modelSkydome2_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath3);

	skydome2_ = new Object3d();
	skydome2_->Initialize(object3dCommon, winApp, dxCommon);
	skydome2_->SetModel(modelFileNamePath3);



}

void Stage::Update()
{

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{     
		isStage = true;
		player_->SetIsGoal(false);
	}

	if (!isStage) {

		
		title_->Update();
		infoSprite->Update();
		static Vector2 infoPosition = { 640.0f,525.0f };
		

		static Vector3 modelPosition = { 0.0f,17.0f,0.0f };
		title_->SetScale({ 2.0f,2.0f,2.0f });
		title_->SetPosition(modelPosition);
		ImGui::Begin("title");
		ImGui::DragFloat3("Position", &modelPosition.x);
		ImGui::End();

		infoSprite->SetPosition(infoPosition);
	}
	else {
#ifdef _DEBUG


		
		//再生時間
		float duration = audio->GetSoundDuration();


		ImGui::Begin("Audio Control");

		if (ImGui::Button("Play")) {
			audio->Play(false);
		}
		if (ImGui::Button("Stop")) {
			audio->Stop();
		}
		if (ImGui::Button("Pause")) {
			audio->Pause();
		}
		if (ImGui::Button("Resume")) {
			audio->Resume();
		}

		//volume
		ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
		// 再生バー
		//再生位置の取得
		playbackPosition = audio->GetPlaybackPosition();
		//再生位置の視認
		ImGui::SliderFloat("Playback Position", &playbackPosition, 0.0f, duration);
		//speed
		ImGui::SliderFloat("Speed", &audio_speed, 0.0f, 2.0f);

#endif // DEBUG
		//カメラの位置
		ImGui::Begin("camera");
		ImGui::DragFloat3("Position", &cameraPosition.x);
		ImGui::DragFloat3("Rotation", &cameraRotation.x);
		ImGui::DragFloat3("Scale", &cameraScale.x);
		ImGui::End();


		player_->Update();
		block->Update();
		goal_->SetPlayer(player_);


		goal_->Update();

		audio->SetVolume(volume);


		if (!player_->GetIsAlive()) {
			audio_speed = 0.0f;
		}

		static bool isRestart = false;
		if (audio_speed <= 0.0f)
		{
			player_->SetVelocityX(0.0f);
			player_->SetRotation({ 0.0f,0.0f,0.0f });

			isRestart = true;
		}

		if (isRestart)
		{
			if (audio_speed < 1.0f)
			{
				audio_speed += 0.01f;
				player_->SetRotation({ 0.0f,0.0f,0.0f });
			}
			else
			{
				audio_speed = 1.0f;
				isRestart = false;
				player_->SetVelocityX(0.2f);
				

			}

		}

		Vector2 move = { 0.0f,0.0f };
		move.x = player_->GetPosition().x;
		audio->SetPlaybackSpeed(audio_speed);
		camera->SetTranslate({ move.x, 15.0f,-20.0f });
		

		if (player_->GetIsGoal())
		{
			isStage = false;
		}


#ifdef _DEBUG
		ImGui::End();
#endif // DEBUG
	}

	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	camera->Update();

	rotate.x += 0.0008f;
	rotate.y += 0.0008f;
	rotate.z += 0.0008f;
	skydome_->SetScale({ 1000.0f, 1000.0f, 1000.0f });
	skydome_->SetRotation(rotate);
	skydome_->SetPosition({ 0.0f,15.0f,0.0f });
	skydome_->Update();


	skydome2_->SetScale({ 1.0f, 1.0f, 1.0f });
	skydome2_->SetRotation({});
	skydome2_->SetPosition({ 0.0f,15.0f,0.0f });
	skydome2_->Update();

}

void Stage::Draw()
{
	if (isStage) {
		player_->Draw();
		block->Draw();
		goal_->Draw();

	}
	else {
		title_->Draw();
		infoSprite->Draw();
	}

	skydome2_->Draw();
	skydome_->Draw(); 
	
	

}
