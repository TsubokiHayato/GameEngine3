#include "SpriteCommon.h"
#include"PSO.h"
void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
	pso_ = new PSO();
	pso_->Initialize(dxCommon);
}

void SpriteCommon::DrawSettingsCommon()
{
	pso_->DrawSettingsCommon();

}
