#include "Stage.h"
#include"TextureManager.h"
#include"ModelManager.h"
#include"Object3dCommon.h"
#include"SpriteCommon.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"

#include"MapChipField.h"
Stage::~Stage()
{
	//入力の削除


	delete camera;
	delete player_;
	delete block;
}
void Stage::Initialize(WinApp* winApp, DirectXCommon* dxCommon, Object3dCommon* object3dCommon, ModelCommon* modelCommon, SpriteCommon* spriteCommon)
{


#pragma region テクスチャの読み込み

	//画像ハンドルをテクスチャマネージャに挿入する
	TextureManager::GetInstance()->LoadTexture(uvCheckerTextureHandle);
	TextureManager::GetInstance()->LoadTexture(monsterBallTextureHandle);


#pragma endregion テクスチャの読み込み

#pragma region ModelManagerの読み込み

	ModelManager::GetInstance()->LoadModel(modelFileNamePath);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath2);

#pragma endregion ModelManagerの読み込み


#pragma region Audioの初期化


	audio = std::make_unique<Audio>();
	audio->Initialize(audioFileName, audioDirectoryPath);
	audio->Play(true);

#pragma endregion Audioの初期化


#pragma region cameraの初期化

	camera = new Camera();
	cameraPosition = { 0.0f,15.0f,-30.0f };
	camera->SetTranslate(cameraPosition);
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	object3dCommon->SetDefaultCamera(camera);


#pragma endregion cameraの初期化


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

	audio_speed = player_->GetVelocity().x * 10.0f*2.0f;

}

void Stage::Update()
{

#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("Position", &cameraPosition.x);
	ImGui::DragFloat3("Rotation", &cameraRotation.x);
	ImGui::DragFloat3("Scale", &cameraScale.x);
	ImGui::End();




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



	player_->Update();
	block->Update();
	goal_->SetPlayer(player_);


	goal_->Update();

	audio->SetVolume(volume);
	
	
	if (!player_->GetIsAlive()){
		audio_speed -= 0.5f;
	}
	static bool isRestart = false;

	static int RestartCount = 0;
	if (audio_speed <= 0.0f)
	{
		RestartCount++;
		if (RestartCount <= 180)
		{

			isRestart = true;
		}
	}
	if (isRestart)
	{
		if (audio_speed < 2.0f)
		{
			audio_speed += 0.01f;
		}
		else
		{
			audio_speed = 2.0f;
			RestartCount = 0;
			isRestart = false;
		}

	}
	else {
		player_->SetVelocityX({ audio_speed / 5.0f });
	}
	
	Vector2 move = { 0.0f,0.0f };
	move.x = player_->GetPosition().x;

	camera->SetTranslate({ move.x, 15.0f,-30.0f });
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	camera->Update();


#ifdef _DEBUG
	ImGui::End();
#endif // DEBUG


}

void Stage::Draw()
{
	player_->Draw();
	block->Draw();
	goal_->Draw();

}
