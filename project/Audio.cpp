#include "Audio.h"
#include <fstream>
#include<cassert>
void Audio::Initialize()
{
	//DirectX�̏����������̖����ɒǉ�
	//XAudio2�̏�����
	HRESULT result = XAudio2Create(&xAudio2,0,XAUDIO2_DEFAULT_PROCESSOR);

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


	//�B�t�@�C���N���[�Y


	//�C�ǂݍ��񂾉����f�[�^��Ԃ�
}
