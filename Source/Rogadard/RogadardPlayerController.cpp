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

	if (ARogadardCharacter* MyPawn = Cast<ARogadardCharacter>(GetPawn()))
	{
		this->ControlRotation =	((MyPawn->GetCursorToWorld()->GetTransformIncludingDecalSize().GetLocation()-MyPawn->GetTransform().GetLocation()).Rotation());
	}
}

void ARogadardPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ARogadardPlayerController::OnResetVR);

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &ARogadardPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARogadardPlayerController::MoveRight);
}

void ARogadardPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARogadardPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		APawn* const MyPawn = GetPawn();
		// add movement in that direction
		MyPawn->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value, false);
	}
}
void ARogadardPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		APawn* const MyPawn = GetPawn();
		// add movement in that direction
		MyPawn->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value, false);
	}
}

// Common destination setting and movement implementation.
void ARogadardPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	if (APawn* const MyPawn = GetPawn())
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly.
		if (Distance > 120.0f)
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}
