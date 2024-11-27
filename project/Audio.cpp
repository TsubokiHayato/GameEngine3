#include "Audio.h"
#include <fstream>
#include<cassert>
void Audio::Initialize()
{
	//DirectX�̏����������̖����ɒǉ�
	//XAudio2�̏�����
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);

}

SoundData Audio::SoundLoadWave(const char* fileName)
{
	//�t�@�C������nullptr�łȂ����Ƃ��m�F
	assert(fileName != nullptr);

	HRESULT result;

	/*-------------
	�@�t�@�C�����J��
	-------------*/

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fileName, std::ios_base::binary);
	//�t�@�C�����J���Ȃ������ꍇ
	assert(file.is_open() && "�t�@�C�����J���܂���ł���");

	/*-----------------
	�A.wav�f�[�^�̓ǂݍ���
	------------------*/

	//RIFF�`�����N�w�b�_�[�̓ǂݍ���
	RiffChunkHeader riffChunkHeader = {};
	file.read((char*)&riffChunkHeader, sizeof(riffChunkHeader));
	//�t�@�C����RIFF�`���łȂ��ꍇ
	if (strncmp(riffChunkHeader.chunk.id, "RIFF", 4) != 0) {
		assert(0 && "RIFF�`���ł͂���܂���");
	}
	//�t�@�C����WAVE�`���łȂ��ꍇ
	if (strncmp(riffChunkHeader.type, "WAVE", 4) != 0) {
		assert(0 && "WAVE�`���ł͂���܂���");
	}
	//�t�H�[�}�b�g�`�����N�̓ǂݍ���
	FormatChunk formatChunk = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&formatChunk, sizeof(formatChunk));
	if (strncmp(formatChunk.chunk.id, "fmt ", 4) != 0) {
		assert(0 && "fmt�`���ł͂���܂���");
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(formatChunk.chunk.size == sizeof(formatChunk.format));
	file.read((char*)&formatChunk.format, formatChunk.chunk.size);

	//�f�[�^�`�����N�̓ǂݍ���
	ChunkHeader dataChunkHeader = {};
	file.read((char*)&dataChunkHeader, sizeof(dataChunkHeader));
	if (strncmp(dataChunkHeader.id, "data", 4) != 0) {
		assert(0 && "data�`���ł͂���܂���");
	}
	if (strncmp(dataChunkHeader.id, "data", 4) == 0) {
		//�ǂݎ��ʒu��Junk�`�����N�̏I���܂Ői�߂�
		file.seekg(dataChunkHeader.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&dataChunkHeader, sizeof(dataChunkHeader));
	}


	if (strncmp(dataChunkHeader.id, "data", 4) != 0) {
		assert(0 && "data�`���ł͂���܂���");
	}

	//Data�`�����N�̃f�[�^(�g�`�f�[�^)��ǂݍ���
	char* pBuffer = new char[dataChunkHeader.size];
	file.read(pBuffer, dataChunkHeader.size);

	//�B�t�@�C���N���[�Y
	file.close();

	//�C�ǂݍ��񂾉����f�[�^��Ԃ�

	//return����ׂ̉����f�[�^
	SoundData soundData = {};

	//WAVEFORMATEX�̐ݒ�
	soundData.wfex = formatChunk.format;
	//�g�`�f�[�^�̐ݒ�
	soundData.pBuffer = (BYTE*)pBuffer;
	//�g�`�f�[�^�̃T�C�Y�̐ݒ�
	soundData.bufferSize = dataChunkHeader.size;

	return soundData;
}

void Audio::SoundUnload(SoundData* soundData)
{
	//�o�b�t�@���������J��
	delete[] soundData->pBuffer;

	soundData->pBuffer = nullptr;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{

	HRESULT result;

	//�g�`�t�H�[�}�b�g����SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice=nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = soundData.pBuffer;
	buffer.AudioBytes = soundData.bufferSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	//�Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buffer);
	result = pSourceVoice->Start(0);
}
