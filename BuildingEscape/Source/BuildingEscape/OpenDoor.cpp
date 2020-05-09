// Copyright Rafael Ocariz 2020

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"

#define OUT

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	CloseTimer = 0.f;
	CheckPtrs();
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FrameDeltaTime = DeltaTime;
	auto CloseTimerStart = GetWorld()->GetTimeSeconds();

	if (DoorSensor && CanOverlappingActorsOpen())
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

	PlaySoundOnDoorMovement(true);
}

void UOpenDoor::Close() noexcept
{
	auto currentYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = ClosedYaw;
	MoveDoor(currentYaw, false);

	PlaySoundOnDoorMovement(false);
}

void UOpenDoor::MoveDoor(float Current, bool IsOpening) noexcept
{
	GetOwner()->SetActorRotation(FRotator(0, FMath::FInterpConstantTo(Current, TargetYaw, FrameDeltaTime, IsOpening ? OpenSpeed : CloseSpeed), 0));
}

void UOpenDoor::CheckPtrs() noexcept
{
	if (!DoorSensor)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component on it, but no DoorSensor set"), *GetOwner()->GetName());
	}

	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component on it, but no AudioComponent set"), *GetOwner()->GetName());
	}
}

bool UOpenDoor::HasOverlappingActors(TArray<AActor*>& OverlappingActors) const noexcept
{
	DoorSensor->GetOverlappingActors(OUT OverlappingActors);
	return OverlappingActors.Num() > 0;
}

bool UOpenDoor::CanOverlappingActorsOpen() const noexcept
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (HasOverlappingActors(OUT OverlappingActors))
	{
		for (const auto& Actor : OverlappingActors)
		{
			auto Component = Actor->FindComponentByClass<UPrimitiveComponent>();

			if (Component->IsSimulatingPhysics())
			{
				TotalMass += Component->GetMass();
			}
		}
	}

	return TotalMass >= MinMassToTrigger;
}

void UOpenDoor::PlaySoundOnDoorMovement(bool OpenState) noexcept
{
	if (IsOpen != OpenState && AudioComponent && !AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
		IsOpen = !IsOpen;
	}
}