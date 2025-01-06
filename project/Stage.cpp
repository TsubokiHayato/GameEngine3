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
	TextureManager::GetInstance()->LoadTexture(basupisuTextureHandle);

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
}

void Stage::Update()
{

#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("Position", &cameraPosition.x);
	ImGui::DragFloat3("Rotation", &cameraRotation.x);
	ImGui::DragFloat3("Scale", &cameraScale.x);
	ImGui::End();





	static float scratchPosition = 0.0f;
	static bool isScratching = false;
	static float lastScratchPosition = 0.0f;
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
	static float volume = 0.1f;
	ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
	audio->SetVolume(volume);

	// 再生バー
	static float playbackPosition = 0.0f;
	//再生位置の取得
	playbackPosition = audio->GetPlaybackPosition();
	//再生位置の視認
	ImGui::SliderFloat("Playback Position", &playbackPosition, 0.0f, duration);
	//audio->SetPlaybackPosition(playbackPosition);

	//speed
	static float speed = 0.0f;
	ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
	audio->SetPlaybackSpeed(speed);

	
	player_->Update();
	block->Update();



	ImGui::End();


#endif // DEBUG

	Vector2 move = { 0.0f,0.0f };
	move.x = player_->GetPosition().x;

	camera->SetTranslate({ move.x, 15.0f,-30.0f });
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	camera->Update();





}

void Stage::Draw()
{
	player_->Draw();
	block->Draw();

}
