#pragma once
#include"BaseScene.h"
#include"Input.h"

class TitleScene : public BaseScene
{


public:

	//初期化
	void Initialize() override;
	//更新処理
	void Update()override;
	//終了処理
	void Finalize()override;
	//描画
	void Draw()override;

private:

};

