// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "Building.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class BASEBUILDINGTUT_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	UInstancedStaticMeshComponent* FoundationInstancedMesh;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	UInstancedStaticMeshComponent* WallInstancedMesh;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	UInstancedStaticMeshComponent* CeilingInstancedMesh;
	
	TArray<FName> MeshInstancedSockets;

	TArray<FInstanceSocketCheck> InstanceSocketCheck;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsValidSocket(UInstancedStaticMeshComponent* HitComponent, const FName& Filter, const FName& SocketName);

public:
	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	void DestroyInstance(FVector HitPoint);

	UFUNCTION(BlueprintCallable, Category = "TUTORIAL")
	FTransform GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName);

	int32 GetHitIndex(const FHitResult& HitResult);

	FBuildingSocketData GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance = 100.0f);

	void AddInstance(const FBuildingSocketData& BuildingSocketData, EBuildType BuildType);
};