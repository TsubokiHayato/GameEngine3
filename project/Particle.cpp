#include "Particle.h"

void Particle::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{

	//引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	InitializeRandomEngine();//ランダムエンジンの初期化
	CreatePSO();//パイプラインステートオブジェクトの作成
	InitializeVertexData();//頂点データの初期化
	CreateVertexResource();//頂点リソースを作成する
	CreateVertexBufferView();//頂点バッファビューを作成する
	WriteVertexDataToVertexResource();//頂点リソースに頂点データに書き込む
}

void Particle::InitializeRandomEngine()
{
	//ランダムエンジンの初期化
	randomEngine.seed(seedGenerator());
}

void Particle::CreatePSO()
{
}

void Particle::InitializeVertexData()
{
}

void Particle::CreateVertexResource()
{
}

void Particle::CreateVertexBufferView()
{
}

void Particle::WriteVertexDataToVertexResource()
{
}
