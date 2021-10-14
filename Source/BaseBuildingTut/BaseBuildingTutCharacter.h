// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseBuildingTutCharacter.generated.h"

class ABuildingVisual;

UCLASS(config=Game)
class ABaseBuildingTutCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ABaseBuildingTutCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);

protected:
	virtual void BeginPlay() override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FHitResult PerformLineTrace(float Distance = 650.0f, bool DrawDebug = false);

	UPROPERTY(BlueprintReadOnly, Category = "TUTORIAL")
	bool bInBuildMode;

	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	TSubclassOf<ABuildingVisual> BuildingClass;
	UPROPERTY(BlueprintReadOnly, Category = "TUTORIAL")
	ABuildingVisual* Builder;
public:
	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	void SetBuildMode(bool Enabled);
	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	bool GetBuildMode() const { return bInBuildMode; }
	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	void CycleBuildingMesh();

	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	void SpawnBuilding();
	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	void DestroyBuildingInstance();

	virtual void Tick(float DeltaSeconds) override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

