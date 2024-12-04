#include "Stage.h"
#include"TextureManager.h"
#include"ModelManager.h"
#include"Object3dCommon.h"
#include"SpriteCommon.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"
Stage::~Stage()
{
//入力の削除
	delete input;

	for (Sprite* sprite : sprites) {
		if (sprite) {
			delete sprite; // メモリを解放
		}
	}
	delete objectBarrier;
	delete modelBarrier;
	sprites.clear();
}
void Stage::Initialize(WinApp* winApp,DirectXCommon*dxCommon,Object3dCommon* object3dCommon,ModelCommon*modelCommon,SpriteCommon* spriteCommon)
{


#pragma region テクスチャの読み込み
	
	//画像ハンドルをテクスチャマネージャに挿入する
	TextureManager::GetInstance()->LoadTexture(uvCheckerTextureHandle);
	TextureManager::GetInstance()->LoadTexture(monsterBallTextureHandle);
#pragma endregion テクスチャの読み込み

#pragma region ModelManagerの読み込み


	

	ModelManager::GetInstance()->LoadModel(modelFileNamePath);

#pragma endregion ModelManagerの読み込み


#pragma region Inputの初期化
	
	input = new Input();
	input->Initialize(winApp);
#pragma endregion Inputの初期化

#pragma region Audioの初期化

	
	audio = std::make_unique<Audio>();
	audio->Initialize(audioFileName, audioDirectoryPath);
	audio->Play(true);

#pragma endregion Audioの初期化


#pragma region cameraの初期化
	
	camera = new Camera();
	
	camera->SetTranslate(cameraPosition);
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	object3dCommon->SetDefaultCamera(camera);


#pragma endregion cameraの初期化

	/*---------------
		スプライト
	---------------*/
#pragma region スプライトの初期化
	


	
	for (uint32_t i = 0; i < 1; ++i) {

		Sprite* sprite = new Sprite();

		//もしfor文のiが偶数なら
		if (i % 2 == 0) {
			//モンスターボールを表示させる
			sprite->Initialize(spriteCommon, winApp, dxCommon, monsterBallTextureHandle);
		}
		else {
			//uvCheckerを表示させる
			sprite->Initialize(spriteCommon, winApp, dxCommon, uvCheckerTextureHandle);
		}


		// 各スプライトに異なる位置やプロパティを設定する
		//Vector2 spritePosition = { i * -1280.0f, 0.0f }; // スプライトごとに異なる位置
		Vector2 spritePosition = { 100.0f, 100.0f }; // スプライトごとに異なる位置
		float spriteRotation = 0.0f;                 // 回転は任意
		Vector4 spriteColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白（RGBA）
		Vector2 size = { 50.0f, 50.0f };             // 任意のサイズ

		//各種機能を使えるようにする
		isFlipX_ = sprite->GetFlipX();
		isFlipY_ = sprite->GetFlipY();
		textureLeftTop = sprite->GetTextureLeftTop();
		isAdjustTextureSize = sprite->GetIsAdjustTextureSize();


		sprite->SetPosition(spritePosition);
		sprite->SetRotation(spriteRotation);
		sprite->SetColor(spriteColor);
		sprite->SetSize(size);
		sprite->SetTextureLeftTop(textureLeftTop);
		sprite->SetGetIsAdjustTextureSize(isAdjustTextureSize);

		sprites.push_back(sprite);
	}
#pragma endregion スプライトの初期化

	/*---------------
	  オブジェクト3D
	---------------*/
#pragma region 3Dモデルの初期化
	//オブジェクト3D
	objectBarrier = new Object3d();
	objectBarrier->Initialize(object3dCommon, winApp, dxCommon);

	
	//モデル
	modelBarrier = new Model();
	modelBarrier->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	objectBarrier->SetModel(modelBarrier);
	objectBarrier->SetModel(modelFileNamePath);

	////////////////////////////////////////////////////////////////////////



#pragma endregion 3Dモデルの初期化


}

void Stage::Update()
{

#ifdef _DEBUG
		ImGui::Begin("camera");
		ImGui::DragFloat3("Position", &cameraPosition.x);
		ImGui::DragFloat3("Rotation", &cameraRotation.x);
		ImGui::DragFloat3("Scale", &cameraScale.x);
		ImGui::End();


		//スプライトのImGui
		for (Sprite* sprite : sprites) {
			if (sprite) {
				ImGui::Begin("Sprite");
				ImGui::SetWindowSize({ 500,100 });

				Vector2 spritePosition = sprite->GetPosition();
				ImGui::SliderFloat2("Position", &spritePosition.x, 0.0f, 1920.0f, "%.1f");
				sprite->SetPosition(spritePosition);

				/*	ImGui::Checkbox("isFlipX", &isFlipX_);
					ImGui::Checkbox("isFlipY", &isFlipY_);
					ImGui::Checkbox("isAdjustTextureSize", &isAdjustTextureSize);
					ImGui::DragFloat2("textureLeftTop", &textureLeftTop.x);*/
				ImGui::End();
			}
		}
		ImGui::Begin("Object3D");
		ImGui::DragFloat3("Position", &modelPosition.x);
		ImGui::DragFloat3("Rotation", &modelRotation.x);
		ImGui::DragFloat3("Scale", &modelScale.x);
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
		static float speed = 1.0f;
		ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
		audio->SetPlaybackSpeed(speed);

		ImGui::End();


#endif // DEBUG


		camera->SetTranslate(cameraPosition);
		camera->setRotation(cameraRotation);
		camera->setScale(cameraScale);
		camera->Update();

		//入力の更新
		input->Update();

		

		modelRotation.x -= 0.01f;
		//modelRotation2.y -= 0.01f;
		modelRotation.z -= 0.01f;

		//オブジェクト3Dの更新
		objectBarrier->Update();

		objectBarrier->SetPosition(modelPosition);
		objectBarrier->SetRotation(modelRotation);
		objectBarrier->SetScale(modelScale);



		//スプライトの更新
		for (Sprite* sprite : sprites) {
			if (sprite) {
				// ここでは各スプライトの位置や回転を更新する処理を行う
				// 例: X軸方向に少しずつ移動させる
				Vector2 currentPosition = sprite->GetPosition();
				/*currentPosition.x = 100.0f;
				currentPosition.y = 100.0f;*/
				float currentRotation = sprite->GetRotation();

				sprite->SetPosition(currentPosition);
				sprite->SetRotation(currentRotation);
				sprite->SetTextureLeftTop(textureLeftTop);
				sprite->SetFlipX(isFlipX_);
				sprite->SetFlipY(isFlipY_);
				sprite->SetGetIsAdjustTextureSize(isAdjustTextureSize);

				sprite->Update();
			}
		}

}

void Stage::Draw()
{
	objectBarrier->Draw();
	for (Sprite* sprite : sprites) {
		if (sprite) {
			sprite->Draw();
		}
	}

}
