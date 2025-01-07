#include "Player.h"
#include"Input.h"
#undef min
#undef max
#include<algorithm>
#include<assert.h>
#include"imguiManager.h"
#include"MapChipField.h"

void Player::Initialize(Object3dCommon* Object3dCommon, ModelCommon* modelCommon, DirectXCommon* dxCommon, WinApp* winApp, MapChipField* map)
{
	object3dCommon = Object3dCommon;
	this->dxCommon = dxCommon;
	this->winApp = winApp;
	mapChipField_ = map;

	assert(map);
	mapChipField_ = map;

	isGoal = false;
	isAlive = true;
	onGround_ = false;
	//モデル
	model_ = new Model();
	model_->Initialize(modelCommon, modelDirectoryPath, modelFileNamePath);

	object3d = new Object3d();
	object3d->Initialize(object3dCommon, this->winApp, this->dxCommon);
	object3d->SetModel(modelFileNamePath);

	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); ++y)
	{
		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x)
		{
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::Player)
			{

				modelPosition = mapChipField_->GetMapChipPositionByIndex(x, y);


			}
		}
	}
	
}
void Player::Update()
{
	if (isAlive) {
		object3d->Update();
		
		Move();
		// 衝突情報を初期化
		CollisionMapInfo collisionMapInfo;
		// 移動量に速度の値をコピー
		collisionMapInfo.movement = velocity_;
		collisionMapInfo.landingFlag = false;
		collisionMapInfo.wallContactFlag = false;
		// マップ衝突チェック
		CheckMapCollision(collisionMapInfo);

		JudgmentMove(collisionMapInfo);

		CeilingContact(collisionMapInfo);

		GroundSetting(collisionMapInfo);

		if (modelPosition.y < 3.0f) {
			if (modelPosition.y < 3.0f) {
				
				
				isAlive = false;
			}
				
		}

	}else {
		for (uint32_t y = 0; y < mapChipField_->GetNumBlockVirtical(); ++y)
		{
			for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x)
			{
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::Player)
				{
					modelPosition = mapChipField_->GetMapChipPositionByIndex(x, y);
				}
			}
		}
		
		modelRotation = Vector3(0, 0, 0);
		modelScale = Vector3(1, 1, 1);
		
		isGoal = false;
		isAlive = true;
		onGround_ = false;

	}

	object3d->SetPosition(modelPosition);
	object3d->SetRotation(modelRotation);
	object3d->SetScale(modelScale);

	object3d->Update();

}


void Player::Draw()
{
	object3d->Draw();
}
void Player::Move()
{
	
	// 移動入力
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		

			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}
	}
	else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info)
{

	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionLeft(info);
	CheckMapCollisionRight(info);
}


void Player::CeilingContact(const CollisionMapInfo& info) {
	// 天井、当り判定
	if (info.hitCeilingFlag) {
		velocity_.y = 0;
	}
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info)
{


	if (info.movement.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(modelPosition + info.movement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal && mapChipTypeNext != MapChipType::kGoal) {
		isGoal = true;
	}


	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {

		hit = true;
	}
	if (mapChipType == MapChipType::kGoal && mapChipTypeNext != MapChipType::kGoal) {
		isGoal = true;
	}

	// ブロックにヒット
	if (hit) {

		MapChipField::IndexSet indexSetNow;

		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(modelPosition + Vector3(0, +kHeight / 3.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex) {

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(modelPosition + info.movement + Vector3(0, +kHeight / 3.0f, 0));

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.movement.y = std::max(0.0f, rect.bottom - modelPosition.y - (kHeight / 3.0f + kBlank));
			info.hitCeilingFlag = true;
		}
	}

}

void Player::CheckMapCollisionDown(CollisionMapInfo& info)
{
	if (info.movement.y >= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(modelPosition + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal && mapChipTypeNext != MapChipType::kGoal) {
		isGoal = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal && mapChipTypeNext != MapChipType::kGoal) {
		isGoal = true;
	}

	if (hit) {
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(modelPosition + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(modelPosition + info.movement + Vector3(0, -kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.movement.y = std::min(0.0f, (rect.top - modelPosition.y) + ((kHeight / 2.0f) + kBlank));
			// 地面に当たったことを記録する
			info.landingFlag = true;
		}
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info)
{

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(modelPosition + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}


	MapChipType mapChipType;
	bool hit = false;

	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal) {
		isGoal = true;
	}
	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal) {
		isGoal = true;
	}

	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.x = std::max(0.0f, (rect.right - modelPosition.x) - (kWidth / 2.0f + kBlank));
	}

}


void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(modelPosition + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;

	// 右下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal ) {
		isGoal = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (mapChipType == MapChipType::kGoal) {
		isGoal = true;
	}

	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement.x = std::min(0.0f, (rect.left -modelPosition.x) + (kWidth / 2.0f + kBlank));
	}

}


void Player::GroundSetting(const CollisionMapInfo& info) {

	// 接地状態の切り替え処理
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態の移行
			onGround_ = false;
		}
		else {
			std::array<Vector3, kNumCorner> positonsNew;

			for (uint32_t i = 0; i < positonsNew.size(); ++i) {
				positonsNew[i] = CornerPosition(modelPosition + info.movement, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			bool hit = false;

			// 左下点の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positonsNew[kLeftBottom] + Vector3(0, 0, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (mapChipType == MapChipType::kGoal) {
				isGoal = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positonsNew[kRightBottom] + Vector3(0, 0, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (mapChipType == MapChipType::kGoal) {
				isGoal = true;
			}
			// 落下開始
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	}
	else {
		// 着地
		if (info.landingFlag) {
			
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 着地状態に切り替える
			onGround_ = true;
		}
	}
}


void Player::JudgmentMove(const CollisionMapInfo& info)
{
	// 移動
	modelPosition += info.movement;
}



Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0},
		{-kWidth / 2.0f, -kHeight / 2.0f, 0},
		{+kWidth / 2.0f, +kHeight / 2.0f, 0},
		{-kWidth / 2.0f, +kHeight / 2.0f, 0}
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
