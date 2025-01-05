#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp) {

	this->object3dCommon = Object3dCommon;
	this->dxCommon = dxCommon;
	this->winApp = winApp;
	
	//input->Initialize(winApp);

	// モデル
	model_ = new Model();
	model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	object3d = new Object3d();
	object3d->Initialize(object3dCommon, this->winApp, this->dxCommon);
	object3d->SetModel(modelFileNamePath);

	modelPosition = {-1.0f, 0.0f, 0.0f};
	modelRotation = {0.0f, 0.0f, 0.0f};
	modelScale = {1.0f, 1.0f, 1.0f};

}

void Player::Update() {
	/*if (input->TriggerKey(DIK_A)) {
		modelPosition.x += 0.01f;
	}*/
	
		object3d->Update();

		object3d->SetPosition(modelPosition);
		object3d->SetRotation(modelRotation);
		object3d->SetScale(modelScale);
	
}

void Player::Draw() { object3d->Draw(); }
