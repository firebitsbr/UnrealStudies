// Copyright Rafael Ocariz 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Open() noexcept;
	void Close() noexcept;
	void MoveDoor(float, bool) noexcept;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)	float ClosedYaw;
	UPROPERTY(EditAnywhere)	float OpenedYaw;
	UPROPERTY(EditAnywhere)	float OpenSpeed;
	UPROPERTY(EditAnywhere)	float CloseSpeed;
	UPROPERTY(EditAnywhere)	float CloseDelay;
	UPROPERTY(EditAnywhere)	float MinMassToTrigger;
	UPROPERTY(EditAnywhere) ATriggerVolume* DoorSensor = nullptr;

	UAudioComponent* AudioComponent = nullptr;
	
	float TargetYaw;
	float FrameDeltaTime;
	float CloseTimer;

	bool IsOpen = false;

	void CheckPtrs() noexcept;
	[[nodiscard]] bool HasOverlappingActors(TArray<AActor*>&) const noexcept;
	[[nodiscard]] bool CanOverlappingActorsOpen() const noexcept;

	void PlaySoundOnDoorMovement(bool) noexcept;
};
