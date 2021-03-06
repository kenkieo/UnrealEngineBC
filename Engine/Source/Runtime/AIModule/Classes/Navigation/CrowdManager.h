// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Tickable.h"
#include "CrowdManager.generated.h"

struct FNavMeshPath;
class UCrowdFollowingComponent;
/**
 *  Crowd manager is responsible for handling crowds using Detour (Recast library)
 *
 *  Agents will respect navmesh for all steering and avoidance updates, 
 *  but it's slower than AvoidanceManager solution (RVO, cares only about agents)
 *
 *  All agents will operate on the same navmesh data, which will be picked from
 *  navigation system defaults (UNavigationSystem::SupportedAgents[0])
 *
 *  To use it, you have to add CrowdFollowingComponent to your agent
 *  (usually: replace class of PathFollowingComponent in AIController by adding 
 *   those lines in controller's constructor
 *
 *   ACrowdAIController::ACrowdAIController(const FObjectInitializer& ObjectInitializer)
 *       : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
 *
 *   or simply add both components and switch move requests between them)
 *
 *  Actors that should be avoided, but are not being simulated by crowd (like players)
 *  should implement CrowdAgentInterface AND register/unregister themselves with crowd manager:
 *  
 *   UCrowdManager* CrowdManager = UCrowdManager::GetCurrent(this);
 *   if (CrowdManager)
 *   {
 *      CrowdManager->RegisterAgent(this);
 *   }
 *
 *   Check flags in CrowdDebugDrawing namespace (CrowdManager.cpp) for debugging options.
 */

USTRUCT()
struct AIMODULE_API FCrowdAvoidanceConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Crowd)
	float VelocityBias;

	UPROPERTY(EditAnywhere, Category=Crowd)
	float DesiredVelocityWeight;
	
	UPROPERTY(EditAnywhere, Category=Crowd)
	float CurrentVelocityWeight;
	
	UPROPERTY(EditAnywhere, Category=Crowd)
	float SideBiasWeight;
	
	UPROPERTY(EditAnywhere, Category=Crowd)
	float ImpactTimeWeight;

	UPROPERTY(EditAnywhere, Category=Crowd)
	float ImpactTimeRange;

	// index in SamplingPatterns array or 0xff for adaptive sampling
	UPROPERTY(EditAnywhere, Category=Crowd)
	uint8 CustomPatternIdx;

	// adaptive sampling: number of divisions per ring
	UPROPERTY(EditAnywhere, Category=Crowd)
	uint8 AdaptiveDivisions;

	// adaptive sampling: number of rings
	UPROPERTY(EditAnywhere, Category=Crowd)
	uint8 AdaptiveRings;
	
	// adaptive sampling: number of iterations at best velocity
	UPROPERTY(EditAnywhere, Category=Crowd)
	uint8 AdaptiveDepth;

	FCrowdAvoidanceConfig() :
		VelocityBias(0.4f), DesiredVelocityWeight(2.0f), CurrentVelocityWeight(0.75f),
		SideBiasWeight(0.75f), ImpactTimeWeight(2.5f), ImpactTimeRange(2.5f),
		CustomPatternIdx(0xff), AdaptiveDivisions(7), AdaptiveRings(2), AdaptiveDepth(5)
	{}
};

USTRUCT()
struct AIMODULE_API FCrowdAvoidanceSamplingPattern
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Crowd)
	TArray<float> Angles;

	UPROPERTY(EditAnywhere, Category = Crowd)
	TArray<float> Radii;

	void AddSample(float AngleInDegrees, float NormalizedRadius);
	void AddSampleWithMirror(float AngleInDegrees, float NormalizedRadius);
};

struct AIMODULE_API FCrowdAgentData
{
#if WITH_RECAST
	/** special filter for checking offmesh links */
	struct dtQuerySpecialLinkFilter* LinkFilter;
#endif

	/** poly ref that agent is standing on from previous update */
	NavNodeRef PrevPoly;

	/** index of agent in detour crowd */
	int32 AgentIndex;

	/** remaining time for next path optimization */
	float PathOptRemainingTime;

	/** is this agent fully simulated by crowd? */
	uint32 bIsSimulated : 1;

	/** if set, agent wants path optimizations */
	uint32 bWantsPathOptimization : 1;

	FCrowdAgentData() :
#if WITH_RECAST
		LinkFilter(0),
#endif
		PrevPoly(0), AgentIndex(-1), PathOptRemainingTime(0), bIsSimulated(false), bWantsPathOptimization(false)
	{}

	bool IsValid() const { return AgentIndex >= 0; }
	void ClearFilter();
};

struct FCrowdTickHelper : FTickableGameObject
{
	TWeakObjectPtr<class UCrowdManager> Owner;

	FCrowdTickHelper() : Owner(NULL) {}
	virtual void Tick(float DeltaTime);
	virtual bool IsTickable() const { return Owner.IsValid(); }
	virtual bool IsTickableInEditor() const { return true; }
	virtual bool IsTickableWhenPaused() const { return true; }
	virtual TStatId GetStatId() const;
};

UCLASS()
class AIMODULE_API UCrowdManager : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime);
	virtual void BeginDestroy() override;

	/** adds new agent to crowd */
	void RegisterAgent(const class ICrowdAgentInterface* Agent);

	/** removes agent from crowd */
	void UnregisterAgent(const class ICrowdAgentInterface* Agent);

	/** updates agent data */
	void UpdateAgentParams(const class ICrowdAgentInterface* Agent) const;

	/** refresh agent state */
	void UpdateAgentState(const class ICrowdAgentInterface* Agent) const;

	/** update agent after using custom link */
	void OnAgentFinishedCustomLink(const class ICrowdAgentInterface* Agent) const;

	/** sets move target for crowd agent (only for fully simulated) */
	bool SetAgentMoveTarget(const UCrowdFollowingComponent* AgentComponent, const FVector& MoveTarget, TSharedPtr<const FNavigationQueryFilter> Filter) const;

	/** sets move direction for crowd agent (only for fully simulated) */
	bool SetAgentMoveDirection(const UCrowdFollowingComponent* AgentComponent, const FVector& MoveDirection) const;

	/** sets move target using path (only for fully simulated) */
	bool SetAgentMovePath(const UCrowdFollowingComponent* AgentComponent, const FNavMeshPath* Path, int32 PathSectionStart, int32 PathSectionEnd) const;

	/** clears move target for crowd agent (only for fully simulated) */
	void ClearAgentMoveTarget(const UCrowdFollowingComponent* AgentComponent) const;

	/** switch agent to waiting state */
	void PauseAgent(const UCrowdFollowingComponent* AgentComponent) const;

	/** resumes agent movement */
	void ResumeAgent(const UCrowdFollowingComponent* AgentComponent, bool bForceReplanPath = true) const;

	/** check if object is a valid crowd agent */
	bool IsAgentValid(const class UCrowdFollowingComponent* AgentComponent) const;
	bool IsAgentValid(const class ICrowdAgentInterface* Agent) const;

	/** returns number of nearby agents */
	int32 GetNumNearbyAgents(const class ICrowdAgentInterface* Agent) const;

	/** reads existing avoidance config or returns false */
	bool GetAvoidanceConfig(int32 Idx, FCrowdAvoidanceConfig& Data) const;

	/** updates existing avoidance config or returns false */
	bool SetAvoidanceConfig(int32 Idx, const FCrowdAvoidanceConfig& Data);

	/** remove started offmesh connections from corridor */
	void SetOffmeshConnectionPruning(bool bRemoveFromCorridor);

	/** block path visibility raycasts when crossing different nav areas */
	void SetSingleAreaVisibilityOptimization(bool bEnable);

	/** adjust current position in path's corridor, starting test from PathStartIdx */
	void AdjustAgentPathStart(const UCrowdFollowingComponent* AgentComponent, const FNavMeshPath* Path, int32& PathStartIdx) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void DebugTick() const;
#endif

	/** notify called when detour navmesh is changed */
	void OnNavMeshUpdate();

	class UWorld* GetWorld() const;

	static UCrowdManager* GetCurrent(class UObject* WorldContextObject);
	static UCrowdManager* GetCurrent(class UWorld* World);

protected:

	UPROPERTY(transient)
	class ANavigationData* MyNavData;

	/** obstacle avoidance params */
	UPROPERTY(EditAnywhere, Category=Config)
	TArray<FCrowdAvoidanceConfig> AvoidanceConfig;

	/** obstacle avoidance params */
	UPROPERTY(EditAnywhere, Category=Config)
	TArray<FCrowdAvoidanceSamplingPattern> SamplingPatterns;

	/** max number of agents supported by crowd */
	UPROPERTY(EditAnywhere, Category=Config)
	int32 MaxAgents;

	/** max radius of agent that can be added to crowd */
	UPROPERTY(EditAnywhere, Category=Config)
	float MaxAgentRadius;

	/** max number of neighbor agents for velocity avoidance */
	UPROPERTY(EditAnywhere, Category=Config)
	int32 MaxAvoidedAgents;

	/** max number of wall segments for velocity avoidance */
	UPROPERTY(EditAnywhere, Category=Config)
	int32 MaxAvoidedWalls;

	/** how often should agents check their position after moving off navmesh? */
	UPROPERTY(EditAnywhere, Category=Config)
	float NavmeshCheckInterval;

	/** how often should agents try to optimize their paths? */
	UPROPERTY(EditAnywhere, Category=Config)
	float PathOptimizationInterval;

	uint32 bPruneStartedOffmeshConnections : 1;
	uint32 bSingleAreaVisibilityOptimization : 1;

	/** agents registered in crowd manager */
	TMap<const class ICrowdAgentInterface*, FCrowdAgentData> ActiveAgents;

#if WITH_RECAST
	/** crowd manager */
	class dtCrowd* DetourCrowd;

	/** debug data */
	struct dtCrowdAgentDebugInfo* DetourAgentDebug;
	class dtObstacleAvoidanceDebugData* DetourAvoidanceDebug;
#endif

#if WITH_EDITOR
	FCrowdTickHelper* TickHelper;
#endif

	/** try to initialize nav data from already existing ones */
	virtual void UpdateNavData();

	/** setup params of crowd avoidance */
	virtual void UpdateAvoidanceConfig();

	/** called from tick, just after updating agents proximity data */
	virtual void PostProximityUpdate();

#if WITH_RECAST
	void AddAgent(const class ICrowdAgentInterface* Agent, FCrowdAgentData& AgentData) const;
	void RemoveAgent(const class ICrowdAgentInterface* Agent, FCrowdAgentData* AgentData) const;
	void GetAgentParams(const class ICrowdAgentInterface* Agent, struct dtCrowdAgentParams* AgentParams) const;

	/** prepare agent for next step of simulation */
	void PrepareAgentStep(const class ICrowdAgentInterface* Agent, FCrowdAgentData& AgentData, float DeltaTime) const;

	/** pass new velocity to movement components */
	void ApplyVelocity(const UCrowdFollowingComponent* AgentComponent, int32 AgentIndex) const;

	/** check changes in crowd simulation and adjust UE specific properties (smart links, poly updates) */
	void UpdateAgentPaths();

	/** switch debugger to object selected in PIE */
	void UpdateSelectedDebug(const class ICrowdAgentInterface* Agent, int32 AgentIndex) const;

	void CreateCrowdManager();
	void DestroyCrowdManager();

	void DrawDebugCorners(const struct dtCrowdAgent* CrowdAgent) const;
	void DrawDebugCollisionSegments(const struct dtCrowdAgent* CrowdAgent) const;
	void DrawDebugPath(const struct dtCrowdAgent* CrowdAgent) const;
	void DrawDebugVelocityObstacles(const struct dtCrowdAgent* CrowdAgent) const;
	void DrawDebugPathOptimization(const struct dtCrowdAgent* CrowdAgent) const;
	void DrawDebugNeighbors(const struct dtCrowdAgent* CrowdAgent) const;
#endif
};
