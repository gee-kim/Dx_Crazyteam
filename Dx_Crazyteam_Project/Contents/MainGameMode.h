#pragma once
#include "CAGameMode.h"
#include <EngineBase/EngineThread.h>
#include <EngineBase/EngineNetHeader.h>
#include <EngineCore/EngineNetWindow.h>
#include <EngineBase/EngineDispatcher.h>


class APlayer;
class AVillage;
class UTextWidget;
class AMainGameMode : public ACAGameMode
{
	GENERATED_BODY(ACAGameMode)
public:
	// constrcuter destructer
	AMainGameMode(); // 디폴트 생성자
	~AMainGameMode(); // 디폴트 소멸자

	// delete Function
	AMainGameMode(const AMainGameMode& _Other) = delete; // 디폴트 복사 생성자
	AMainGameMode(AMainGameMode&& _Other) noexcept = delete; 
	AMainGameMode& operator=(const AMainGameMode& _Other) = delete; // 디폴트 대입 연산자
	AMainGameMode& operator=(AMainGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel);
	void LevelEnd(ULevel* _NextLevel);
private:
	UTextWidget* ShowText = nullptr;
	std::shared_ptr<AVillage> Village = nullptr;
	std::shared_ptr<APlayer> Player1 = nullptr;
};

