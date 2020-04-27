// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	TArray<FString> ListOfWords;
	FString HiddenWord;

	int32 Lifes;
	int32 Bulls;
	int32 Cows;

	void ClearHiddenWordAndLifes() noexcept;

	void InitGame() noexcept;
	void PrintHint() noexcept;

	bool IsValidInput(const FString& Input) noexcept;
	bool IsIsogram(const FString& Input) const noexcept;

	void WinEvent() noexcept;
	void LostEvent(const FString& Input) noexcept;
	void GetAmountOfBullsAndCows(const FString& Input) noexcept;

	bool Replay(const FString& Input) noexcept;
};
