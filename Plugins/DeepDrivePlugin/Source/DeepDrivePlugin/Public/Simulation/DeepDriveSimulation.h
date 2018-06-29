

#pragma once

#include "GameFramework/Actor.h"

#include "Public/Simulation/DeepDriveSimulationDefines.h"

#include "DeepDriveSimulation.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDeepDriveSimulation, Log, All);

class DeepDriveSimulationCaptureProxy;
class DeepDriveSimulationServerProxy;

class ADeepDriveAgent;
class ADeepDriveAgentControllerCreator;
class ADeepDriveAgentControllerBase;
class UCaptureSinkComponentBase;
class ADeepDriveSimulationFreeCamera;
class ADeepDriveSplineTrack;

USTRUCT(BlueprintType)
struct FDeepDriveAdditionalAgentData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSubclassOf<ADeepDriveAgent>	Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EDeepDriveAgentControlMode		Mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int32	ConfigurationSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int32	StartPositionSlot;
};

UCLASS()
class DEEPDRIVEPLUGIN_API ADeepDriveSimulation	:	public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeepDriveSimulation();

	// Called when the game starts or when spawned
	virtual void PreInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	float CaptureInterval = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
	FString		IPAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
	int32		Port = 9876;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	TSubclassOf<ADeepDriveAgent>	Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	EDeepDriveAgentControlMode	InitialControllerMode = EDeepDriveAgentControlMode::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	int32	InitialConfigurationSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	int32	StartPositionSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	TArray<FDeepDriveAdditionalAgentData>	AdditionalAgents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Agents)
	TMap<EDeepDriveAgentControlMode, ADeepDriveAgentControllerCreator*>	ControllerCreators;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FreeCamera)
	ADeepDriveSimulationFreeCamera	*FreeCamera = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configuration)
	int32	Seed;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void LookUp(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void Turn(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SelectCamera(EDeepDriveAgentCameraType CameraType);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SelectMode(EDeepDriveAgentControlMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void NextAgent();

	UFUNCTION(BlueprintCallable, Category = "Spectator")
	void PreviousAgent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Agents")
	void OnAgentSpawned(ADeepDriveAgent *SpawnedAgent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Agents")
	void OnCurrentAgentChanged(ADeepDriveAgent *CurrentAgent);

	UFUNCTION(BlueprintCallable, Category = "Misc")
	FRandomStream& acquireRandomStream(const FName &RandomStreamId);

	UFUNCTION(BlueprintCallable, Category = "Agents")
	void OnDebugTrigger();

	bool resetAgent();
	
	ADeepDriveAgent* getCurrentAgent() const;
	ADeepDriveAgentControllerBase* getCurrentAgentController() const;
	TArray<UCaptureSinkComponentBase*>& getCaptureSinks();

	FRandomStream& getRandomStream();

private:

	ADeepDriveAgent* spawnAgent(EDeepDriveAgentControlMode mode, int32 configSlot, int32 startPosSlot);

	void spawnAdditionalAgents();

	ADeepDriveAgentControllerBase* spawnController(EDeepDriveAgentControlMode mode, int32 configSlot, int32 startPosSlot);

	void switchToAgent(int32 index);
	void switchToCamera(EDeepDriveAgentCameraType type);

	bool									m_isActive = false;
	DeepDriveSimulationServerProxy			*m_ServerProxy = 0;
	DeepDriveSimulationCaptureProxy			*m_CaptureProxy = 0;
	TArray<UCaptureSinkComponentBase*>		m_CaptureSinks;

	TArray<ADeepDriveAgent*>				m_Agents;
	int32									m_curAgentIndex = 0;
	ADeepDriveAgent							*m_curAgent = 0;
	EDeepDriveAgentControlMode				m_curAgentMode = EDeepDriveAgentControlMode::NONE;


	ADeepDriveAgentControllerBase			*m_curAgentController = 0;

	EDeepDriveAgentCameraType				m_curCameraType = EDeepDriveAgentCameraType::NONE;
	float									m_OrbitCameraPitch = 0.0f;
	float									m_OrbitCameraYaw = 0.0f;

	FRandomStream							m_RandomStream;

	TMap<FName, FRandomStream>				m_RandomStreams;
};


inline ADeepDriveAgent* ADeepDriveSimulation::getCurrentAgent() const
{
	return m_curAgent;
}

inline ADeepDriveAgentControllerBase* ADeepDriveSimulation::getCurrentAgentController() const
{
	return m_curAgentController;
}

inline TArray<UCaptureSinkComponentBase*>& ADeepDriveSimulation::getCaptureSinks()
{
	return m_CaptureSinks;
}

inline FRandomStream& ADeepDriveSimulation::getRandomStream()
{
	return m_RandomStream;
}
