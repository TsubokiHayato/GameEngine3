#pragma once
#include <cstdint>
#include <vector>
#include <cmath>
#include "Vector3.h"
#include "string"



enum class MapChipType {
	kBlank, // ��
	kBlock, // �u���b�N
	Player,//�v���C���[
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;
		float right;
		float top;
		float bottom;
	};

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	void SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType);
private:

	// 1�u���b�N�̃T�C�Y
	static inline const float kBlockWidth = 1.0f; // 2
	static inline const float kBlockHeight = 1.0f; // 2
	// �u���b�N�̌�
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

};