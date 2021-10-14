// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ABuildingVisual::ABuildingVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BuildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMeshComponent"));
	RootComponent = BuildMesh;

	BuildingTypeIndex = 0;

	bMaterialIsTrue = false;
	
	bReturnedMesh = true;
}

// Called when the game starts or when spawned
void ABuildingVisual::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}

	if (MaterialTrue)
	{
		bMaterialIsTrue = true;
		BuildMesh->SetMaterial(0, MaterialTrue);
	}
}

ABuilding* ABuildingVisual::GetHitBuildingActor(const FHitResult& HitResult)
{
	return Cast<ABuilding>(HitResult.GetActor());
}

void ABuildingVisual::SetMeshTo(EBuildType BuildType)
{
	bReturnedMesh = false;
	for (const FBuildingVisualType& Building : BuildingTypes)
	{
		if (Building.BuildType == BuildType)
		{
			BuildMesh->SetStaticMesh(Building.BuildingMesh);
			return;
		}
	}
}

void ABuildingVisual::ReturnMeshToSelected()
{
	bReturnedMesh = true;
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		SetActorHiddenInGame(false);
		InteractingBuilding = GetHitBuildingActor(HitResult);
		if (InteractingBuilding)
		{
			if (!bReturnedMesh)
			{
				ReturnMeshToSelected();
			}
			
			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 25.0f);
			if (!SocketData.SocketTransform.Equals(FTransform()))
			{
				SetActorTransform(SocketData.SocketTransform);
				if (MaterialTrue && !bMaterialIsTrue)
				{
					bMaterialIsTrue = true;
					BuildMesh->SetMaterial(0, MaterialTrue);
				}
				return;
			}
			else
			{
				if (MaterialFalse && bMaterialIsTrue)
				{
					bMaterialIsTrue = false;
					BuildMesh->SetMaterial(0, MaterialFalse);
				}
				SetActorLocation(HitResult.Location);
			}
		}
		else
		{
			if (bReturnedMesh)
			{
				SetMeshTo(EBuildType::Foundation);
			}
			SetActorLocation(HitResult.Location);
		}
	}
	else
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true);
	}
}

void ABuildingVisual::SpawnBuilding()
{
	if (BuildingClass && !IsHidden())
	{
		if (InteractingBuilding)
		{
			if (bMaterialIsTrue)
			{
				InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
			}
		}
		else
		{
			GetWorld()->SpawnActor<ABuilding>(BuildingClass, GetActorTransform());
		}
	}
}

void ABuildingVisual::CycleMesh()
{
	if (bReturnedMesh)
	{
		if (++BuildingTypeIndex >= BuildingTypes.Num())
		{
			BuildingTypeIndex = 0;
		}

		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
		{
			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
		}
	}
}
