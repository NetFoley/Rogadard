// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogadardCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ARogadardCharacter::ARogadardCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 999.f, 0.f);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
}

void ARogadardCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARogadardCharacter, attacking);
	DOREPLIFETIME(ARogadardCharacter, hurted);
	DOREPLIFETIME(ARogadardCharacter, life);
	DOREPLIFETIME(ARogadardCharacter, currentLife);
}

void ARogadardCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void ARogadardCharacter::attack_Implementation()
{
	if (this->isNotStunned())
	{
		this->setAttack(true);
		this->GetCharacterMovement()->DisableMovement();
	}
}

void ARogadardCharacter::stopAttack()
{
	this->setAttack(false);
	this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ARogadardCharacter::hurt_Implementation(float dmg)
{
	this->setCurrentLife(this->getCurrentLife() - dmg);
	if (this->currentLife > 0.0f)
	{
		this->stopAttack();
		this->setHurt(true);
		this->GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("DAMAGED"));
	}
}	

void ARogadardCharacter::stopHurt()
{
	this->setHurt(false);
	this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ARogadardCharacter::setAttack(bool state)
{
	this->attacking = state;
}

void ARogadardCharacter::setHurt(bool state)
{
	this->hurted = state;
}

bool ARogadardCharacter::getAttack()
{
	return this->attacking;
}
void ARogadardCharacter::setLife_Implementation(float amount)
{
	this->life = amount;
	this->setCurrentLife(amount);
}

void ARogadardCharacter::setCurrentLife_Implementation(float amount)
{
	this->currentLife = amount;
}

float ARogadardCharacter::getLife()
{
	return this->life;
}
float ARogadardCharacter::getCurrentLife()
{
	return this->currentLife;
}

bool ARogadardCharacter::getHurt()
{
	return this->hurted;
}

bool ARogadardCharacter::isNotStunned()
{
	return !(this->attacking || this->hurted);
}

/*
//Client-specific functionality
if (IsLocallyControlled())

//Server-specific functionality
if (GetLocalRole() == ROLE_Authority)*/