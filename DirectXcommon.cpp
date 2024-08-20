#include "DirectXcommon.h"


#include<cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXcommon::Initialize()
{
	Device_Initialize();
}

void DirectXcommon::Update()
{
}

void DirectXcommon::Device_Initialize()
{


#ifdef _DEBUG

	Microsoft::WRL::ComPtr <ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//�f�o�b�N���C����L��������
		debugController->EnableDebugLayer();
		//�����GPU���ł��`�F�b�N���s����悤�ɂ���
		debugController->SetEnableGPUBasedValidation(true);

	}

#endif



#pragma region DXGIFactory
	/*DXGIFactory�̍쐬*/
	//DXGI�t�@�N�g���[�̐ݒu
	Microsoft::WRL::ComPtr <IDXGIFactory7> dxgiFactory = nullptr;

	//HRESULT��Window�n�̃G���[�R�[�h�ł���A
	//�֐��������������ǂ���SUCCEEDED�}�N���Ŕ��f�o����
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	//�������̍��{�I�ȕ����ŃG���[���o���ꍇ�̓v���O�������Ԉ���Ă��邩�A
	//�ǂ��ɂ��ł��Ȃ��ꍇ�������̂�assert�ɂ��Ă���

	assert(SUCCEEDED(hr));
#pragma endregion

#pragma region adapter
	//�g�p����A�_�v�^�p�̕ϐ�
	Microsoft::WRL::ComPtr <IDXGIAdapter4> useAdapter = nullptr;
	//��Ԃ������т̃A�_�v�^�𗊂�
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {
		//�A�_�v�^�̏��擾
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		//�\�t�g�E�F�A�A�_�v�^�o�Ȃ��Ȃ�΍̗p
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�̗p�����A�_�v�^�̏������O�ɏo��
			Log(ConvertString(std::format(L"USE Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		//�\�t�g�E�F�A�A�_�v�^�Ȃ�Ό��Ȃ��������Ƃɂ���
		useAdapter = nullptr;
	}
	//�K�؂ȃA�_�v�^��������Ȃ������̂ŋN���ł��Ȃ�
	assert(useAdapter != nullptr);

#pragma endregion

#pragma region D3D12Device

	Microsoft::WRL::ComPtr <ID3D12Device> device = nullptr;
	//�@�\���x���ƃ��O�o�͗p�̕�����
	D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//�������̗p�ł��邩�����Ă���
	for (size_t i = 0; i < _countof(featureLevels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device));
		//�w�肵���@�\���x���Ńf�o�C�X�������ł������m�F
		if (SUCCEEDED(hr)) {
			//�����ł����̂Ń��o�͂����ă��[�v�𔲂���
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}

	}
	//�f�o�C�X�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(device != nullptr);
	//�����������̃��O���o��
	Log("Complete create D3D12Device!!!\n");

#pragma endregion 

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//��΂��G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//�G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//�x�����Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);


		D3D12_MESSAGE_ID denyIds[] = {
			//window11�ł�DXG�f�o�b�O���C���̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
			//https://stackoverflow.com/questions/69805245/directx-12-application-crashing-window-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//�}�����郌�x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//�w�肵�����b�Z�[�W�̕\����}������
		infoQueue->PushStorageFilter(&filter);
		//���
		infoQueue->Release();

	}
#endif


}
