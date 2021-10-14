// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "DrawDebugHelpers.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedStaticMeshComponent"));
	CeilingInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstancedStaticMeshComponent"));
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	FInstanceSocketCheck InstanceSocket;
	InstanceSocket.InstancedComponent = FoundationInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = WallInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = CeilingInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);
	
	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = FoundationInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Foundation);
	
	MeshInstancedSockets = FoundationInstancedMesh->GetAllSocketNames();
	MeshInstancedSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(CeilingInstancedMesh->GetAllSocketNames());
}

void ABuilding::DestroyInstance(const FBuildingSocketData& BuildingSocketData)
{
	if (BuildingSocketData.InstancedComponent)
	{
		BuildingSocketData.InstancedComponent->RemoveInstance(BuildingSocketData.Index);
	}
}

FTransform ABuilding::GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName)
{
	if (InstancedComponent && InstancedComponent->IsValidInstance(InstanceIndex))
	{
		FTransform InstanceTransform = FTransform();
		InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
		FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);
		InstanceTransform = SocketTransform * InstanceTransform;

		DrawDebugString(GetWorld(), InstanceTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
		DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 5.0f, 10, FColor::Red);
		FTransform Temp;
		InstancedComponent->GetInstanceTransform(InstanceIndex, Temp, true);
		DrawDebugSphere(GetWorld(), Temp.GetLocation(), 5.0f, 15, FColor::Blue);
		
		return InstanceTransform;
		
		/*FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);
		if (SocketTransform.Equals(FTransform()))
		{
			Success = false;
			return FTransform();
		}
		
		FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(SocketTransform, InstanceTransform);
		FVector RelativeLocation = RelativeTransform.GetLocation();
		if (WorldSpace)
		{
			RelativeLocation.Z = SocketTransform.GetLocation().Z;
			InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
			FVector WorldLocation = InstanceTransform.GetLocation() + RelativeLocation;
			RelativeTransform.SetLocation(WorldLocation);
			DrawDebugString(GetWorld(), RelativeTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
			DrawDebugSphere(GetWorld(), RelativeTransform.GetLocation(), 5.0f, 10, FColor::Red);
			return RelativeTransform;
		}
		
		RelativeLocation.Z = InstanceTransform.GetLocation().Z + SocketTransform.GetLocation().Z;
		RelativeTransform.SetLocation(RelativeLocation);
		
		return RelativeTransform;*/
	}

	return FTransform();
}

int32 ABuilding::GetHitIndex(const FHitResult& HitResult)
{
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 10, FColor::Red);

	return HitResult.Item;
}

bool ABuilding::IsValidSocket(UInstancedStaticMeshComponent* HitComponent, int32 Index, const FName& Filter, const FName& SocketName)
{
	bool bSuccess = true;
	if (!HitComponent->DoesSocketExist(SocketName))
	{
		bSuccess = false;
		return bSuccess;
	}
	FString FilterString = Filter.ToString();
	FString SocketNameString = SocketName.ToString();

	if (!SocketNameString.Contains(FilterString, ESearchCase::CaseSensitive))
	{
		bSuccess = false;
	}

	for (const FInstanceSocketCheck& InstanceSocketCheck : InstanceSocketsCheck)
	{
		if (InstanceSocketCheck.InstancedComponent == HitComponent)
		{
			for (const FBuildIndexSockets& BuildIndexSockets : InstanceSocketCheck.InstanceSocketInformation)
			{
				if (BuildIndexSockets.Index == Index)
				{
					for (const FSocketInformation& SocketInformation : BuildIndexSockets.SocketsInformation)
					{
						if (SocketInformation.SocketName == SocketName && SocketInformation.bSocketInUse)
						{
							bSuccess = false;
							break;
						}
					}
				}
			}
		}
	}
	
	return bSuccess;
}

FBuildingSocketData ABuilding::GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance)
{
	FBuildingSocketData SocketData = FBuildingSocketData();
	if (UInstancedStaticMeshComponent* HitComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
	{
		int32 HitIndex = GetHitIndex(HitResult);
	
		if (HitIndex != -1)
		{
			for (const FName& SocketName : MeshInstancedSockets)
			{
				if (IsValidSocket(HitComponent, HitIndex, Filter, SocketName))
				{
					FTransform SocketTransform = GetInstancedSocketTransform(HitComponent, HitIndex, SocketName);
					if (FVector::Distance(SocketTransform.GetLocation(), HitResult.Location) <= ValidHitDistance)
					{
						SocketData.Index = HitIndex;
						SocketData.InstancedComponent = HitComponent;
						SocketData.SocketName = SocketName;
						SocketData.SocketTransform = SocketTransform;
						return SocketData;
					}
				}
			}
		}
	}
	return SocketData;
}

void ABuilding::AddInstance(const FBuildingSocketData& BuildingSocketData, EBuildType BuildType)
{
	for (FInstanceSocketCheck& InstanceSocket : InstanceSocketsCheck)
	{
		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent)
		{
			bool bFoundMatchingIndex = false;

			for (FBuildIndexSockets& IndexSockets : InstanceSocket.InstanceSocketInformation)
			{
				if (IndexSockets.Index == BuildingSocketData.Index)
				{
					bFoundMatchingIndex = true;
					for (FSocketInformation& SocketInformation : IndexSockets.SocketsInformation)
					{
						if (SocketInformation.SocketName == BuildingSocketData.SocketName)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Setting Socket %s to TRUE"), *SocketInformation.SocketName);
							SocketInformation.bSocketInUse = true;
							break;
						}
					}
					break;
				}
			}

			if (!bFoundMatchingIndex)
			{
				FBuildIndexSockets BuildIndexSockets;
				BuildIndexSockets.Index = BuildingSocketData.Index;
			
				FSocketInformation SocketInformation;
				for (const FName& SocketName : InstanceSocket.InstancedComponent->GetAllSocketNames())
				{
					SocketInformation.SocketName = SocketName;
					SocketInformation.bSocketInUse = false;
					if (SocketName == BuildingSocketData.SocketName)
					{
						SocketInformation.bSocketInUse = true;
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
			
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}
		}
	}


	
	switch (BuildType)
	{
		case EBuildType::Foundation : FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
		case EBuildType::Wall : WallInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
		case EBuildType::Ceiling : CeilingInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
	}
}
