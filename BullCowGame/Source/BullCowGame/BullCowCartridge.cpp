// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    PrintLine(TEXT("Welcome to Bulls & Cows Game!"));
    PrintLine(TEXT("Press Enter to continue..."));

    ClearHiddenWordAndLifes();
    SetValidWords();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    ClearScreen();

    if (!HiddenWord.Equals(TEXT("")))
    {
        if (!ProcessInput(Input))
            return;
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

void UBullCowCartridge::SetValidWords() noexcept
{
    for (auto &Word : Words)
    {
        if (IsIsogram(Word) && Word.Len() < 10)
        {
            ValidWords.Emplace(Word);
        }
    }
}

void UBullCowCartridge::InitGame() noexcept
{
    int32 Index = FMath::RandRange(0, ValidWords.Num() - 1);
    HiddenWord = ValidWords[Index];
    ValidWords.RemoveAt(Index);
    Lifes = HiddenWord.Len();
}

void UBullCowCartridge::PrintHint() noexcept
{
    PrintLine(TEXT("The word has %i letters."), HiddenWord.Len());
    FString temp = Lifes == 1 ? "attempt" : "attempts";
    PrintLine(TEXT("You have %i more %s"), Lifes, *temp);
}

bool UBullCowCartridge::ProcessInput(const FString &Input) noexcept
{
    if (Lifes == 0 && Replay(Input))
        return false;

    if (!IsValidInput(Input))
    {
        PrintLine(TEXT("Invalid input, try again."));
        PrintHint();
        return false;
    }

    if (!Input.Equals(HiddenWord))
    {
        LostEvent(Input);
        return false;
    }

    return true;
}

bool UBullCowCartridge::IsValidInput(const FString &Input) noexcept
{
    auto bHasSameLength = Input.Len() == HiddenWord.Len();
    return bHasSameLength && IsIsogram(Input);
}

bool UBullCowCartridge::IsIsogram(const FString &Input) const noexcept
{
    auto bReturnValue = false;
    TSet<TCHAR> Letters;

    for (auto &Letter : Input)
    {
        Letters.Add(Letter, &bReturnValue);
        if (bReturnValue)
            break;
    }

    return !bReturnValue;
}

void UBullCowCartridge::WinEvent() noexcept
{
    PrintLine(TEXT("You found it!"));
    PrintLine(TEXT("Let's go to the next word!"));
    PrintLine(TEXT(""));
}

void UBullCowCartridge::LostEvent(const FString &Input) noexcept
{
    --Lifes;

    if (Lifes > 0)
    {
        auto Count = GetBullsCows(Input);

        PrintHint();
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
        return;
    }

    PrintLine(TEXT("You're lost all your chances!"));
    PrintLine(TEXT("Maybe next time."));

    PrintLine(TEXT(""));
    PrintLine(TEXT("Do you want to restart the game? (Y/N)"));
}

FBullsCowsCount UBullCowCartridge::GetBullsCows(const FString &Input) const noexcept
{
    FBullsCowsCount Count{0, 0};

    for (int32 Index = 0; Index < HiddenWord.Len(); ++Index)
    {
        if (HiddenWord[Index] == Input[Index])
        {
            ++Count.Bulls;
            continue;
        }

        for (auto &Letter : HiddenWord)
        {
            if (Input[Index] == Letter)
            {
                ++Count.Cows;
                break;
            }
        }
    }

    return Count;
}

bool UBullCowCartridge::Replay(const FString &Input) noexcept
{
    auto bReturnValue = false;

    if (Input.Equals("Y"))
    {
        ClearHiddenWordAndLifes();
        bReturnValue = true;
    }
    else if (Input.Equals("N"))
    {
        // Close the game
    }
    else
    {
        PrintLine(TEXT("Incorrect Input! Try again."));
    }

    return bReturnValue;
}