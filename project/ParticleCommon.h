#pragma once
#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
class ParticlePSO;
class Camera;
class ParticleCommon
{

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon">DirectX���ʕ���</param>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// ���ʕ`��ݒ�
	/// </summary>
	void DrawSettingsCommon();

	/*---------------------------------------------------
			GETTER & SETTER
	---------------------------------------------------*/
	DirectXCommon* GetDxCommon()const { return dxCommon_; }
	WinApp* GetWinApp()const { return winApp_; }

	void SetDefaultCamera(Camera* camera) { defaultCamera = camera; }
	Camera* GetDefaultCamera()const { return defaultCamera; }

private:

	WinApp* winApp_ = nullptr;//�E�B���h�E�Y�A�v���P�[�V����
	DirectXCommon* dxCommon_ = nullptr;//DirectX���ʕ���
	ParticlePSO* pso = nullptr;//PSO
	Camera* defaultCamera = nullptr;//�f�t�H���g�J����


};

