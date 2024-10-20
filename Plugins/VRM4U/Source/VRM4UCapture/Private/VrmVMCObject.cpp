// Fill out your copyright notice in the Description page of Project Settings.


#include "VrmVMCObject.h"
#include "VRM4U_VMCSubsystem.h"

#include "Engine/Engine.h"
#include "UObject/StrongObjectPtr.h"
#include "Misc/ScopeLock.h"
#include "OSCManager.h"
#include "OSCServer.h"
#include "Kismet/GameplayStatics.h"
#include "JJH/JJH_IdolGameModeBase.h"

void UVrmVMCObject::DestroyServer() {
	ServerName = "";
	port = 0;

	if (OSCServer.Get()) {
		OSCServer->Stop();
	}
	OSCServer.Reset(nullptr);
}



void UVrmVMCObject::CreateServer(FString inName, uint16 inPort) {
	ServerName = inName;
	port = inPort;
#if	UE_VERSION_OLDER_THAN(4,25,0)
#else
	OSCServer.Reset(UOSCManager::CreateOSCServer(ServerName, port, true, true, FString(), this));

	OSCServer->OnOscMessageReceivedNative.RemoveAll(nullptr);
	OSCServer->OnOscMessageReceivedNative.AddUObject(this, &UVrmVMCObject::OSCReceivedMessageEvent);

#if WITH_EDITOR
	OSCServer->SetTickInEditor(true);
#endif // WITH_EDITOR
#endif

}

void UVrmVMCObject::OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port) {

	FOSCAddress a = UOSCManager::GetOSCMessageAddress(Message);
	FString addressPath = UOSCManager::GetOSCAddressFullPath(a);

	TArray<FString> str;
	UOSCManager::GetAllStrings(Message, str);

	TArray<float> curve;
	UOSCManager::GetAllFloats(Message, curve);

	FTransform t;
	float f = 0;
	if (curve.Num() == 1) {
		f = curve[0];
	}
	if (curve.Num() >= 7) {
		t.SetLocation(FVector(-curve[0], curve[2], curve[1]) * 100.f);
		t.SetRotation(FQuat(-curve[3], curve[5], curve[4], curve[6]));
	}
	if (curve.Num() >= 10) {
		t.SetScale3D(FVector(curve[7], curve[9], curve[8]));
	}

	if (addressPath == TEXT("/VMC/Ext/Root/Pos")) {
		VMCData.BoneData.FindOrAdd(str[0]) = t;
	}
	if (addressPath == TEXT("/VMC/Ext/Bone/Pos")) {
		VMCData.BoneData.FindOrAdd(str[0]) = t;
	}
	if (addressPath == TEXT("/VMC/Ext/Blend/Val")) {
		VMCData.CurveData.FindOrAdd(str[0]) = f;
	}

	if (addressPath == TEXT("/VMC/Ext/OK")) {
		FScopeLock lock(&cs);
		VMCData_Cache = VMCData;
	}

	//UE_LOG ( LogTemp , Warning , TEXT ( "multicast1" ) );
	// 클라이언트로 최신 데이터를 동기화
	SetVMCData ( );

}
bool UVrmVMCObject::CopyVMCData(FVMCData& dst) {
	
	FScopeLock lock ( &cs );  // 멀티스레드 안전성을 보장하기 위한 락
	
	World = GetOuter()->GetWorld ( );
	if (World)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "1111" ) );
		IdolGMB = Cast<AJJH_IdolGameModeBase> ( World );
		if (IdolGMB)
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "2222" ) );
			IdolGMB->ServerGetVMCData ( VMCData );
			VMCData = IdolGMB->MyData;
		}
	}
	dst = VMCData;
	//if (UGameplayStatics::GetGameMode ( this ))
	//{
	//	IdolGMB = Cast<AJJH_IdolGameModeBase> ( UGameplayStatics::GetGameMode ( this ) );
	//	IdolGMB->ServerGetVMCData ( VMCData );
	//	VMCData = IdolGMB->MyData;
	//}
	dst = VMCData;  // 서버는 최신 데이터를 그대로 사용


	//for (const auto& Pair : VMCData_Cache.BoneData)
	//{
	//	UE_LOG ( LogTemp , Warning , TEXT ( "Bone: %s, Transform: %s" ) , *Pair.Key , *Pair.Value.ToString ( ) );
	//}
	//UE_LOG ( LogTemp , Warning , TEXT ( "multicast12" ) );
	//SetVMCData ( );  // 최신 데이터를 클라이언트로 전송

	//// GetOuter()를 사용하여 월드 가져오기
	//UWorld* World = GetWorld ( );
	//if (!World) {
	//	UE_LOG ( LogTemp , Warning , TEXT ( "NoWorld" ) );
	//	return false; // 월드를 가져올 수 없는 경우 오류 처리
	//}

	//// 현재 NetMode에 따라 처리
	//switch (World->GetNetMode ( )) {
	//case ENetMode::NM_Client:
	//	// 클라이언트에서는 캐시된 데이터를 사용
	//	dst = VMCData_Cache;
	//	break;
	//case ENetMode::NM_ListenServer:
	//	// 서버에서 최신 데이터를 클라이언트로 동기화
	//	dst = VMCData;  // 서버는 최신 데이터를 그대로 사용
	//	ServerSetVMCData ( );  // 최신 데이터를 클라이언트로 전송
	//	break;
	//default:
	//	dst = VMCData_Cache; // 기본적으로 캐시된 데이터를 사용
	//	break;
	//}
	return true;
}

class UWorld* UVrmVMCObject::GetWorld ( ) const
{
	return World;
}

void UVrmVMCObject::SetVMCData ( )
{
	//여기서 바꿔서 겜모베에 보내고 겜모베에서 멀티캐스트한 데이터를 받아와서 쓴다.
	
	// 서버에서 실행되는 로직
	TArray<FString> BoneNames;
	TArray<FTransform> BoneTransforms;
	TArray<FString> CurveNames;
	TArray<float> CurveValues;

	// TMap 데이터를 배열로 변환
	for (const TPair<FString , FTransform>& Bone : VMCData.BoneData) {
		BoneNames.Add ( Bone.Key );
		BoneTransforms.Add ( Bone.Value );
		// Bone.Value (FTransform)를 UE_LOG로 출력

	}
	for (const TPair<FString , float>& Curve : VMCData.CurveData) {
		CurveNames.Add ( Curve.Key );
		CurveValues.Add ( Curve.Value );
	}
	// 또는 특정 게임모드 클래스로 캐스팅
	
	// 클라이언트로 배열 데이터를 전송
	MulticastSetVMCData ( BoneNames , BoneTransforms , CurveNames , CurveValues );
}

void UVrmVMCObject::FindVrmVMCObject ( )
{
}

void UVrmVMCObject::MulticastSetVMCData_Implementation ( const TArray<FString>& BoneNames , const TArray<FTransform>& BoneTransforms , const TArray<FString>& CurveNames , const TArray<float>& CurveValues ) {
	FScopeLock Lock ( &cs );

	// 배열 데이터를 TMap에 복원
	VMCData_Cache.BoneData.Empty ( );
	for (int32 i = 0; i < BoneNames.Num ( ); ++i) {
		VMCData_Cache.BoneData.Add ( BoneNames[i] , BoneTransforms[i] );
		// 각 본의 이름과 변환 정보를 개별적으로 출력
	}

	VMCData_Cache.CurveData.Empty ( );
	for (int32 i = 0; i < CurveNames.Num ( ); ++i) {
		VMCData_Cache.CurveData.Add ( CurveNames[i] , CurveValues[i] );
	}
}

void UVrmVMCObject::ClearVMCData() {
	FScopeLock lock(&cs);
	VMCData.ClearData();
	VMCData_Cache.ClearData();
	UE_LOG ( LogTemp , Error , TEXT ( "!@#$" ) );
}

