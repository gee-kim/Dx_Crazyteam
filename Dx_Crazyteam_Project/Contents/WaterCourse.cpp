#include "PreCompile.h"
#include "WaterCourse.h"
#include <EngineCore/DefaultSceneComponent.h>
#include "BaseMap.h"
#include "CAGameMode.h"

AWaterCourse::AWaterCourse()
{
	WaterCourseRender = CreateDefaultSubObject<USpriteRenderer>("Render");
	WaterCourseRender->SetupAttachment(Root);
}

AWaterCourse::~AWaterCourse()
{
}

void AWaterCourse::BeginPlay()
{
	Super::BeginPlay();

	StateInit();
	CreateAnimation();

	WaterCourseRender->SetAutoSize(20.0f, true);
	WaterCourseRender->SetActive(false);
}

void AWaterCourse::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	State.Update(_DeltaTime);

	if (true == CreateStart)
	{
		//CreateWaterStream(_DeltaTime);
	}
}

void AWaterCourse::StateInit()
{
	State.CreateState("None");

	State.CreateState("CreateCenter"); // 중간 물줄기.
	State.CreateState("CreateStream"); // 물 줄기.
	State.CreateState("CreateEndStem");// 물 줄기 끝.

	State.CreateState("Delete"); // 사라짐.

	State.SetStartFunction("None", std::bind(&AWaterCourse::NoneBegin, this));
	State.SetUpdateFunction("None", std::bind(&AWaterCourse::NoneTick, this, std::placeholders::_1));


	State.SetFunction("CreateCenter",
		std::bind(&AWaterCourse::CreateCenterBegin, this),
		std::bind(&AWaterCourse::CreateCenterTick, this, std::placeholders::_1),
		std::bind(&AWaterCourse::CreateCenterExit, this));
	State.SetFunction("CreateStream",
		std::bind(&AWaterCourse::CreateStreamBegin, this),
		std::bind(&AWaterCourse::CreateStreamTick, this, std::placeholders::_1),
		std::bind(&AWaterCourse::CreateStreamExit, this));
	State.SetFunction("CreateEndStem",
		std::bind(&AWaterCourse::CreateEndStemBegin, this),
		std::bind(&AWaterCourse::CreateEndStemTick, this, std::placeholders::_1),
		std::bind(&AWaterCourse::CreateEndStemExit, this));

	State.SetFunction("Delete",
		std::bind(&AWaterCourse::DeleteBegin, this),
		std::bind(&AWaterCourse::DeleteTick, this, std::placeholders::_1),
		std::bind(&AWaterCourse::DeleteExit, this));

	State.ChangeState("None");
}


void AWaterCourse::CreateAnimation()
{
	// 물 줄기 센터
	WaterCourseRender->CreateAnimation("Water_Center", "Center11.png", 0.125f, true);

	{
		// 물줄기 뻗어 나갈 때 사용할 애니메이션
		WaterCourseRender->CreateAnimation("CreateUp", "up12.png", 0.125f, true, 0, 0);
		WaterCourseRender->CreateAnimation("CreateDown", "down12.png", 0.125f, true, 0, 0);
		WaterCourseRender->CreateAnimation("CreateLeft", "left12.png", 0.125f, true, 0, 0);
		WaterCourseRender->CreateAnimation("CreateRight", "right12.png", 0.125f, true, 0, 0);
	}

	{
		// 물 끝. -> Tick에서 사용할 애니메이션.
		WaterCourseRender->CreateAnimation("EndStemUp", "up12.png", 0.125f, true, 0, 1);
		WaterCourseRender->CreateAnimation("EndStemDown", "down12.png", 0.125f, true, 0, 1);
		WaterCourseRender->CreateAnimation("EndStemLeft", "left12.png", 0.125f, true, 0, 1);
		WaterCourseRender->CreateAnimation("EndStemRight", "right12.png", 0.125f, true, 0, 1);

		// 물 줄기 중간 -> Tick에서 사용할 애니메이션.
		WaterCourseRender->CreateAnimation("StreamUp", "up22.png", 0.125f, true, 0, 2);
		WaterCourseRender->CreateAnimation("StreamDown", "down22.png", 0.125f, true, 0, 2);
		WaterCourseRender->CreateAnimation("StreamLeft", "left22.png", 0.125f, true, 0, 2);
		WaterCourseRender->CreateAnimation("StreamRight", "right22.png", 0.125f, true, 0, 2);
	}

	{
		// 물 끝 사라짐.
		WaterCourseRender->CreateAnimation("D_EndStemUp", "up12.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_EndStemDown", "down12.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_EndStemLeft", "left12.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_EndStemRight", "right12.png", 0.125f, true, 1, 10);

		// 물 줄기 사라짐.
		WaterCourseRender->CreateAnimation("D_StreamUp", "up22.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_StreamDown", "down22.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_StreamLeft", "left22.png", 0.125f, true, 1, 10);
		WaterCourseRender->CreateAnimation("D_StreamRight", "right22.png", 0.125f, true, 1, 10);
	}

	WaterCourseRender->ChangeAnimation("Water_Center");
}

#pragma region None
void AWaterCourse::NoneBegin()
{
}

void AWaterCourse::NoneTick(float _DeltaTime)
{
}
#pragma endregion

#pragma region Center
void AWaterCourse::CreateCenterBegin()
{
	CenterLifeTime = 0.0f;
	CreateStart = true; // 물줄기 만들어라.
}
void AWaterCourse::CreateCenterTick(float _DeltaTime)
{
	CenterLifeTime += _DeltaTime;
	if (LifeTime <= CenterLifeTime)
	{
		State.ChangeState("Delete");
		return;
	}
}
void AWaterCourse::CreateCenterExit()
{
	CenterLifeTime = 0.0f;
}
#pragma endregion

#pragma region CreateStream // 물줄기
void AWaterCourse::CreateStreamBegin()
{
	switch (WaterCourseDir)
	{
	case EEngineDir::Up:
	{
		WaterCourseRender->ChangeAnimation("StreamUp");
		break;
	}
	case EEngineDir::Down :
	{
		WaterCourseRender->ChangeAnimation("StreamDown");
		break;
	}
	case EEngineDir::Left:
	{
		WaterCourseRender->ChangeAnimation("StreamLeft");
		break;
	}
	case EEngineDir::Right:
	{
		WaterCourseRender->ChangeAnimation("StreamRight");
		break;
	}
	default :
		break;
	}

	WaterCourseRender->SetActive(true);
}

void AWaterCourse::CreateStreamTick(float _DeltaTime)
{
	CenterLifeTime += _DeltaTime;

	if (1.0f <= CenterLifeTime)
	{
		State.ChangeState("Delete");
		return;
	}
}
void AWaterCourse::CreateStreamExit()
{
	CenterLifeTime = 0.0f;
}
#pragma endregion

#pragma region CreateEndStem  TODO
void AWaterCourse::CreateEndStemBegin()
{
}
void AWaterCourse::CreateEndStemTick(float _DeltaTime)
{
}
void AWaterCourse::CreateEndStemExit()
{
}
#pragma endregion



#pragma region Delete
void AWaterCourse::DeleteBegin()
{
}

void AWaterCourse::DeleteTick(float _DeltaTime)
{
	// Animation Final Frame callback
	
}

void AWaterCourse::DeleteExit()
{
}
#pragma endregion

// 터지면 들어옴.
void AWaterCourse::CreateWaterStream(float _DeltaTime)
{
	CreateDeltaTime += _DeltaTime;



	//for (size_t i = 1; i <= PowerValue; i++)
	//{
	//}

	if (CreateTime <= CreateDeltaTime)
	{
		CurPos;
		// 만들어 질 곳에 뭐가 있음?
		{
			std::shared_ptr<AMapObject> NextMapObject = GetGameMode()->GetCurMap()->GetMapObject(CurPos.y , CurPos.x+1);
			EMapObjectType type = NextMapObject->GetType();
			if (type == EMapObjectType::None)
			{
				std::shared_ptr<AWaterCourse> Stem = GetWorld()->SpawnActor<AWaterCourse>("Stream");
				Stem->SetDir(EEngineDir::Down);
				Stem->CreateWaterStream();
			}
		}
		/*{
			std::shared_ptr<AMapObject> NextMapObject = GetGameMode()->GetCurMap()->GetMapObject(CurPos.y + 1, CurPos.x);
			EMapObjectType type = NextMapObject->GetType();
			if (type == EMapObjectType::None)
			{
				std::shared_ptr<AWaterCourse> Stem = GetWorld()->SpawnActor<AWaterCourse>("Stream");
				Stem->SetDir(EEngineDir::Up);
				Stem->CreateWaterStream();
			}
		}
		{
			std::shared_ptr<AMapObject> NextMapObject = GetGameMode()->GetCurMap()->GetMapObject(CurPos.y, CurPos.x - 1);
			EMapObjectType type = NextMapObject->GetType();
			if (type == EMapObjectType::None)
			{
				std::shared_ptr<AWaterCourse> Stem = GetWorld()->SpawnActor<AWaterCourse>("Stream");
				Stem->SetDir(EEngineDir::Left);
				Stem->CreateWaterStream();
			}
		}
		{
			std::shared_ptr<AMapObject> NextMapObject = GetGameMode()->GetCurMap()->GetMapObject(CurPos.y, CurPos.x + 1);
			EMapObjectType type = NextMapObject->GetType();
			if (type == EMapObjectType::None)
			{
				std::shared_ptr<AWaterCourse> Stem = GetWorld()->SpawnActor<AWaterCourse>("Stream");
				Stem->SetDir(EEngineDir::Right);
				Stem->CreateWaterStream();
			}
		}*/





		CreateStart = false; // Tick Control
	}
	//NextMapObject->WaterInteract();

}