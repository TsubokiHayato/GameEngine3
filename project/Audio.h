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
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	///�����f�[�^�̓ǂݍ���
	/// </summary>
	SoundData SoundLoadWave(const char* fileName);
	/// <summary>
	///	�I������
	/// </summary>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// �T�E���h�̍Đ�
	/// </summary>
	/// <param name="xAudio2"></param>
	/// <param name="soundData"></param>
	void SoundPlayWave(IXAudio2* xAudio2,const SoundData& soundData);
private:

	
	
	ComPtr<IXAudio2> xAudio2;
	
	IXAudio2MasteringVoice* masterVoice;
};
