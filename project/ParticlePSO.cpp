#include "ParticlePSO.h"

void ParticlePSO::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	dxCommon_ = dxCommon;
	device = dxCommon_->GetDevice();
	commandList = dxCommon_->GetCommandList();


	//�O���t�B�b�N�X�p�C�v���C���̍쐬
	CreateGraphicPipeline();

}

void ParticlePSO::DrawSettingsCommon()
{
	/*���[�g�V�O�l�C�`�����Z�b�g����R�}���h*/
	 //RootSignature��ݒ�B

	commandList->SetGraphicsRootSignature(rootSignature.Get());


	/*�O���t�B�N�X�p�C�v���C���X�e�[�g���Z�b�g����R�}���h*/

	commandList->SetPipelineState(graphicsPipeLineState.Get());//PSO��ݒ�


	/*�v���~�e�B�u�g�|���W�[���Z�b�g����R�}���h*/
	//�`��ݒ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void ParticlePSO::CreateRootSignature()
{

	//RootSignature�쐬
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange�쐬

	descriptorRange[0].BaseShaderRegister = 0;//0����n�܂�
	descriptorRange[0].NumDescriptors = 1;//����1
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRV������
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offset�������v�Z

	descriptorForInstancing[0].BaseShaderRegister = 0;//0����n�܂�
	descriptorForInstancing[0].NumDescriptors = 1;//����1
	descriptorForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRV������
	descriptorForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offset�������v�Z



	//RootParameter�쐬�B02_01�ǉ�//02_03�X�V

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBV���g��
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	rootParameters[0].Descriptor.ShaderRegister = 0;//���W�X�^�ԍ�0�ƃo�C���h

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorForInstancing);

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DESCRIPTOR_TABLE���g��
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootSignature.pParameters = rootParameters;//���[�g�p�����[�^�z��ւ̃|�C���^
	descriptionRootSignature.NumParameters = _countof(rootParameters);//�z��̒���


#pragma region Sampler_03_00//


	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�o�C�A�j�A�t�B���^
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//��r���Ȃ�	
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//�����������MipMap������
	staticSamplers[0].ShaderRegister = 0;//���W�X�^�ԍ�0������
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


#pragma endregion





	dxCommon_->hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(dxCommon_->hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//�o�C�i�������ɍ쐬

	dxCommon_->hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(dxCommon_->hr));


}

void ParticlePSO::CreateGraphicPipeline()
{
	//���[�g�V�O�l�C�`���v�̍쐬
	CreateRootSignature();



	/*------------
	  InputLayOut
	------------*/

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;


	inputElementDescs[3].SemanticName = "COLOR";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;



	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/*------------
	  BlendState
	------------*/


	//���ׂĂ̐F�v�f����������
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA; // provided code
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;


	/*------------------
	  RasterizerState
	------------------*/

	//���ʕ\��
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//�O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/*-------------------
	  Vertex&Pixel_Shader
	-------------------*/

	//Shader���R���p�C������
	vertexShaderBlob = dxCommon_->CompileShader(L"Resources/Shaders/Particle.VS.hlsl",
		L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = dxCommon_->CompileShader(L"Resources/Shaders/Particle.PS.hlsl",
		L"ps_6_0");
	assert(pixelShaderBlob != nullptr);


	/*---------------
	DepthStencilDesc�̐ݒ�
	-------------------*/

	//Depth�̋@�\��L����
	depthStencilDesc.DepthEnable = true;
	//�������݂��܂�
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//��r�֐���LessEqual
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	/*------------------
	 �@ PSO�𐶐�����
	------------------*/


	graphicPipelineStateDesc.pRootSignature = rootSignature.Get();//RootSignature
	graphicPipelineStateDesc.InputLayout = inputLayoutDesc;//InputLayOut

	graphicPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };//vertexShader

	graphicPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };//pixcelShader

	graphicPipelineStateDesc.BlendState = blendDesc;//BlendState
	graphicPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState

	//��������RTV�̏��
	graphicPipelineStateDesc.NumRenderTargets = 1;
	graphicPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//���p����g�|���W(�`��)�̃^�C�v�B�O�p�`
	graphicPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�ǂ̂悤�ɉ�ʂɏ������ނ̂��̐ݒ�
	graphicPipelineStateDesc.SampleDesc.Count = 1;
	graphicPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;


	//DepthStencil
	graphicPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//���ۂɐ���
	dxCommon_->hr = device->CreateGraphicsPipelineState(&graphicPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipeLineState));
	assert(SUCCEEDED(dxCommon_->hr));


}
