#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "MapChipField.h"

namespace {

	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},
		{"1", MapChipType::kBlock},
		{"2", MapChipType::Player},
	};

}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2 / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;

	return rect;
}

void MapChipField::ResetMapChipData() {

	// �}�b�v�`�b�v�f�[�^�����Z�b�g
	mapChipData_.data.clear();

	mapChipData_.data.resize(kNumBlockVirtical);

	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// �}�b�v�`�b�v�f�[�^�����Z�b�g
	ResetMapChipData();

	// �t�@�C�����J��
	std::ifstream file;
	file.open(filePath);

	assert(file.is_open());

	//  �}�b�v�`�b�vCSV
	std::stringstream mapChipCsv;
	// �t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	mapChipCsv << file.rdbuf();
	// �t�@�C�������
	file.close();

	// CSV����}�b�v�`�b�v�f�[�^��ǂݍ���
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}


Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

void MapChipField::SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType) {
	// �C���f�b�N�X�͈̔̓`�F�b�N
	if (xIndex >= 0 && xIndex < kNumBlockHorizontal && yIndex >= 0 && yIndex < kNumBlockVirtical) {
		// �}�b�v�`�b�v�̎�ނ�ݒ�
		mapChipData_.data[yIndex][xIndex] = mapChipType;
	}
}