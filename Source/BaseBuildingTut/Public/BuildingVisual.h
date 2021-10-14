// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BuildingVisual.generated.h"

class ABuilding;
class UMaterialInstance;

UCLASS()
class BASEBUILDINGTUT_API ABuildingVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TUTORIAL")
	class UStaticMeshComponent* BuildMesh;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	TSubclassOf<ABuilding> BuildingClass;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	TArray<FBuildingVisualType> BuildingTypes;
	uint8 BuildingTypeIndex;

	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	UMaterialInstance* MaterialFalse;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	UMaterialInstance* MaterialTrue;

	bool bMaterialIsTrue;

	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

	ABuilding* InteractingBuilding;

	FBuildingSocketData SocketData;

	void SetMeshTo(EBuildType BuildType);
	void ReturnMeshToSelected();
	bool bReturnedMesh;

public:
	void SetBuildPosition(const FHitResult& HitResult);
	void SpawnBuilding();
	void DestroyInstance(const FHitResult& HitResult);
	void CycleMesh();
};
