// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullsCowsCount
{
	int32 Bulls;
	int32 Cows;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &Input) override;

	// Your declarations go below!
private:
	TArray<FString> ValidWords;
	FString HiddenWord;

	int32 Lifes;

	void ClearHiddenWordAndLifes() noexcept;

	void SetValidWords() noexcept;

	void InitGame() noexcept;
	void PrintHint() noexcept;

	bool ProcessInput(const FString&) noexcept;
	bool IsValidInput(const FString&) noexcept;
	bool IsIsogram(const FString&) const noexcept;

	void WinEvent() noexcept;
	void LostEvent(const FString&) noexcept;

	FBullsCowsCount GetBullsCows(const FString&) const noexcept;

	bool Replay(const FString&) noexcept;
};
