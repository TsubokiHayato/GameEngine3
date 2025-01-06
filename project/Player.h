#pragma once
#include <vector>
#include"Object3d.h"
#include "Model.h"
#include "MapChipField.h"

class Player
{
public:

	struct CollisionMapInfo {
		bool hitCeilingFlag = false;
		bool landingFlag = false;
		bool wallContactFlag = false;
		Vector3 movement = {};
	};


	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

public:

	void Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map);
	void Update();
	void Draw();

	void Move();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	Vector3 GetPosition() {return object3d->GetPosition(); }
	void SetCamera(Camera* camera) { object3d->SetCamera(camera); }

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CeilingContact(const CollisionMapInfo& info);
	void GroundSetting(const CollisionMapInfo& info);
	void JudgmentMove(const CollisionMapInfo& info);



	Vector3 CornerPosition(const Vector3& center, Corner corner);

	bool GetIsAlive() { return isAlive; }
	bool GetIsGoal() { return isGoal; }
	void SetIsGoal(bool isGoal) { this->isGoal = isGoal; }



private:

	//オブジェクト3D共通部分
	Object3dCommon* object3dCommon = nullptr;
	//DirectX共通部分
	DirectXCommon* dxCommon = nullptr;
	//ウィンドウズアプリケーション
	WinApp* winApp = nullptr;

	
	

	Object3d* object3d;
	Model* model_ = nullptr;
	MapChipField* mapChipField_ = nullptr;

	//モデルディレクトリパス
	const std::string modelDirectoryPath = "Resources";
	//モデルファイルパス2
	const std::string modelFileNamePath = "barrier.obj";

	Vector3 modelPosition = { 0.0f,0.0f,0.0f };

	Vector3 modelVector = { 0.1f,0.0f,0.0f };
	//モデルの回転
	Vector3 modelRotation = { 0.0f,0.0f,0.0f };
	//モデルの拡大
	Vector3 modelScale = { 1.0f,1.0f,1.0f };

	bool isAlive = true;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	bool onGround_ = true;
	Vector3 velocity_ = { 0.05f,0.0f,0.0f };
	static inline const float kJumpAcceleration = 0.5f;
	static inline const float kAcceleration = 0.2f;
	static inline const float kGravityAcceleration = 0.07f;
	static inline const float kBlank = 18.0f;
	static inline const float kLimitFallSpeed = 1.0f;


	bool isGoal = false;
};

