#pragma once
#include<xaudio2.h>
#include <wrl/client.h>
#include <cstdint>
#pragma comment(lib,"xaudio2.lib")
using Microsoft::WRL::ComPtr;

//�`�����N�w�b�_�[
struct ChunkHeader
{
	char id[4];//�`�����NID
	int32_t size;//�`�����N�T�C�Y
};

//RIFF�`�����N�w�b�_�[
struct RiffChunkHeader
{
	ChunkHeader chunk;//�`�����N�w�b�_�[
	char type[4];//\WAVE
};

//FMT�`�����N�w�b�_�[
struct FormatChunk
{

	ChunkHeader chunk;//"fmt "�`�����N�w�b�_�[
	WAVEFORMATEX format;//�g�`�t�H�[�}�b�g

};

//�����f�[�^
struct SoundData {
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;//WaveFormatex
	//�o�b�t�@�̂��擪�A�h���X
	BYTE* pBuffer;
	//�o�b�t�@�̃T�C�Y
	unsigned int bufferSize;
};

class Audio
{

public:

	void Initialize();
	SoundData SoundLoadWave(const char* fileName);

private:

	
	
	ComPtr<IXAudio2> xAudio2;
	
	IXAudio2MasteringVoice* masterVoice;
};
