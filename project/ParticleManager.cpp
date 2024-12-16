#include "ParticleManager.h"

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	//引数で受け取ってメンバ変数に記録する
	this->dxCommon = dxCommon;
	this->srvManager = srvManager;

	//ランダムエンジンの初期化
	randomEngine = std::mt19937(seedGenerator());
	distribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);

	//PSO生成



}
