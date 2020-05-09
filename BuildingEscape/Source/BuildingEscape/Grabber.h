// Copyright Rafael Ocariz 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

// created just to be used as default on GetLineTraceEnd Function
static FVector DefaultVector = FVector();

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Variables
	UPROPERTY(EditAnywhere, Category = Properties) float GrabMaxDistance;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Functions
	[[nodiscard]] bool GetNeededComponents() noexcept;

	void SetupInputComponent() noexcept;

	void Grab() noexcept;

	[[nodiscard]] FHitResult GetFirstPhysicsBodyInReach() const noexcept;
	[[nodiscard]] FVector GetLineTraceEnd(FVector& = DefaultVector) const noexcept;

	void Release() noexcept;
};
