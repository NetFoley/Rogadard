// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogadardCharacter.generated.h"

UCLASS(Blueprintable)
class ARogadardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARogadardCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UWidgetComponent* GetHealthBar() const { return UWidgetComponent; }
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	//Check if character is stunned
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool isNotStunned();

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void attack();
	UFUNCTION(BlueprintCallable)
		void stopAttack();

	UFUNCTION(BlueprintCallable, Reliable, Server)
		void hurt(float dmg);
	UFUNCTION(BlueprintCallable)
		void stopHurt();

	UFUNCTION(BlueprintCallable)
		void setAttack(bool state);
	UFUNCTION(BlueprintCallable)
		void setHurt(bool state);
	UFUNCTION(BlueprintCallable, Reliable, Server)
		void setLife(float amount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float getLife();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool getAttack();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool getHurt();

	UPROPERTY(Replicated)
		float life = 100.0;
	UPROPERTY(Replicated)
		bool attacking = false;
	UPROPERTY(Replicated)
		bool hurted = false;
		
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;


};

