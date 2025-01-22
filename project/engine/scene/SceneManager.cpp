#include "SceneManager.h"
#include"ImGuiManager.h"
#include"DebugScene.h"
#include"TitleScene.h"
#include"StageScene.h"
#include"ClearScene.h"
void SceneManager::Initialize(Object3dCommon* object3dCommon, SpriteCommon* spriteCommon, ParticleCommon* particleCommon, WinApp* winApp, DirectXCommon* dxCommon)
{
	//�e���ʕ����̃|�C���^���󂯎��
	this->object3dCommon = object3dCommon;
	this->spriteCommon = spriteCommon;
	this->winApp = winApp;
	this->dxCommon = dxCommon;
	this->particleCommon = particleCommon;

	//�����V�[����ݒ�
	currentScene = std::make_unique<DebugScene>();
	currentScene->Initialize(this->object3dCommon,this->spriteCommon,this->particleCommon,this->winApp,this->dxCommon);

	//�V�[���ԍ���ݒ�
	currentSceneNo = 0;
	//�O�̃V�[���ԍ���ݒ�
	prevSceneNo = -1;
}

void SceneManager::Update()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene == nullptr) {
		return;
	}
	//�O�̃V�[���ԍ���ݒ�
	prevSceneNo = currentSceneNo;
	//���݂̃V�[���ԍ����擾
	currentSceneNo = currentScene->GetSceneNo();

	//�O�̃V�[���ԍ��ƌ��݂̃V�[���ԍ����قȂ�ꍇ
	if (prevSceneNo != currentSceneNo) {
		//���݂̃V�[����nullptr�łȂ��ꍇ
		if (currentScene != nullptr) {
			//�I������
			currentScene->Finalize();
		}

		//�V�[���ԍ��ɂ���ăV�[����ݒ�
		if (currentSceneNo == DEBUG) {
			//�f�o�b�O�V�[����ݒ�
			currentScene = std::make_unique<DebugScene>();
			currentScene->Initialize(object3dCommon, spriteCommon,particleCommon, winApp, dxCommon);
		}
		else if (currentSceneNo == TITLE) {
			//�^�C�g���V�[����ݒ�
			currentScene = std::make_unique<TitleScene>();
			currentScene->Initialize(object3dCommon, spriteCommon, particleCommon, winApp, dxCommon);
		}
		else if (currentSceneNo == STAGE) {
			//�X�e�[�W�V�[����ݒ�
			currentScene = std::make_unique<StageScene>();
			currentScene->Initialize(object3dCommon, spriteCommon, particleCommon, winApp, dxCommon);
		}
		else if (currentSceneNo == CLEAR) {
			//�N���A�V�[����ݒ�
			currentScene = std::make_unique<ClearScene>();
			currentScene->Initialize(object3dCommon, spriteCommon, particleCommon, winApp, dxCommon);
		}
		//������
		currentScene->Initialize(object3dCommon, spriteCommon, particleCommon, winApp, dxCommon);
	}
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//�X�V����
		currentScene->Update();
	}
}

void SceneManager::Finalize()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//�I������
		currentScene->Finalize();
	}

}

void SceneManager::Object3DDraw()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//3D�I�u�W�F�N�g�`��
		currentScene->Object3DDraw();
	}
}

void SceneManager::SpriteDraw()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//�X�v���C�g�`��
		currentScene->SpriteDraw();
	}
}

void SceneManager::ImGuiDraw()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//ImGui�`��
		currentScene->ImGuiDraw();
	}

	//�V�[���I���E�B���h�E
	ImGui::Begin("Scene");
	if (ImGui::Button("Debug")) {
		currentScene->SetSceneNo(DEBUG);
	}
	if (ImGui::Button("Title")) {
		currentScene->SetSceneNo(TITLE);
	}
	if (ImGui::Button("Stage")) {
		currentScene->SetSceneNo(STAGE);
	}
	if (ImGui::Button("Clear")) {
		currentScene->SetSceneNo(CLEAR);
	}
	ImGui::End();

}

void SceneManager::ParticleDraw()
{
	//���݂̃V�[����nullptr�łȂ��ꍇ
	if (currentScene) {
		//�p�[�e�B�N���`��
		currentScene->ParticleDraw();
	}
}
