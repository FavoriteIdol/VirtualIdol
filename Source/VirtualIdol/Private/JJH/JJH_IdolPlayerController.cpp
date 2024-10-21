// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_IdolPlayerController.h"

void AJJH_IdolPlayerController::ServerGetVMCData_Implementation ( const FVMCData& Data )
{
	MyData = Data;
	if (HasAuthority ( ))
	{
		MulticastSendVMCData ( MyData );
	}
}

void AJJH_IdolPlayerController::MulticastSendVMCData_Implementation ( const FVMCData& Data )
{
    MyData = Data;

	bIsReceived = true;


//
//
//	if (bApplyPerfectSync) {
//		for (auto& c : MyData.CurveData) {
//			if (c.Key.Contains ( TEXT ( "BlendShape." ) ) == false) continue;
//
//			c.Key.RightChopInline ( 11 ); // [blendahape.]
//		}
//	}
//
//	TMap<FString , FTransform>& BoneTrans = MyData.BoneData;
//
//	const auto& MorphList = Output.AnimInstanceProxy->GetSkelMeshComponent ( )->GetSkinnedAsset ( )->GetMorphTargets ( );
//	for (auto& c : MyData.CurveData) {
//#if	UE_VERSION_OLDER_THAN(5,3,0)
//		{
//			SmartName::UID_Type NewUID;
//			FName NewName = *c.Key;
//
//			NewUID = Skeleton->GetUIDByName ( USkeleton::AnimCurveMappingName , NewName );
//
//			Output.Curve.Set ( NewUID , c.Value );
//		}
//#else
//		auto m = MorphList.FindByPredicate ( [&c]( const TObjectPtr<UMorphTarget>& m ) {
//			FString s = c.Key;
//
//			if (m->GetName ( ).Compare ( s , ESearchCase::IgnoreCase )) {
//				return false;
//			}
//			return true;
//		} );
//		if (m) {
//			Output.Curve.Set ( *m->GetName ( ) , c.Value );
//		}
//		else {
//			Output.Curve.Set ( *c.Key , c.Value );
//		}
//#endif
//	}
//
//	{
//		bool bFirstBone = true;
//
//		for (const auto& t : VrmMetaObject_Internal->humanoidBoneTable) {
//#if	UE_VERSION_OLDER_THAN(4,27,0)
//			auto* tmpVal = BoneTrans.Find ( t.Key.ToLower ( ) );
//			if (tmpVal == nullptr) continue;
//
//			auto modelBone = *tmpVal;
//#else
//			auto filterList = BoneTrans.FilterByPredicate ( [&t]( TPair<FString , FTransform> a ) {
//				return a.Key.Compare ( t.Key , ESearchCase::IgnoreCase ) == 0;
//			}
//			);
//			if (filterList.Num ( ) != 1) continue;
//			auto modelBone = filterList.begin ( )->Value;
//#endif
//
//
//			int index = RefSkeleton.FindBoneIndex ( *t.Value );
//			if (index < 0) continue;
//
//			FBoneTransform f ( FCompactPoseBoneIndex ( index ) , modelBone );
//			f.Transform.SetRotation(FQuat::Identity);
//
//			if (bFirstBone) {
//				bFirstBone = false;
//
//				if (bUseRemoteCenterPos) {
//					auto v = f.Transform.GetLocation ( ) * ModelRelativeScale;
//					f.Transform.SetTranslation ( v );
//				}
//				else {
//					auto v = RefSkeletonTransform[index].GetLocation ( );
//					f.Transform.SetTranslation ( v );
//				}
//
//
//				 root bone
//				FTransform RootTrans;
//				for (auto& a : BoneTrans) {
//					if (a.Key.Compare ( TEXT ( "root" ) , ESearchCase::IgnoreCase )) {
//						continue;
//					}
//					RootTrans = a.Value;
//					break;
//				}
//				if (index == 0) {
//					 hip == root
//					f.Transform.SetTranslation ( f.Transform.GetLocation ( ) + RootTrans.GetLocation ( ) );
//				}
//				else {
//					 orig root
//					FBoneTransform bt ( FCompactPoseBoneIndex ( 0 ) , RootTrans );
//					tmpOutTransform.Add ( bt );
//					boneIndexTable.Add ( 0 );
//				}
//
//			}
//			else {
//				FVector v = RefSkeletonTransform[index].GetLocation ( );
//				f.Transform.SetTranslation ( v );
//			}
//
//			if (bIgnoreLocalRotation) {
//				auto r_refg = RefSkeletonTransform_global[index].GetRotation ( );
//				auto r_ref = RefSkeletonTransform[index].GetRotation ( );
//				auto r_vmc = f.Transform.GetRotation ( );
//
//				auto r_dif = r_refg.Inverse ( ) * r_vmc * r_refg;
//
//				f.Transform.SetRotation ( r_ref * r_dif );
//			}
//			f.Transform.SetTranslation(RefSkeletonTransform[index].GetLocation());
//			tmpOutTransform.Add ( f );
//			boneIndexTable.Add ( index );
//		}
//
//		 bone hierarchy
//		for (int i = 1; i < tmpOutTransform.Num ( ); ++i) {
//			int parentBoneIndex = RefSkeleton.GetParentIndex ( boneIndexTable[i] );
//			int parentInTable = boneIndexTable.Find ( parentBoneIndex );
//
//			for (int j = 0; j < 1000; ++j) {
//				if (parentInTable >= 0) {
//					break;
//				}
//				if (parentBoneIndex < 0) break;
//
//				 add outtransform with ref bone
//				FBoneTransform f ( FCompactPoseBoneIndex ( parentBoneIndex ) , RefSkeletonTransform[parentBoneIndex] );
//				tmpOutTransform.Add ( f );
//				boneIndexTable.Add ( parentBoneIndex );
//
//				parentBoneIndex = RefSkeleton.GetParentIndex ( parentBoneIndex );
//				parentInTable = boneIndexTable.Find ( parentBoneIndex );
//			}
//		}
//
//		 sort
//		tmpOutTransform.Sort ( FCompareBoneTransformIndex ( ) );
//		boneIndexTable.Sort ( );
//	}
//
//	for (int i = 0; i < tmpOutTransform.Num ( ); ++i) {
//		auto& a = tmpOutTransform[i];
//
//		int parentBoneIndex = RefSkeleton.GetParentIndex ( boneIndexTable[i] );
//
//		int parentInHandTable = boneIndexTable.Find ( parentBoneIndex );
//		if (parentInHandTable >= 0) {
//			a.Transform *= tmpOutTransform[parentInHandTable].Transform;
//		}
//		else {
//			 root
//			auto BoneSpace = EBoneControlSpace::BCS_ParentBoneSpace;
//			FAnimationRuntime::ConvertBoneSpaceTransformToCS ( ComponentTransform , Output.Pose , a.Transform , a.BoneIndex , BoneSpace );
//		}
//		OutBoneTransforms.Add ( a );
//	}

}