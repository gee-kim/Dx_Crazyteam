#include "PreCompile.h"
#include "Camp.h"
#include <EngineCore/DefaultSceneComponent.h>

ACamp::ACamp() 
{

}

ACamp::~ACamp() 
{
}

void ACamp::BeginPlay()
{
	Super::BeginPlay();

	Renderer->SetSprite("Camp_Back1.png");
	Renderer->SetAutoSize(1.f, true);
}

void ACamp::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void ACamp::AddObjectInit()
{
	AddMapObject(1, 1, EMapObject::CampBlock);
	AddMapObject(3, 1, EMapObject::CampBlock);
	AddMapObject(1, 3, EMapObject::CampBlock);
	AddMapObject(3, 3, EMapObject::CampBlock);

	AddMapObject(6, 6, EMapObject::CampMoveBlock);
	AddMapObject(9, 6, EMapObject::CampMoveBlock);
	AddMapObject(6, 9, EMapObject::CampMoveBlock);
	AddMapObject(9, 9, EMapObject::CampMoveBlock);

	AddMapObject(6, 1, EMapObject::Item, EItemType::ItemBubble);
	AddMapObject(8, 1, EMapObject::Item, EItemType::ItemNiddle);
	AddMapObject(6, 3, EMapObject::Item, EItemType::ItemOwl);
	AddMapObject(8, 3, EMapObject::Item, EItemType::ItemRoller);

}

