#include "MyGame.h"

void MyGame::Initialize()
{
#pragma region ��ՃV�X�e���̏�����

	//�E�B���h�E�Y�A�v���P�[�V����
	
	winApp = new WinApp();
	winApp->Initialize();
#ifdef DEBUG
	//���[�N�`�F�b�J�[
	D3DResourceLeakChecker leakChecker;
#endif // _DEBUG


#ifdef DEBUG
	//���[�N�`�F�b�J�[
	D3DResourceLeakChecker leakChecker;


#endif // DEBUG

	//DirectX���ʕ���
	
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//�X�v���C�g���ʕ���
	
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);



	//�I�u�W�F�N�g3D�̋��ʕ���
	
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	//���f�����ʕ���
	
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);


	
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

#pragma endregion ��ՃV�X�e���̏�����


#pragma region TextureManeger�̏�����
	//�e�N�X�`���}�l�[�W���[�̏�����
	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);

	//�e�N�X�`���}�l�[�W���ɒǉ�����摜�n���h��
	std::string uvCheckerTextureHandle = "Resources/uvChecker.png";
	std::string monsterBallTextureHandle = "Resources/monsterBall.png";

	//�摜�n���h�����e�N�X�`���}�l�[�W���ɑ}������
	TextureManager::GetInstance()->LoadTexture(uvCheckerTextureHandle);
	TextureManager::GetInstance()->LoadTexture(monsterBallTextureHandle);

#pragma endregion TextureManeger�̏�����

#pragma region ModelManager�̏�����
	//���f���}�l�[�W���[�̏�����
	ModelManager::GetInstance()->initialize(dxCommon);

	//���f���f�B���N�g���p�X
	const std::string modelDirectoryPath = "Resources";
	//���f���t�@�C���p�X
	const std::string modelFileNamePath = "plane.obj";
	//���f���t�@�C���p�X2
	const std::string modelFileNamePath2 = "barrier.obj";

	ModelManager::GetInstance()->LoadModel(modelFileNamePath);
	ModelManager::GetInstance()->LoadModel(modelFileNamePath2);

#pragma endregion ModelManager�̏�����

#pragma region ImGuiManager�̏�����
#ifdef _DEBUG

	//ImGui�̏�����
	
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp, dxCommon);

#endif // DEBUG

#pragma endregion ImGuiManager�̏�����

#pragma region AudioCommon�̏�����
	//�I�[�f�B�I���ʕ�
	AudioCommon::GetInstance()->Initialize();
	const std::string audioFileName = "fanfare.wav";
	const std::string audioDirectoryPath = "Resources/Audio/";

#pragma endregion AudioCommon�̏�����
#pragma region Input�̏�����
	//���͏�����
	
	input = new Input();
	input->Initialize(winApp);
#pragma endregion Input�̏�����

#pragma region Audio�̏�����

	
	audio = std::make_unique<Audio>();
	audio->Initialize(audioFileName, audioDirectoryPath);
	audio->Play(true);

#pragma endregion Audio�̏�����
	/*---------------
		�X�v���C�g
	---------------*/
#pragma region �X�v���C�g�̏�����
	// �X�v���C�g������
	
	for (uint32_t i = 0; i < 1; ++i) {

		Sprite* sprite = new Sprite();

		//����for����i�������Ȃ�
		if (i % 2 == 0) {
			//�����X�^�[�{�[����\��������
			sprite->Initialize(spriteCommon, winApp, dxCommon, monsterBallTextureHandle);
		}
		else {
			//uvChecker��\��������
			sprite->Initialize(spriteCommon, winApp, dxCommon, uvCheckerTextureHandle);
		}


		// �e�X�v���C�g�ɈقȂ�ʒu��v���p�e�B��ݒ肷��
		//Vector2 spritePosition = { i * -1280.0f, 0.0f }; // �X�v���C�g���ƂɈقȂ�ʒu
		Vector2 spritePosition = { 100.0f, 100.0f }; // �X�v���C�g���ƂɈقȂ�ʒu
		float spriteRotation = 0.0f;                 // ��]�͔C��
		Vector4 spriteColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �F�͔��iRGBA�j
		Vector2 size = { 50.0f, 50.0f };             // �C�ӂ̃T�C�Y

		//�e��@�\���g����悤�ɂ���
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
#pragma endregion �X�v���C�g�̏�����

	/*---------------
	  �I�u�W�F�N�g3D
	---------------*/
#pragma region 3D���f���̏�����
	//�I�u�W�F�N�g3D
	
	object3d = new Object3d();
	object3d->Initialize(object3dCommon, winApp, dxCommon);

	


	//���f��
	
	model = new Model();
	model->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	object3d->SetModel(model);
	object3d->SetModel("plane.obj");

	////////////////////////////////////////////////////////////////////////



	//�I�u�W�F�N�g3D
	
	object3d2 = new Object3d();
	object3d2->Initialize(object3dCommon, winApp, dxCommon);




	//���f��
	
	model2 = new Model();
	model2->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath2);

	object3d2->SetModel(model2);
	object3d2->SetModel(modelFileNamePath2);

#pragma endregion 3D���f���̏�����

#pragma region camera�̏�����
	//�J����

	camera = new Camera();
	
	camera->SetTranslate(cameraPosition);
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	object3dCommon->SetDefaultCamera(camera);
	object3d->SetCamera(camera);
	object3d2->SetCamera(camera);

#pragma endregion camera�̏�����




}

void MyGame::Update()
{

	/*-------------------
	 Windows���b�Z�[�W����
	-------------------*/
	if (winApp->ProcessMessage()) {
		break;
	}


	/*-------------------
		 ���͂̍X�V
	-------------------*/

	/*-------
	  ImGui
	-------*/
#ifdef _DEBUG

	imGuiManager->Begin();

	ImGui::Begin("camera");
	ImGui::DragFloat3("Position", &cameraPosition.x);
	ImGui::DragFloat3("Rotation", &cameraRotation.x);
	ImGui::DragFloat3("Scale", &cameraScale.x);
	ImGui::End();

	//�X�v���C�g��ImGui
	 //�X�v���C�g��ImGui
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

	ImGui::Begin("Object3D2");
	ImGui::DragFloat3("Position", &modelPosition2.x);
	ImGui::DragFloat3("Rotation", &modelRotation2.x);
	ImGui::DragFloat3("Scale", &modelScale2.x);
	ImGui::End();


	static float scratchPosition = 0.0f;
	static bool isScratching = false;
	static float lastScratchPosition = 0.0f;
	//�Đ�����
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

	// �Đ��o�[
	static float playbackPosition = 0.0f;
	//�Đ��ʒu�̎擾
	playbackPosition = audio->GetPlaybackPosition();
	//�Đ��ʒu�̎��F
	ImGui::SliderFloat("Playback Position", &playbackPosition, 0.0f, duration);
	//audio->SetPlaybackPosition(playbackPosition);

	//speed
	static float speed = 1.0f;
	ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
	audio->SetPlaybackSpeed(speed);

	ImGui::End();

	ImGui::ShowDemoWindow();
	imGuiManager->End();
#endif // DEBUG

	/*--------------
	   �Q�[���̏���
	--------------*/
	camera->SetTranslate(cameraPosition);
	camera->setRotation(cameraRotation);
	camera->setScale(cameraScale);
	camera->Update();

	//���͂̍X�V
	input->Update();

	modelRotation.y += 0.01f;

	modelRotation2.x -= 0.01f;
	//modelRotation2.y -= 0.01f;
	modelRotation2.z -= 0.01f;

	//�I�u�W�F�N�g3D�̍X�V
	object3d->Update();

	object3d->SetPosition(modelPosition);
	object3d->SetRotation(modelRotation);
	object3d->SetScale(modelScale);

	object3d2->Update();

	object3d2->SetPosition(modelPosition2);
	object3d2->SetRotation(modelRotation2);
	object3d2->SetScale(modelScale2);


	//�X�v���C�g�̍X�V
	for (Sprite* sprite : sprites) {
		if (sprite) {
			// �����ł͊e�X�v���C�g�̈ʒu���]���X�V���鏈�����s��
			// ��: X�������ɏ������ړ�������
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

void MyGame::Finalize()
{


#pragma region AllRelease



	//���\�[�X���[�N�`�F�b�N

	//WindowsApp�̍폜
	winApp->Finalize();
	delete winApp;
	winApp = nullptr;

	//DirectX���ʕ����̍폜
	CloseHandle(dxCommon->GetFenceEvent());
	delete dxCommon;

	AudioCommon::GetInstance()->Finalize();
	//���͂̍폜
	delete input;
	//�J�����̍폜
	delete camera;

	//�X�v���C�g���ʕ����̍폜
	delete spriteCommon;


	for (Sprite* sprite : sprites) {
		if (sprite) {
			delete sprite; // �����������
		}
	}

	delete object3dCommon;
	delete object3d;

	delete modelCommon;
	delete model;

	delete model2;
	delete object3d2;
	sprites.clear(); // �|�C���^���N���A


	//�e�N�X�`���}�l�[�W���̏I��
	TextureManager::GetInstance()->Finalize();
	//���f���}�l�[�W���[�̏I��
	ModelManager::GetInstance()->Finalize();

#ifdef _DEBUG
	imGuiManager->Finalize();
#endif // DEBUG

	delete srvManager;


	//�x�����Ɏ~�܂�
	//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

#pragma endregion AllRelease

}

void MyGame::Draw()
{


	/*-------------------
	�@�@DirectX�`��J�n
	�@-------------------*/
	dxCommon->PreDraw();
	srvManager->PreDraw();
	/*-------------------
	�@�@�V�[���̕`��
�@�@-------------------*/



  //3D�I�u�W�F�N�g�̕`�揀���B3D�I�u�W�F�N�g�̕`��ɋ��ʂ̃O���t�B�b�N�X�R�}���h��ς�
	object3dCommon->DrawSettingsCommon();

	//�I�u�W�F�N�g3D�̕`��

#pragma region Draw3D

	object3d->Draw();
	object3d2->Draw();

#pragma endregion Draw3D

#pragma region Draw2D
	/*-------------------
			2D
	--------------------*/

	//2D�I�u�W�F�N�g�̕`�揀���B2D�I�u�W�F�N�g�̕`��ɋ��ʂ̃O���t�B�b�N�X�R�}���h��ς�
	spriteCommon->DrawSettingsCommon();

	// �`�揈��
	for (Sprite* sprite : sprites) {
		if (sprite) {
			sprite->Draw();
		}
	}

#pragma endregion Draw2D


#ifdef _DEBUG

	imGuiManager->Draw();
#endif // DEBUG

	/*-------------------
	�@�@DirectX�`��I��
  �@�@-------------------*/

	dxCommon->PostDraw();


}
