#pragma once
#include"DirectXCommon.h"
class ParticlePSO
{


public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon">DirectX���ʕ���</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// ���ʕ`��ݒ�
	/// </summary>
	void DrawSettingsCommon();

private:
	/*---------------------------------------------------
			�֐�
	---------------------------------------------------*/

	/// <summary>
	/// ���[�g�V�O�l�C�`���̍쐬
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̍쐬
	/// </summary>
	void CreateGraphicPipeline();

	/*-----------------
		rootSignature
	-----------------*/
	DirectXCommon* dxCommon_;

	//RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//DescriptorRange�쐬
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//
	D3D12_DESCRIPTOR_RANGE descriptorForInstancing[1] = {};
	//RootParameter�쐬�B
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	//Sampler�쐬
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//�V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr <ID3DBlob> errorBlob = nullptr;
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;
	/*------------
	  InputLayOut
	------------*/
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[4] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	/*------------
	  BlendState
	------------*/
	D3D12_BLEND_DESC blendDesc{};

	/*------------------
	  RasterizerState
	------------------*/

	//RasterizerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	/*-------------------
	  Vertex&Pixel_Shader
	-------------------*/

	//Shader���R���p�C������
	Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob;

	Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob;
	/*---------------
	DepthStencilDesc�̐ݒ�
	-------------------*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	/*------------------
	 �@ PSO�𐶐�����
	------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicPipelineStateDesc{};
	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipeLineState = nullptr;

	Microsoft::WRL::ComPtr <ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> commandList;




};

