// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_IdolGameModeBase.h"
#include "VRM4UCapture/Public/VRM4U_VMCSubsystem.h"
#include "Kismet/GameplayStatics.h"




void AJJH_IdolGameModeBase::ServerGetVMCData_Implementation (const FVMCData& Data )
{
    if (this)
    {
	    MyData = Data;
        MulticastSendVMCData ( MyData );
    }
}

void AJJH_IdolGameModeBase::MulticastSendVMCData_Implementation ( const FVMCData& Data )
{
    FindVrmVMCObject ( );
    MyData = Data;
	//// 배열 데이터를 TMap에 복원
	//MyData.BoneData.Empty ( );
	//for (int32 i = 0; i < BoneNames.Num ( ); ++i) {
	//	MyData.BoneData.Add ( BoneNames[i] , BoneTransforms[i] );
	//	// 각 본의 이름과 변환 정보를 개별적으로 출력
	//}

	//MyData.CurveData.Empty ( );
	//for (int32 i = 0; i < CurveNames.Num ( ); ++i) {
	//	MyData.CurveData.Add ( CurveNames[i] , CurveValues[i] );
	//}
}

void AJJH_IdolGameModeBase::FindVrmVMCObject ( )
{
    // UVrmVMCObject 클래스를 상속받은 모든 오브젝트를 찾기 위한 배열
    TArray<UObject*> FoundObjects;

    // UVrmVMCObject 클래스의 모든 오브젝트를 찾음
    GetObjectsOfClass ( UVrmVMCObject::StaticClass ( ) , FoundObjects , true );

    // 찾은 오브젝트가 있으면, 그 중 첫 번째 오브젝트를 UVrmVMCObject로 캐스팅하여 반환
    if (FoundObjects.Num ( ) > 0)
    {
        VrmVMCObject = Cast<UVrmVMCObject> ( FoundObjects[0] );
        if (VrmVMCObject)
        {
            // 성공적으로 찾음, 원하는 작업 수행
            UE_LOG ( LogTemp , Error , TEXT ( "Found UVrmVMCObject" ) );
        }
    }
    else
    {
        UE_LOG ( LogTemp , Warning , TEXT ( "No UVrmVMCObject found" ) );
    }
}

void AJJH_IdolGameModeBase::BeginPlay ( )
{
    Super::BeginPlay ( );
    auto MyObj = NewObject<UVrmVMCObject> ( this , UVrmVMCObject::StaticClass ( ) );
}
