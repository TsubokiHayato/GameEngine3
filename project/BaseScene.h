#pragma once

class SceneManager;

class BaseScene
{
public:
	//デストラクタ
	virtual ~BaseScene() = default;
	//初期化
	virtual void Initialize() = 0;
	//更新処理
	virtual void Update() = 0;
	//終了処理
	virtual void Finalize() = 0;
	//描画
	virtual void Draw() = 0;

private:
	//シーンマネージャー[絶対にdeleteしてはいけない]
	SceneManager* sceneManager_ = nullptr; 

public:
	virtual void SetSceneManager(SceneManager* sceneManager) { 
		sceneManager_ = sceneManager;
	}
};
