#include "TextureManager.h"
TextureManager* TextureManager::instance = nullptr;

void TextureManager::Initialize()
{
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	//�e�N�X�`���t�@�C����ǂ�Ńv���O�����ň�����悤�ɂ���
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// �~�b�v�}�b�v�̍쐬
	DirectX::ScratchImage mipImages;
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//�e�N�X�`���f�[�^��ǉ�
	textureDatas.resize(textureDatas.size() + 1);
	//�ǉ������e�N�X�`���f�[�^�̎Q�Ƃ��擾����
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = DirectXCommon::(textureData.metadata);
}

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return nullptr;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}
