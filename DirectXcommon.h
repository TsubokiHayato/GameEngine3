#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>

class DirectXcommon
{
public:
	void Initialize();
	void Update();


	//�f�o�C�X�̏�����
	void Device_Initialize();
	//�R�}���h�֘A�̏�����
	void Command_Initialize();
	//�X���b�v�`�F�[���̐���
	void SwapChain_Create();
private:
	//DirectX�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
};

