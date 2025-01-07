#include "Title.h"

#include"TextureManager.h"
#include"ModelManager.h"
#include"Object3dCommon.h"
#include"SpriteCommon.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"


void Title::Initialize(WinApp* winApp, DirectXCommon* dxCommon, Object3dCommon* object3dCommon, ModelCommon* modelCommon, SpriteCommon* spriteCommon)
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



	modelTitle_ = new Model();
	modelTitle_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);
	title_ = new Object3d();
	title_->Initialize(object3dCommon, winApp, dxCommon);
	title_->SetModel(modelFileNamePath);

	info_ = new Sprite();
	info_->Initialize(spriteCommon, winApp, dxCommon,uvCheckerTextureHandle);
	info_->SetPosition(infoPosition);
	info_->SetSize(infoSize);
	info_->SetAnchorPoint(infoAnchorPoint);
	info_->SetColor(infoColor);


}

void Title::Update() {


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
	static float speed = 1.0f;
	ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
	audio->SetPlaybackSpeed(speed);

	ImGui::End();

	
	title_->SetPosition(titlePosition);
	title_->SetRotation(titleRotation);
	title_->SetScale(titleScale);
	title_->Update();

	info_->Update();


}

void Title::Draw() {
	title_->Draw();
	info_->Draw();
}
