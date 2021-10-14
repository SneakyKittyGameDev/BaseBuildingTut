#pragma once

#include "DataTypes.Generated.h"

UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Foundation	UMETA(DisplayName = "Foundation"),
	Wall		UMETA(DisplayName = "Wall"),
	Ceiling		UMETA(DisplayName = "Ceiling")
};

USTRUCT(BlueprintType)
struct FBuildingVisualType
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	class UStaticMesh* BuildingMesh;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	EBuildType BuildType;
	UPROPERTY(EditDefaultsOnly, Category = "TUTORIAL")
	FName FilterCharacter;
};

class UInstancedStaticMeshComponent;

USTRUCT(BlueprintType)
struct FBuildingSocketData
{
	GENERATED_BODY()
	UInstancedStaticMeshComponent* InstancedComponent;
	int32 Index;
	FName SocketName;
	FTransform SocketTransform;
};

USTRUCT(BlueprintType)
struct FSocketInformation
{
	GENERATED_BODY()
	FName SocketName;
	bool bSocketInUse = false;
};

USTRUCT(BlueprintType)
struct FBuildIndexSockets
{
	GENERATED_BODY()
	int32 Index;
	TArray<FSocketInformation> SocketsInformation;
};

USTRUCT(BlueprintType)
struct FInstanceSocketCheck
{
	GENERATED_BODY()
	UInstancedStaticMeshComponent* InstancedComponent;
	TArray<FBuildIndexSockets> InstanceSocketInformation;
};