// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

class USkeleton;

/**
* This is a native transient structure.
* Contains:
* - BoneIndicesArray: Array of RequiredBoneIndices for Current Asset. In increasing order. Mapping to current Array of Transforms (Pose).
* - BoneSwitchArray: Size of current Skeleton. true if Bone is contained in RequiredBones array, false otherwise.
**/
struct FBoneContainer
{
private:
	/** Array of RequiredBonesIndices. In increasing order. */
	TArray<FBoneIndexType>	BoneIndicesArray;
	/** Array sized by Current RefPose. true if Bone is contained in RequiredBones array, false otherwise. */
	TBitArray<>				BoneSwitchArray;

	/** Asset BoneIndicesArray was made for. Typically a SkeletalMesh. */
	TWeakObjectPtr<UObject>	Asset;
	/** If Asset is a SkeletalMesh, this will be a pointer to it. Can be NULL if Asset is a USkeleton. */
	TWeakObjectPtr<USkeletalMesh> AssetSkeletalMesh;
	/** If Asset is a Skeleton that will be it. If Asset is a SkeletalMesh, that will be its Skeleton. */
	TWeakObjectPtr<USkeleton> AssetSkeleton;

	/** Pointer to RefSkeleton of Asset. */
	const FReferenceSkeleton* RefSkeleton;

	/** Mapping table between Skeleton Bone Indices and Pose Bone Indices. */
	TArray<int32> SkeletonToPoseBoneIndexArray;

	/** Mapping table between Pose Bone Indices and Skeleton Bone Indices. */
	TArray<int32> PoseToSkeletonBoneIndexArray;

	/** For debugging. */
	/** Disable Retargeting. Extract animation, but do not retarget it. */
	bool bDisableRetargeting;
	/** Disable animation compression, use RAW data instead. */
	bool bUseRAWData;

public:

	FBoneContainer();

	FBoneContainer(const TArray<FBoneIndexType>& InRequiredBoneIndexArray, UObject& InAsset);

	/** Initialize BoneContainer to a new Asset, RequiredBonesArray and RefPoseArray. */
	void InitializeTo(const TArray<FBoneIndexType>& InRequiredBoneIndexArray, UObject& InAsset);

	/** Returns true if FBoneContainer is Valid. Needs an Asset, a RefPoseArray, and a RequiredBonesArray. */
	const bool IsValid() const
	{
		return (Asset.IsValid() && (RefSkeleton != NULL) && (BoneIndicesArray.Num() > 0));
	}

	/** Get Asset this BoneContainer was made for. Typically a SkeletalMesh, but could also be a USkeleton. */
	UObject* GetAsset() const
	{
		return Asset.Get();
	}

	/** Get SkeletalMesh Asset this BoneContainer was made for. Could be NULL if Asset is a Skeleton. */
	USkeletalMesh* GetSkeletalMeshAsset() const
	{
		return AssetSkeletalMesh.Get();
	}

	/** Get Skeleton Asset. Could either be the SkeletalMesh's Skeleton, or the Skeleton this BoneContainer was made for. Is non NULL is BoneContainer is valid. */
	USkeleton* GetSkeletonAsset() const
	{
		return AssetSkeleton.Get();
	}

	/** Disable Retargeting for debugging. */
	void SetDisableRetargeting(bool InbDisableRetargeting)
	{
		bDisableRetargeting = InbDisableRetargeting;
	}

	/** True if retargeting is disabled for debugging. */
	bool GetDisableRetargeting() const
	{
		return bDisableRetargeting;
	}

	/** Ignore compressed data and use RAW data instead, for debugging. */
	void SetUseRAWData(bool InbUseRAWData)
	{
		bUseRAWData = InbUseRAWData;
	}

	/** True if we're requesting RAW data instead of compressed data. For debugging. */
	bool ShouldUseRawData() const
	{
		return bUseRAWData;
	}

	/**
	* returns Required Bone Indices Array
	*/
	const TArray<FBoneIndexType>& GetBoneIndicesArray() const
	{
		return BoneIndicesArray;
	}

	/**
	* returns Bone Switch Array. BitMask for RequiredBoneIndex array.
	*/
	const TBitArray<>& GetBoneSwitchArray() const
	{
		return BoneSwitchArray;
	}

	/** Pointer to RefPoseArray for current Asset. */
	const TArray<FTransform>& GetRefPoseArray() const
	{
		return RefSkeleton->GetRefBonePose();
	}

	/** Access to Asset's RefSkeleton. */
	const FReferenceSkeleton& GetReferenceSkeleton() const
	{
		return *RefSkeleton;
	}

	/** Number of Bones in RefPose for current asset. This is NOT the number of bones in RequiredBonesArray, but the TOTAL number of bones in the RefPose of the current Asset! */
	const int32 GetNumBones() const
	{
		return RefSkeleton->GetNum();
	}

	/** Get BoneIndex for BoneName for current Asset. */
	ENGINE_API int32 GetPoseBoneIndexForBoneName(const FName& BoneName) const;

	/** Get ParentBoneIndex for current Asset. */
	ENGINE_API int32 GetParentBoneIndex(const int32& BoneIndex) const;

	/** Get Depth between bones for current asset. */
	int32 GetDepthBetweenBones(const int32& BoneIndex, const int32& ParentBoneIndex) const;

	/** Returns true if bone is child of for current asset. */
	bool BoneIsChildOf(const int32& BoneIndex, const int32& ParentBoneIndex) const;

	/**
	* Serializes the bones
	*
	* @param Ar - The archive to serialize into.
	* @param Rect - The bone container to serialize.
	*
	* @return Reference to the Archive after serialization.
	*/
	friend FArchive& operator<<(FArchive& Ar, FBoneContainer& B)
	{
		Ar
			<< B.BoneIndicesArray
			<< B.BoneSwitchArray
			<< B.Asset
			<< B.AssetSkeletalMesh
			<< B.AssetSkeleton
			<< B.SkeletonToPoseBoneIndexArray
			<< B.PoseToSkeletonBoneIndexArray
			<< B.bDisableRetargeting
			<< B.bUseRAWData
			;
		return Ar;
	}

	/**
	* Returns true of RequiredBonesArray contains this bone index.
	*/
	bool Contains(FBoneIndexType NewIndex) const
	{
		return BoneSwitchArray[NewIndex];
	}

	/** Const accessor to GetSkeletonToPoseBoneIndexArray(). */
	TArray<int32> const & GetSkeletonToPoseBoneIndexArray() const
	{
		return SkeletonToPoseBoneIndexArray;
	}

	/** Const accessor to GetSkeletonToPoseBoneIndexArray(). */
	TArray<int32> const & GetPoseToSkeletonBoneIndexArray() const
	{
		return PoseToSkeletonBoneIndexArray;
	}

private:
	/** Initialize FBoneContainer. */
	ENGINE_API void Initialize();

	/** Cache remapping data if current Asset is a SkeletalMesh, with all compatible Skeletons. */
	void RemapFromSkelMesh(USkeletalMesh const & SourceSkeletalMesh, USkeleton& TargetSkeleton);

	/** Cache remapping data if current Asset is a Skeleton, with all compatible Skeletons. */
	void RemapFromSkeleton(USkeleton const & SourceSkeleton);
};