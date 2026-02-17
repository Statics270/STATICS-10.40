#pragma once
#include "framework.h"

namespace Vehicles {
    void SpawnVehicles()
    {
        UGameplayStatics* Statics = UGameplayStatics::GetDefaultObj();

        TArray<AActor*> Spawners;

        UClass* Class = AFortAthenaVehicleSpawner::StaticClass();

        Statics->GetAllActorsOfClass(UWorld::GetWorld(), Class, &Spawners);

        for (int i = 0; i < Spawners.Num(); i++)
        {
            AFortAthenaVehicleSpawner* VehicleSpawner = (AFortAthenaVehicleSpawner*)Spawners[i];
            
            if (!VehicleSpawner || !VehicleSpawner->GetVehicleClass())
                continue;

            // Spawn the vehicle using the spawner's vehicle class
            AActor* SpawnedActor = SpawnActorClass<AFortAthenaVehicleSpawner>(
                VehicleSpawner->K2_GetActorLocation(),
                VehicleSpawner->K2_GetActorRotation(),
                VehicleSpawner->GetVehicleClass()
            );

            if (!SpawnedActor)
                continue;

            // Properly initialize replication for multiplayer
            SpawnedActor->SetReplicates(true);
            SpawnedActor->SetReplicateMovement(true);
            
            // Set the owner to the game mode so it's properly networked
            SpawnedActor->SetOwner(UWorld::GetWorld()->AuthorityGameMode);

            // Initialize vehicle physics
            // Call PostInitializeComponents to ensure physics components are set up
            if (!SpawnedActor->bInitialized) {
                SpawnedActor->PostInitializeComponents();
                SpawnedActor->bInitialized = true;
            }

            // Enable collision and physics simulation
            TArray<UPrimitiveComponent*> PrimitiveComponents;
            SpawnedActor->GetComponents(PrimitiveComponents);
            
            for (UPrimitiveComponent* PrimComp : PrimitiveComponents) {
                if (PrimComp && PrimComp->IsSimulatingPhysics()) {
                    PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                    PrimComp->SetSimulatePhysics(true);
                    PrimComp->WakeRigidBody();
                }
            }

            // Force an initial update to ensure the vehicle is in the correct state
            SpawnedActor->OnRep_InitialOverlappingVehicles();
            SpawnedActor->UpdateOverlaps();

            Log(std::format("Spawned vehicle at: X={}, Y={}, Z={}", 
                SpawnedActor->K2_GetActorLocation().X,
                SpawnedActor->K2_GetActorLocation().Y,
                SpawnedActor->K2_GetActorLocation().Z).c_str());
        }

        Spawners.Free();
        
        Log(std::format("Spawned {} vehicles total.", Spawners.Num()).c_str());
    }
}