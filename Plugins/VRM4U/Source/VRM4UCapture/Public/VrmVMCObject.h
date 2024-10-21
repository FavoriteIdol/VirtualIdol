#pragma once

#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "UObject/StrongObjectPtr.h"
#include "OSCServer.h"  // for game build link error

#include "VrmVMCObject.generated.h"

struct FOSCMessage;

USTRUCT(BlueprintType)
struct FVMCDataPair
{
    GENERATED_BODY()

    UPROPERTY()
    FString Key;

    UPROPERTY()
    FTransform Value;
};

USTRUCT(BlueprintType)
struct FVMCData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ServerAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Port;

    // 원본 데이터
    TMap<FString, FTransform> BoneData;
    TMap<FString, float> CurveData;

    // 복제를 위한 배열
    //UPROPERTY()
    //TArray<FVMCDataPair> ReplicatedBoneData;

    //UPROPERTY()
    //TArray<FVMCDataPair> ReplicatedCurveData;

    //// 복제 전 데이터를 배열로 변환하는 함수
    //void PrepareForReplication()
    //{
    //    ReplicatedBoneData.Empty();
    //    for (const auto& Pair : BoneData)
    //    {
    //        ReplicatedBoneData.Add({Pair.Key, Pair.Value});
    //    }

    //    ReplicatedCurveData.Empty();
    //    for (const auto& Pair : CurveData)
    //    {
    //        ReplicatedCurveData.Add({Pair.Key, FTransform(Pair.Value, FQuat::Identity, FVector::OneVector)});
    //    }
    //}

    //// 전송된 배열 데이터를 다시 TMap으로 복원하는 함수
    //void ApplyReplicatedData()
    //{
    //    BoneData.Empty();
    //    for (const auto& Pair : ReplicatedBoneData)
    //    {
    //        BoneData.Add(Pair.Key, Pair.Value);
    //    }

    //    CurveData.Empty();
    //    for (const auto& Pair : ReplicatedCurveData)
    //    {
    //        CurveData.Add(Pair.Key, Pair.Value.GetTranslation().X);  // 커브 데이터는 X 축 값만 사용
    //    }
    //}
    void ClearData ( ) {
        BoneData.Empty ( );
        CurveData.Empty ( );
    }

    bool operator== ( const FVMCData& Other ) const {
        if (Port != Other.Port) return false;
        if (ServerAddress != Other.ServerAddress) return false;
        return true;
    }

};

// FVMCData 구조체에 대한 직렬화 지원 추가 (네트워크 전송을 위한 직렬화 기능)
//template<>
//struct TStructOpsTypeTraits<FVMCData> : public TStructOpsTypeTraitsBase2<FVMCData>
//{
//    enum 
//    {
//        WithNetSerializer = true
//    };
//};

UCLASS()
class VRM4UCAPTURE_API UVrmVMCObject : public UObject
{
    GENERATED_BODY()

private:
    FCriticalSection cs;  // 멀티스레드 안전성을 위한 크리티컬 섹션
    TStrongObjectPtr<UOSCServer> OSCServer;  // OSC 서버 객체 포인터
    FVMCData VMCData;  // 현재 VMC 데이터
    FVMCData VMCData_Cache;  // 클라이언트에서 사용할 캐시된 데이터

public:

    class ATP_ThirdPersonCharacter* MyCharacter;
    // Begin UObject interface
	virtual class UWorld* GetWorld() const override;
	// End UObject interface

	// Set this to a valid world right after creation
	UPROPERTY(Transient)
	UWorld * World;

    class AJJH_IdolGameModeBase* IdolGMB;

    void SetVMCData();  // 서버에서 VMC 데이터를 클라이언트에게 동기화하는 함수

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastSetVMCData(const TArray<FString>& BoneNames, const TArray<FTransform>& BoneTransforms, const TArray<FString>& CurveNames, const TArray<float>& CurveValues);  // 멀티캐스트로 데이터를 전송하는 함수

    void FindVrmVMCObject ( );

    FString ServerName;  // 서버 이름
    uint16 port;  // 포트 번호

    void CreateServer(FString name, uint16 port);  // OSC 서버 생성
    void DestroyServer();  // OSC 서버 파괴
    void OSCReceivedMessageEvent(const FOSCMessage& Message, const FString& IPAddress, uint16 Port);  // 메시지 수신 이벤트

    bool CopyVMCData(FVMCData& dst);  // 서버와 클라이언트 간의 VMC 데이터를 복사하는 함수
    void ClearVMCData();  // VMC 데이터를 초기화하는 함수
};
