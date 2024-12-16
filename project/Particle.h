#pragma once
#include"MaterialData.h"
#include<random>
class DirectXCommon;
class SrvManager;

class Particle
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="dxCommon">DirectX共通部分</param>
    /// <param name="srvManager">SRVマネージャー</param>
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

    /// <summary>
    ///ランダムエンジンの初期化
    /// </summary>
    void InitializeRandomEngine();

    /// <summary>
    ///パイプラインステートオブジェクトの作成
    /// </summary>
    void CreatePSO();

    /// <summary>
    /// 頂点データの初期化
    /// </summary>
    void InitializeVertexData();

    /// <summary>
    /// 頂点リソースを作成する
    /// </summary>
    void CreateVertexResource();

    /// <summary>
    /// 頂点バッファビューを作成する
    /// </summary>
    void CreateVertexBufferView();

    /// <summary>
    ///頂点リソースに頂点データに書き込む
    /// </summary>
    void WriteVertexDataToVertexResource();

private:
    //ダイレクトX共通部分
    DirectXCommon* dxCommon_ = nullptr;
    //SRVマネージャー
    SrvManager* srvManager_ = nullptr;

    struct ParticleGroup {
        //マテリアルデータ
        MaterialData materialData;
        //パーティクルのリスト

        //インスタンシングデータ用SRVインデックス

        //インスタンシングリソース

        //インスタンス数

        //インスタンシングデータを書き込むためのポインタ
    };

    std::random_device seedGenerator;
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> distribution{-1.0f, 1.0f};
};
