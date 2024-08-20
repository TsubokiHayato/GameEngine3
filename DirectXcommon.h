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
	//�[�x�o�b�t�@�̐���
	void DepthBuffer_Create();
	//�e��f�B�X�N���v�^�q�[�v�̐���
	void DescriptorHeap_Create();
	//�����_�[�^�[�Q�b�g�r���[�̏�����
	void RTV_Initialize();
	//�[�x�X�e���V���r���[�̏�����
	void DSV_Initialize();
	//�t�F���X�̐���
	void Fence_Create();
	//�r���[�|�[�g��`�̏�����
	void viewport_Initialize();
	//�V�U�����O��`�̏�����
	void scissor_Initialize();
	//DXC�R���p�C���̐���
	void dxcCompiler_Create();
	//ImGui�̏�����
	void ImGui_Initialize();


	//DescriptorHeap�̂����������񂷂�
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> CreateDescriptorHeap(
		Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);


	Microsoft::WRL::ComPtr <ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, int32_t width, int32_t height);


	//CPU��DescriptorHandle���擾����֐�
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	//GPU��DescriptorHandle���擾����֐�
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	//SRV�̎w��ԍ���CPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	//SRV�̎w��ԍ���GPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	//RTV�̎w��ԍ���CPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	//RTV�̎w��ԍ���GPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	//DSV�̎w��ԍ���CPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	//DSV�̎w��ԍ���GPU�f�B�X�N���v�^�n���h�����擾����
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);
private:

};

