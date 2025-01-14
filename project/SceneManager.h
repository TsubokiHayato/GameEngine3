#pragma once
#include "BaseScene.h"
class SceneManager
{

private:


	static SceneManager* instance;
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

public:

	static SceneManager* GetInstance();

	
	/// <summary>
	///初期化
	/// </summary>
	void Update();

	/// <summary>
	///描画
	/// </summary>
	void Draw();

	/// <summary>
	///終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// シーンを設定する
	/// </summary>
	/// <param name="scene">次のシーン</param>
	void SetNextScene(BaseScene* nextScene) {
		this->nextScene = nextScene;
	}

private:
	BaseScene* scene = nullptr;
	BaseScene* nextScene = nullptr;


};

