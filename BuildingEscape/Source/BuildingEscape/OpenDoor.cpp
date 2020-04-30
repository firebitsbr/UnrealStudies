// Copyright Rafael Ocariz 2020


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatIteracts = GetWorld()->GetFirstPlayerController()->GetPawn();
	CloseTimer = 0.f;
	CheckDoorSensorPtr();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FrameDeltaTime = DeltaTime;
	auto CloseTimerStart = GetWorld()->GetTimeSeconds();

	if (DoorSensor && ActorThatIteracts && DoorSensor->IsOverlappingActor(ActorThatIteracts))
	{
		Open();
		CloseTimer = CloseTimerStart;
		return;
	}

	if (CloseTimerStart - CloseTimer < CloseDelay) return;

	Close();
}

void UOpenDoor::Open() noexcept
{
	auto currentYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = OpenedYaw;
	MoveDoor(currentYaw, true);
}

void UOpenDoor::Close() noexcept
{
	auto currentYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = ClosedYaw;
	MoveDoor(currentYaw, false);
}

void UOpenDoor::MoveDoor(float Current, bool IsOpening) noexcept
{
	GetOwner()->SetActorRotation(FRotator(0, FMath::FInterpConstantTo(Current, TargetYaw, FrameDeltaTime, IsOpening ? OpenSpeed : CloseSpeed), 0));
}

void UOpenDoor::CheckDoorSensorPtr() const noexcept
{
	if (!DoorSensor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component on it, but no DoorSensor set"), *GetOwner()->GetName());
	}
}

