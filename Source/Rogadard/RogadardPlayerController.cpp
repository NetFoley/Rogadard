// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogadardPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "RogadardCharacter.h"
#include "Math.h"
#include "Engine/World.h"

ARogadardPlayerController::ARogadardPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ARogadardPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	ARogadardCharacter* MyPawn = Cast<ARogadardCharacter>(GetPawn());
	if (MyPawn)
	{
		this->ControlRotation = ((MyPawn->GetCursorToWorld()->GetTransformIncludingDecalSize().GetLocation() - MyPawn->GetTransform().GetLocation()).Rotation());
	}
}

void ARogadardPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &ARogadardPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARogadardPlayerController::MoveRight);
	InputComponent->BindAction("Attack", IE_Pressed, this, &ARogadardPlayerController::doAttack);
}

void ARogadardPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARogadardPlayerController::doAttack()
{
	ARogadardCharacter* MyPawn = Cast<ARogadardCharacter>(GetPawn());
	if (MyPawn){
		MyPawn->attack();
	}
}

void ARogadardPlayerController::MoveForward(float Value)
{
	ARogadardCharacter* MyPawn = Cast<ARogadardCharacter>(GetPawn());
	if (MyPawn){
		if (Value != 0.f) {
			MyPawn->AddMovementInput(FVector(1.f, 0.f, 0.f), Value, false);
		}
	}
}
void ARogadardPlayerController::MoveRight(float Value)
{
	ARogadardCharacter* MyPawn = Cast<ARogadardCharacter>(GetPawn());
	if (MyPawn) {
		if (Value != 0.f){
		MyPawn->AddMovementInput(FVector(0.f, 1.f, 0.f), Value, false);
}
	}
}
