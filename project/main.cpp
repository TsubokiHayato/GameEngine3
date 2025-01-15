#include<Framework.h>
#include"Order.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//フレームワークのインスタンスを生成
	Framework* framework = new Order();

	framework->Run();

	delete framework;
	
	return 0;

}