
#include<string>
#include<format>
#include<cassert>

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include<dxgidebug.h>
#pragma comment(lib,"dxguid.lib")

#include<dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

#include"MT_Matrix.h"

#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"


#include "externals/DirectXTex/d3dx12.h"
#include "externals/DirectXTex/DirectXTex.h"
#include"vector"


#include <fstream>
#include<sstream>

#include<wrl.h>


#include "Input.h"
#include"WinApp.h"
#include"DirectXcommon.h"


# define PI 3.14159265359f

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


struct Vector2
{
	float x;
	float y;

};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct VertexData {
	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct  TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

struct D3DResourceLeakChecker
{
#pragma region ReportLiveObjects
	~D3DResourceLeakChecker() {
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			debug->Release();
		}
	}
#pragma endregion

};



//log�쐬
void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());

}

std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}
std::wstring ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

//DescriptorHeap�̂����������񂷂�
Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> CreateDescriptorHeap(
	Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {

	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}



//Resource�쐬�̊֐�
Microsoft::WRL::ComPtr <ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr <ID3D12Device> device, size_t sizeInBytes) {


	Microsoft::WRL::ComPtr <IDXGIFactory7>dxgiFactory = nullptr;

	//HRESULT��Window�n�̃G���[�R�[�h�ł���A
	//�֐��������������ǂ���SUCCEEDED�}�N���Ŕ��f�o����
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));


	//���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC ResourceDesc{};


	//�o�b�t�@���\�[�X�B�e�N�X�`���̏ꍇ�ʂ̐ݒ������
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = (sizeInBytes);//���\�[�X�̃t�@�C���B

	//�o�b�t�@�̏ꍇ�͂�����1�ɂ��錈�܂�
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	//�o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���ۂɃ��\�[�X�����
	Microsoft::WRL::ComPtr <ID3D12Resource> Resource = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;

}

//�R���p�C���[�V�F�[�_�[�֐�	
IDxcBlob* CompileShader(
	//Compiler����Shader�t�@�C���ւ̃p�X
	const std::wstring& filePath,
	//Compiler�Ɏg�p����Profile
	const wchar_t* profile,
	//�������Ő����������̂��R��
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {


	/*---------------
	1 msl�t�@�C����ǂ�
	---------------*/


	//���ꂩ��V�F�[�_���R���p�C������|�����O�ɏo��
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));

	//hlsl�t�@�C����ǂݍ���
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

	//�ǂ߂Ȃ�������~�߂�
	assert(SUCCEEDED(hr));

	//�ǂݍ��񂾃t�@�C���̓��e��ݒ肷��
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF-8�����R�[�h�ł��邱�Ƃ�ʒm

	/*-----------
	2 Compile����
	-----------*/

	LPCWSTR arguments[] = {
		filePath.c_str(),//�ǂݍ��񂾃t�@�C��
		L"-E",L"main",//�G���g���[�|�C���g�̎w��
		L"-T",profile,//shaderProfile�̐ݒ�
		L"-Zi",L"-Qembed_debug",//�f�o�b�O�p�̏��𖄂ߍ���
		L"-Od",//�œK�����O���Ă���
		L"-Zpr",//���������C�A�E�g�͍s�D��
	};

	//���ۂ�Shader���R���p�C����ݒ肷��
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)
	);

	assert(SUCCEEDED(hr));


	/*--------------------------
	3 �x���E�G���[���łĂȂ����m�F����
	--------------------------*/

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	/*--------------------------
	�@4 Compile���ʂ��󂯎���ĕԂ�
	--------------------------*/

	//�R���p�C�����ʂ�����s�p�̃o�C�i���������擾
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

	//�����������O���o��
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));

	//�����g��Ȃ����\�[�X�����
	shaderSource->Release();
	shaderResult->Release();
	//���s�p�̃o�C�i�������
	return shaderBlob;

}


//LoadTexture�֐�
DirectX::ScratchImage LoadTexture(const std::string& filePath) {

	//�e�N�X�`���t�@�C����ǂ�Ńv���O�����ň�����悤�ɂ���
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// �~�b�v�}�b�v�̍쐬
	DirectX::ScratchImage mipImages;
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//�~�b�v�}�b�v�t���̃f�[�^��Ԃ�
	return mipImages;
}

Microsoft::WRL::ComPtr <ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, const DirectX::TexMetadata& metadata) {

	/*------------------------
	metaData�����Resource�̐ݒ�
	------------------------*/

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);//Texture�̕�
	resourceDesc.Height = UINT(metadata.height);//Texture�̍���
	resourceDesc.MipLevels = UINT(metadata.mipLevels);//mipmap�̐�
	resourceDesc.DepthOrArraySize = UINT(metadata.arraySize);//���s���@or �z��Texture�̔z��
	resourceDesc.Format = metadata.format;//Texture��Format
	resourceDesc.SampleDesc.Count = 1;//1�Œ�B�T���v�����O�J�E���g
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//Texture�̎������B���i�g���Ă���̂�2����


	/*--------------
	���p����Heap�̐ݒ�
	--------------*/

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;//�ׂ����ݒ�
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBack�|���V�[��CPU�A�N�Z�X�\
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�v���Z�b�T�̋߂��ɔz�u

	/*----------
	Resource�̐���
	-----------*/

	Microsoft::WRL::ComPtr <ID3D12Resource> resource = nullptr;

	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heap�̐ݒ�
		D3D12_HEAP_FLAG_NONE,//Heap�̓���Ȑݒ�B���ɂȂ�
		&resourceDesc,//Resource�̐ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,//�����ResourceState�BTexture�͊�{�ǂނ���
		nullptr,//Clear�œK�l�B�g��Ȃ��̂�nullptr
		IID_PPV_ARGS(&resource));//�쐬����Resorce�|�C���^�ւ̃|�C���^

	assert(SUCCEEDED(hr));

	return resource;

}

void UploadTextureData(Microsoft::WRL::ComPtr <ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {

	//Meta�����擾
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	//�SMipMap�ɂ���
	for (size_t mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel) {

		//MipLevel���w�肵��Image���擾
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Texture�ɓ]��
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,//�S�̈�փR�s
			img->pixels,//���f�[�^�A�h���X
			UINT(img->rowPitch),//1���C���T�C�Y
			UINT(img->slicePitch)//1���T�C�Y
		);
		assert(SUCCEEDED(hr));
	}
}


Microsoft::WRL::ComPtr <ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, int32_t width, int32_t height) {

	//��������Resource�̐ݒ�
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;//Texture�̕�
	resourceDesc.Height = height;//Texture�̍���
	resourceDesc.MipLevels = 1;//mipmap�̐�
	resourceDesc.DepthOrArraySize = 1;//���s��or�z��Texture�̔z��
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DepthStencil�Ƃ��ė��p�\�ȃt�H�[�}�b�g
	resourceDesc.SampleDesc.Count = 1;//�T���v�����O�J�E���g�B1�Œ�
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//�񎟌�
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //DepthStencil�Ƃ��Ďg���ʒm

	//���p����Heap�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//VRAM��ɍ��

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//�t�H�[�}�b�g�BResource�ƍ��킹��

	//Resource�̐���
	Microsoft::WRL::ComPtr <ID3D12Resource> resouce = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&resouce));
	assert(SUCCEEDED(hr));

	return resouce;

}

//CPU��DescriptorHandle���擾����֐�
D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

//GPU��DescriptorHandle���擾����֐�
D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}




MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filePath) {

	/*------------------------
	1 : ���ŕK�v�ɂȂ�ϐ��̐錾
	------------------------*/

	MaterialData materialData;
	std::string line;


	/*------------------------
	2 : �t�@�C�����J��
	------------------------*/

	std::ifstream file(directoryPath + "/" + filePath);
	assert(file.is_open());

	/*---------------------------------------------
	3 : ���ۂɃt�@�C����ǂ݁AMaterialData���\�z���Ă���
	---------------------------------------------*/

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}


	/*------------------------
	4 : MaterialData��Ԃ�
	------------------------*/

	return materialData;

}



//�I�u�W�F�N�g�ǂݍ��ފ֐�

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	/*-------------
	1 : OBJ�t�@�C��
	--------------*/
	ModelData modelData;//�\�z����
	std::vector<Vector4> positions;//�ʒu
	std::vector<Vector3> normals;//�@��
	std::vector<Vector2> texcoords;//�e�N�X�`�����W
	std::string line;//�t�@�C������ǂ�1�s���i�[�������


	/*----------------------
	2 : OBJ�t�@�C����ǂݍ���
	----------------------*/
	std::ifstream file(directoryPath + "/" + filename); //�t�@�C����ǂݍ���
	assert(file.is_open());//�J���Ȃ�������~�߂�



	/*-----------------------------
	3 : �t�@�C����ǂ݁AModelData���\�z
	-------------------------------*/

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//�擪�̎��ʎq��ǂ�


		//identifier�ɉ���������


		/*------------------
			���_����ǂ�
		------------------*/
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}

		/*---------------
			�O�p�`�����
		----------------*/
		else if (identifier == "f") {


			VertexData triangle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				position.x *= -1.0f;
				normal.x *= -1.0f;
				texcoord.y = 1.0f - texcoord.y;


				/*VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);*/

				triangle[faceVertex] = { position,texcoord,normal };
			}

			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibrary�̖��O���擾����
			std::string materialFilename;
			s >> materialFilename;
			//��{�I��obj�t�@�C���Ɠ���K�w��mtl�͑��݂�����̂ŁA�f�B���N�g�����ƃt�@�C������n��
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}


	return modelData;

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = nullptr;
	winApp = new WinApp();

	winApp->Initialize();

	D3DResourceLeakChecker leakChecker;

#pragma endregion

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


#pragma region commandQueue
	//�R�}���h�L���[�𐶐�����
	Microsoft::WRL::ComPtr <ID3D12CommandQueue> commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{ };
	hr = device->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue));
	//�R�}���h�L���[�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));
#pragma endregion

#pragma region commandList
	//�R�}���h�A���P�[�^�𐶐�����
	Microsoft::WRL::ComPtr <ID3D12CommandAllocator> commandAllocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	//�R�}���h�A���P�[�^�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));

	//�R�}���h���X�g��������
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> commandList = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));

	//�R�}���h���X�g�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));

#pragma endregion

#pragma region SwapChain

	//�X���b�v�`�F�[���𐶐�����
	Microsoft::WRL::ComPtr <IDXGISwapChain4> swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = winApp->kClientWidth;//��ʂ̕��B�E�B���h�E�̂��N���C�A���g�̈�𓯂����̂ɂ��Ă���
	swapChainDesc.Height = winApp->kClientHeight;//��ʂ̍����B�E�B���h�E�̂��N���C�A���g�̈�𓯂����̂ɂ��Ă���
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F�̌`��
	swapChainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//�`��̃^�[�Q�b�g�Ƃ��ė��p����
	swapChainDesc.BufferCount = 2;//�_�u���o�b�t�@
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//���j�^�ɂ�������A���g��j��

	//�R�}���h�L���[�A�E�B���h�E�n���h���A�����Ă���n���Đ�������
	hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), winApp->GetHWND(), &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));



#pragma endregion

#pragma region DescriptorHeap

	//RTV�f�B�X�N�C���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

	//SRV�f�B�X�N�C���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	//DSV�f�B�X�N�C���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
#pragma endregion


#pragma region DescriptorSize


	const uint32_t descriptorSizeSRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t descriptorSizeRTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

#pragma endregion

#pragma region SwapChange����Resource�����������Ă���

	Microsoft::WRL::ComPtr <ID3D12Resource> swapChainResources[2] = { nullptr };
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));

	//���܂��擾�ł��Ȃ���΋N���ł��Ȃ�
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));

#pragma endregion


#pragma region RTV
	//RTV�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//�o�͌��ʂ�SRGB�ɕϊ����ď�������
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2d�e�N�X�`���Ƃ��ď�������

	//�f�B�X�N���v�^�̐擪���擾����
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = GetCPUDescriptorHandle(rtvDescriptorHeap, descriptorSizeRTV, 0);

	//RTV��2���̂Ńf�B�X�N���v�^��2�p��
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	//�܂���ڂ͍ŏ��̏��ɂ���B���ꏊ��������Ŏw�肵�Ă�����K�v������
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0].Get(), &rtvDesc, rtvHandles[0]);

	//2�ڂ̃f�B�X�N���v�^�n���h��������(���͂�)
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2�ڂ����
	device->CreateRenderTargetView(swapChainResources[1].Get(), &rtvDesc, rtvHandles[1]);

#pragma endregion



#pragma region DSV
	Microsoft::WRL::ComPtr <ID3D12Resource> depthStencilResource = CreateDepthStencilTextureResource(device, winApp->kClientWidth, winApp->kClientHeight);

	//DSV�̐ݒ�

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//
	device->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, GetCPUDescriptorHandle(dsvDescriptorHeap, descriptorSizeDSV, 0));


#pragma endregion




#pragma region Fence&Event
	//
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//
	HANDLE fenceEvent = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent != nullptr);

#pragma endregion



#pragma region DXC_Initialization//
	//dxcCompiler��������
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));


	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));


#pragma endregion

#pragma region PSO(Pipeline_State_Object)//
	/*------------
	RootSignature
	------------*/

	//RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange�쐬
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//0����n�܂�
	descriptorRange[0].NumDescriptors = 1;//����1
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRV������
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offset�������v�Z


	//RootParameter�쐬�B02_01�ǉ�//02_03�X�V
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBV���g��
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	rootParameters[0].Descriptor.ShaderRegister = 0;//���W�X�^�ԍ�0�ƃo�C���h

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBV���g��
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShader�ł���
	rootParameters[1].Descriptor.ShaderRegister = 0;//���W�X�^�ԍ�0�ƃo�C���h

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DESCRIPTOR_TABLE���g��
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBV���g��
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShader�ł���
	rootParameters[3].Descriptor.ShaderRegister = 1;//���W�X�^�ԍ�1�ƃo�C���h

	descriptionRootSignature.pParameters = rootParameters;//���[�g�p�����[�^�z��ւ̃|�C���^
	descriptionRootSignature.NumParameters = _countof(rootParameters);//�z��̒���


#pragma region Sampler_03_00//

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
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



	//�V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr <ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//�o�C�i�������ɍ쐬
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	/*------------
	  InputLayOut
	------------*/
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
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


	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/*------------
	  BlendState
	------------*/



	D3D12_BLEND_DESC blendDesc{};
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

	//RasterizerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//�O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/*-------------------
	  Vertex&Pixel_Shader
	-------------------*/

	//Shader���R���p�C������
	Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob = CompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob = CompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);


	/*---------------
	DepthStencilDesc�̐ݒ�
	-------------------*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depth�̋@�\��L����
	depthStencilDesc.DepthEnable = true;
	//�������݂��܂�
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//��r�֐���LessEqual
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	/*------------------
	 �@ PSO�𐶐�����
	------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicPipelineStateDesc{};

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
	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipeLineState = nullptr;
	hr = device->CreateGraphicsPipelineState(&graphicPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipeLineState));
	assert(SUCCEEDED(hr));

#pragma endregion


#pragma region Material_Resource_02_01
	//�}�e���A���p�̃��\�[�X�����B�����Color1���̃T�C�Y��p�ӂ���
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource =
		CreateBufferResource(device, sizeof(Material));
	//�}�e���A���Ƀf�[�^����������
	Material* materialData = nullptr;
	//�������ނ��߂̃A�h���X���擾
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//����͐Ԃ���������ł݂�
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();

#pragma endregion


#pragma region Material_Resource_Sprite
	//�}�e���A���p�̃��\�[�X�����B�����Color1���̃T�C�Y��p�ӂ���
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResourceSprite =
		CreateBufferResource(device, sizeof(Material));
	//�}�e���A���Ƀf�[�^����������
	Material* materialDataSprite = nullptr;
	//�������ނ��߂̃A�h���X���擾
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	//����͐Ԃ���������ł݂�
	materialDataSprite->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialDataSprite->enableLighting = false;
	materialDataSprite->uvTransform = MakeIdentity4x4();

#pragma endregion

	Transform uvTransFormSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};


#pragma region DirectionalLightData
	//���s�����p�p�̃��\�[�X�����B�����Color1���̃T�C�Y��p�ӂ���
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource =
		CreateBufferResource(device, sizeof(DirectionalLight));
	//���s�����p�Ƀf�[�^����������
	DirectionalLight* directionalLightData = nullptr;
	//�������ނ��߂̃A�h���X���擾
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.5f,-0.5f,0.0f };
	directionalLightData->intensity = 1.0f;



#pragma endregion



#pragma region TransformMatrixResource_02_02

	//WVP�p�̃��\�[�X�����
	Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource =
		CreateBufferResource(device, sizeof(TransformationMatrix));
	//�f�[�^����������
	TransformationMatrix* wvpData = nullptr;
	//�������ނ��߂̃A�h���X���擾
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//�P�ʍs�����������ł���
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();


#pragma endregion






#pragma region SpriteResource

	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResourceSprite = CreateBufferResource(device, sizeof(VertexData) * 6);

	//���_�o�b�t�@�r���[���쐬����
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	//���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	//�g�p���郊�\�[�X�̃T�C�Y�͒��_6���̃T�C�Y
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	//1���_������̃T�C�Y
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);

	VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	//�O�p�`��2�g���āA�l�p�`�̍��
	/*---------------
		B-------D		1���� : ABC�̎O�p�`
		|		|		2���� : BDC�̎O�p�`
		|		|
		A-------C
	----------------*/
	//A
	vertexDataSprite[0].pos = { 0.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[0].normal = { 0.0f,0.0f,1.0f };

	//B

	vertexDataSprite[1].pos = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[1].normal = { 0.0f,0.0f,1.0f };

	//C

	vertexDataSprite[2].pos = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite[2].normal = { 0.0f,0.0f,1.0f };


	//D
	vertexDataSprite[3].pos = { 640.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { 1.0f,0.0f };
	vertexDataSprite[3].normal = { 0.0f,0.0f,1.0f };


	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResourceSprite = CreateBufferResource(device, sizeof(TransformationMatrix));
	//�f�[�^����������
	TransformationMatrix* transformationMatrixDataSprite = nullptr;

	//�������ނ��߂̃A�h���X���擾
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//�P�ʍs�����������ł���
	transformationMatrixDataSprite->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite->World = MakeIdentity4x4();

#pragma endregion


#pragma region indexResourceSprite

	//WVP�p�̃��\�[�X�����
	Microsoft::WRL::ComPtr <ID3D12Resource> indexResourceSprite =
		CreateBufferResource(device, sizeof(uint32_t) * 6);


	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};

	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();

	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

	indexDataSprite[0] = 0;    indexDataSprite[1] = 1;   indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;    indexDataSprite[4] = 3;   indexDataSprite[5] = 2;



#pragma endregion



#pragma region ModelData
	//���f����݂���
	ModelData modelData = LoadObjFile("resources", "axis.obj");
	//���_���\�[�X�����
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource = CreateBufferResource(device, sizeof(VertexData) * modelData.vertices.size());
	//���_�o�b�t�@�r���[���쐬����
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//���_���\�[�X�Ƀf�[�^����������
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
#pragma endregion


#pragma region ShaderResorceView


	//Texture��ǂ�œ]������
	DirectX::ScratchImage mipImages = LoadTexture("Resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr <ID3D12Resource> textureResource = CreateTextureResource(device, metadata);
	UploadTextureData(textureResource.Get(), mipImages);

	//2���ڂ�Texture��ǂ�œ]������
	DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr <ID3D12Resource> textureResource2 = CreateTextureResource(device, metadata2);
	UploadTextureData(textureResource2.Get(), mipImages2);

	//metaData�����SRV��	�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//metaData�����SRV��	�ݒ�(2����)
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);



	//SRV���쐬����DescriptorHeap�̏ꏊ�����߂�
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	//�擪��ImGui���g���Ă���̂ł��̎�������
	textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRV�̐���
	device->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);



	//SRV���쐬����DescriptorHeap�̏ꏊ�����߂�(2����)
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//�擪��ImGui���g���Ă���̂ł��̎�������(2����)
	textureSrvHandleCPU2.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU2.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRV�̐���(2����)
	device->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


#pragma endregion



#pragma region Viewport&Scissor//
	//�r���[�|�[�g
	D3D12_VIEWPORT viewport{};
	//�N���C�A���g�̈�̗̈�̃T�C�Y�ƈꏏ�ɂ��ĉ�ʑS�̂ɕ\��
	viewport.Width = (float)winApp->kClientWidth;
	viewport.Height = (float)winApp->kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//�V�U�[��`
	D3D12_RECT scissorRect{};
	//��{�I�Ƀr���[�|�[�g�Ɠ�����`���\�������悤�ɂ���
	scissorRect.left = 0;
	scissorRect.right = winApp->kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = winApp->kClientHeight;

#pragma endregion




#pragma region ImGui_Initialize

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHWND());
	ImGui_ImplDX12_Init(device.Get(),
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 0),
		GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 0));

#pragma endregion



	Input* input = nullptr;

	input = new Input();
	input->Initialize(winApp);




	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, float(winApp->kClientWidth) / float(winApp->kClientHeight), 0.1f, 100.0f);

	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };


	bool useMonsterBall = true;



	//�E�B���h�E�́~�{�^���񂪉������܂Ń��[�v
	while (true) {
		if (winApp->ProcessMessage()) {
			break;
		}
		//�E�B���h�E�Ƀ��b�Z�[�W�����Ă���ŗD��ŏ���������



		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		ImGui::Text("Material");
		ImGui::Checkbox("useMonsterBall", &useMonsterBall);
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f, -10.0f, 10.0f);


		ImGui::ColorEdit4("material.color", &materialData->color.x);
		ImGui::SliderFloat("intensity", &directionalLightData->intensity, 0.0f, 30.0f);
		ImGui::Text("Sprite");
		ImGui::DragFloat("UVTranslate", &uvTransFormSprite.translate.x, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("UVScale", &uvTransFormSprite.scale.x, 0.01f, -10.0f, 10.0f);

		ImGui::SliderAngle("UVRotate", &uvTransFormSprite.rotate.z);

		/*--------
		�Q�[���̏���
		---------*/

		input->Update();
		if (input->PushKey(DIK_A)) {
			cameraTransform.rotate.y += 0.01f;
		}
		if (input->PushKey(DIK_D)) {
			cameraTransform.rotate.y -= 0.01f;
		}
		if (input->PushKey(DIK_SPACE) && input->TriggerKey(DIK_SPACE)) {
			cameraTransform.rotate.x -= 0.01f;
		}

		//transform.rotate.y += 0.03f;
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, float(winApp->kClientWidth) / float(winApp->kClientHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

		wvpData->WVP = worldViewProjectionMatrix;

		wvpData->World = worldMatrix;


		Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransFormSprite.scale, uvTransFormSprite.rotate, uvTransFormSprite.translate);

		materialDataSprite->uvTransform = uvTransformMatrix;

		Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
		Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
		Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(winApp->kClientWidth), float(winApp->kClientHeight), 0.0f, 100.0f);
		Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
		transformationMatrixDataSprite->WVP = worldViewProjectionMatrixSprite;
		transformationMatrixDataSprite->World = worldMatrixSprite;






		//ImGui::ShowDemoWindow();
		ImGui::Render();




		/*-----
		�`�揈��
		------*/

		//���ꂩ�珑�����ރo�b�N�o�b�t�@�̃C���f�b�N�X���擾
		UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

#pragma region TransitionBarrier

		//TransitionBarrier�̐ݒ�
		D3D12_RESOURCE_BARRIER barrier{};
		//����̃o���A��Transition
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		//None�ɂ��Ă���
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		//�o���A�𒣂�Ώۂ̃��\�[�X�B���݂̃o�b�N�o�b�t�@�ɑ΂��čs��
		barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
		//�ΈڑO(����)��ResourceState
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		//�Έڌ��ResourceState
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		//TransitionBarrier�𒣂�
		commandList->ResourceBarrier(1, &barrier);


		//depth��������
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			GetCPUDescriptorHandle(dsvDescriptorHeap, descriptorSizeDSV, 0);
		commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);


		//�w�肵���F�ŉ�ʑS�̂��N���A����
		float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//RGB�̒l�B���ۂ��F
		commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

		//depthClear
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion

		//�`��p��Descriptor�̐ݒ�
		ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap.Get() };
		commandList->SetDescriptorHeaps(1, descriptorHeaps);


#pragma region Command//Triangle

		commandList->RSSetViewports(1, &viewport);//viewPort��ݒ�
		commandList->RSSetScissorRects(1, &scissorRect);//Scissor��ݒ�
		//RootSignature��ݒ�B
		commandList->SetGraphicsRootSignature(rootSignature.Get());
		commandList->SetPipelineState(graphicsPipeLineState.Get());//PSO��ݒ�
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView);//VBV��ݒ�
		//�`��ݒ�
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�}�e���A��CBuffer�̏ꏊ��ݒ�_02_01
		commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());


		//wvp�p��CBuffer�̏ꏊ��ݒ�_02_02
		commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

		//SRV��DescriptorTable�̐擪��ݒ�B2��rootParameter[2]�ł���B
		commandList->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);

		commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());


		//�`��

		commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);



		//Sprite�̕`��
		commandList->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
		commandList->IASetIndexBuffer(&indexBufferViewSprite);

		commandList->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());

		//TransformationMatrixCBuffer�̐ݒ�
		commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());

		commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
		//�`��
		commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

#pragma endregion

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

#pragma region TransitionBarrier
		//��ʂɏ������������ׂďI���A��ʂɉf���̂ŁA��Ԃ�Έ�
		// �����RenderTarget����Present�ɂ���
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		//TransitionBarrier�𒣂�
		commandList->ResourceBarrier(1, &barrier);


		//�R�}���h���X�g�̓��e���m�肳����B���ׂẴR�}���h��ς�ł���Close���邱��
		hr = commandList->Close();
		assert(SUCCEEDED(hr));

#pragma endregion

#pragma region commandKick

		//GPU�ɃR�}���h���X�g�̎��s�s�킹��
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		commandQueue->ExecuteCommandLists(1, commandLists);

		//GPU��OS�ɉ�ʂ̌������s���悤�ɂ���
		swapChain->Present(1, 0);
#pragma endregion
#pragma region GPU-signal

		//Fence�̒l���X�V
		fenceValue++;
		//GPU�������܂ł��ǂ蒅�������ɁAFence�̒l���w�肵���l�ɑ������悤��Signal��������
		commandQueue->Signal(fence.Get(), fenceValue);


		if (fence->GetCompletedValue() < fenceValue) {
			//
			fence->SetEventOnCompletion(fenceValue, fenceEvent);
			//�C�x���g�҂�
			WaitForSingleObject(fenceEvent, INFINITE);
		}

		//���̃t���[���p�̃R�}���h���X�g������
		hr = commandAllocator->Reset();
		assert(SUCCEEDED(hr));
		hr = commandList->Reset(commandAllocator.Get(), nullptr);
		assert(SUCCEEDED(hr));

#pragma endregion



	}

#pragma region AllRelease



	//���\�[�X���[�N�`�F�b�N
	delete input;

	winApp->Finalize();

	delete winApp;
	winApp = nullptr;

	CloseHandle(fenceEvent);









	//ImGui
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�x�����Ɏ~�܂�
	//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

#pragma endregion




	return 0;

}