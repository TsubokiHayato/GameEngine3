#pragma once
#include<random>

//前方宣言
class DirectXCommon;
class SrvManager;

class ParticleManager
{

private:
	static ParticleManager* instance;
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

public:
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ParticleManager* GetInstance() { return instance; }

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="srvManager">SRV管理クラス</param>
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	void Update();

	void Draw();

	void Finalize();

private:
	DirectXCommon* dxCommon = nullptr;
	SrvManager* srvManager = nullptr;

	//ランダムエンジン
	std::random_device seedGenerator;
	std::mt19937 randomEngine{seedGenerator()};
	std::uniform_real_distribution<float> distribution{-1.0f, 1.0f};

};
