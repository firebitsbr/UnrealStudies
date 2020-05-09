// Copyright Rafael Ocariz 2020

#include "DrawDebugHelpers.h"
#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	if (GetNeededComponents())
	{
		SetupInputComponent();
	}
}

bool UGrabber::GetNeededComponents() noexcept
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No PhysicsHandle Component Attached."), *GetOwner()->GetName());
	}

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No InputComponent Attached."), *GetOwner()->GetName());
	}

	return PhysicsHandle && InputComponent;
}

void UGrabber::SetupInputComponent() noexcept
{
	InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
}

void UGrabber::Grab() noexcept
{
	auto ObjectToGrab = GetFirstPhysicsBodyInReach();

	if (ObjectToGrab.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ObjectToGrab.GetComponent(), 
			NAME_None, 
			GetLineTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const noexcept
{
	FVector PlayerViewpointLocation;

	auto LineTraceEnd = GetLineTraceEnd(OUT PlayerViewpointLocation);

	FCollisionQueryParams TraceParamenters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParamenters);

	return std::move(Hit);
}

FVector UGrabber::GetLineTraceEnd(FVector& PlayerViewpointLocation) const noexcept
{
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + (PlayerViewpointRotation.Vector() * GrabMaxDistance);
}

void UGrabber::Release() noexcept
{
	PhysicsHandle->ReleaseComponent();
}