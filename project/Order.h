#pragma once
#include"Framework.h"

class Order : public Framework
{
public:
	//初期化
	void Initialize()override;
	//更新
	void Update()override;
	//終了処理
	void Finalize()override;
	//描画
	void Draw()override;

};

