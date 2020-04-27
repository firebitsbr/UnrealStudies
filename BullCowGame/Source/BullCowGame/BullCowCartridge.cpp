// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    PrintLine(TEXT("Welcome to Bulls & Cows Game!"));
    PrintLine(TEXT("Press Enter to continue..."));

    ClearHiddenWordAndLifes();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    
    if(!HiddenWord.Equals(TEXT("")))
    {
        if(Lifes == 0 && Replay(Input)) return;

        if(!IsValidInput(Input))
        {
            PrintLine(TEXT("Invalid input, try again."));
            PrintHint();
            return;
        }

        if(!Input.Equals(HiddenWord))
        {
            LostEvent(Input);

            if(Lifes > 0)
            {
                PrintHint();
                PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
            }
            
            return;
        }

        WinEvent();
    }

    InitGame();
    PrintHint();
}

void UBullCowCartridge::ClearHiddenWordAndLifes() noexcept
{ 
	HiddenWord = TEXT(""); 
	Lifes = 0;
}

void UBullCowCartridge::InitGame() noexcept
{
    HiddenWord = TEXT("cake");
    Lifes = HiddenWord.Len();
}

void UBullCowCartridge::PrintHint() noexcept
{
    PrintLine(TEXT("The word has %i letters."), HiddenWord.Len());
    FString temp = Lifes == 1 ? "attempt" : "attempts";
    PrintLine(TEXT("You have %i more %s"), Lifes, *temp);
}

bool UBullCowCartridge::IsValidInput(const FString& Input) noexcept
{
    auto bHasSameLength = Input.Len() == HiddenWord.Len();
    return bHasSameLength && IsIsogram(Input);
}

bool UBullCowCartridge::IsIsogram(const FString& Input) const noexcept
{
    auto bReturnValue = false;

    TSet<TCHAR> Letters;

    for(auto& Letter : Input)
    {
        Letters.Add(Letter, &bReturnValue);
        if(bReturnValue) break;
    }

    return !bReturnValue;
}

void UBullCowCartridge::WinEvent() noexcept
{
    PrintLine(TEXT("You found it!"));
    PrintLine(TEXT("Let's go to the next word!"));
    PrintLine(TEXT(""));
}
	
void UBullCowCartridge::LostEvent(const FString& Input) noexcept
{
    --Lifes;

    if(Lifes > 0)
    {
        GetAmountOfBullsAndCows(Input);
        return;
    }

    PrintLine(TEXT("You're lost all your chances!"));
    PrintLine(TEXT("Maybe next time."));
    
    PrintLine(TEXT(""));
    PrintLine(TEXT("Do you want to restart the game? (Y/N)"));
}

void UBullCowCartridge::GetAmountOfBullsAndCows(const FString& Input) noexcept
{
    Bulls = 0;
    Cows = 0;

    for(int32 i = 0; i < HiddenWord.Len() && i < Input.Len(); ++i)
    {
        if(HiddenWord[i] == Input[i]) ++Bulls;
        else ++Cows;
    }
}

bool UBullCowCartridge::Replay(const FString& Input) noexcept
{
    auto bReturnValue = false;

    if(Input.Equals("Y"))
    {
        ClearHiddenWordAndLifes();
        bReturnValue = true;
    }
    else if(Input.Equals("N"))
    {
        // Close the game
    }
    else
    {
        PrintLine(TEXT("Incorrect Input! Try again."));
    }
    
    return bReturnValue;
}