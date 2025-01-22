#pragma once
#include"IScene.h"
#include"Particle.h"
#include"Camera.h"
#include"ParticleEmitter.h"

class TitleScene :public IScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="object3dCommon">3D�I�u�W�F�N�g���ʕ���</param>
	/// <param name="spriteCommon">�X�v���C�g���ʕ���</param>
	/// <param name="particleCommon">�p�[�e�B�N�����ʕ���</param>
	/// <param name="winApp">�E�B���h�E�A�v���P�[�V����</param>
	/// <param name="dxCommon">DirectX���ʕ���</param>
	void Initialize(Object3dCommon* object3dCommon, SpriteCommon* spriteCommon, ParticleCommon* particleCommon, WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 3D�I�u�W�F�N�g�`��
	/// </summary>
	void Object3DDraw()override;

	/// <summary>
	/// �X�v���C�g�`��
	/// </summary>
	void SpriteDraw()override;

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGuiDraw()override;

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void ParticleDraw()override;

private:
	Object3dCommon* object3dCommon;
	SpriteCommon* spriteCommon;
	ParticleCommon* particleCommon;
	WinApp* winApp;
	DirectXCommon* dxCommon;

private:
	std::unique_ptr <Particle> particle;
	std::unique_ptr<ParticleEmitter> particleEmitter_;

	std::unique_ptr <Camera>camera;
	Vector3 cameraPosition = { 0.0f,0.0f,-5.0f };
	Vector3 cameraRotation = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };



};

