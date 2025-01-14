#include "SceneManager.h"
SceneManager* SceneManager::instance = nullptr;
SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;

}

void SceneManager::Update()
{
	// 次のシーンが設定されている場合
	if (nextScene) {
		// 旧シーンの終了処理
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}


		// シーン切り替え
		scene = nextScene;
		nextScene = nullptr;
		// シーンマネージャーのセットする
		scene->SetSceneManager(this);

		// 新シーンの初期化
		scene->Initialize();
	}
	//実行中のシーンの更新
	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::Finalize()
{
}
