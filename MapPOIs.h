#pragma once
#include "framework.h"

namespace MapPOIs {
    // POI categories for weighted spawning
    enum class EPOICategory {
        MajorCity,     // High bot density (e.g., Tilted Towers, Retail Row, Pleasant Park)
        MediumTown,    // Medium density (e.g., Salty Springs, Dusty Depot, Lazy Links)
        SmallArea,     // Low density (e.g., random houses, camps, outposts)
        MAX
    };

    // Structure to hold POI data
    struct POI {
        std::string Name;
        FVector Center;
        float Radius;
        EPOICategory Category;
    };

    // Chapter 1 Season 10 major POIs
    // These are approximate coordinates for the Athena_Terrain map
    static const POI POIs[] = {
        // Major Cities (High Density)
        { "Tilted Towers", { 1250, 3250, 0 }, 600.f, EPOICategory::MajorCity },
        { "Retail Row", { 4250, 4750, 0 }, 700.f, EPOICategory::MajorCity },
        { "Pleasant Park", { -1750, 1250, 0 }, 800.f, EPOICategory::MajorCity },
        { "Dusty Depot", { 3500, 1000, 0 }, 500.f, EPOICategory::MajorCity },
        { "Lonely Lodge", { 5250, 1750, 0 }, 550.f, EPOICategory::MediumTown },

        // Medium Towns (Medium Density)
        { "Salty Springs", { -100, -450, 0 }, 400.f, EPOICategory::MediumTown },
        { "Lazy Links", { 2500, -4000, 0 }, 600.f, EPOICategory::MediumTown },
        { "Paradise Palms", { 2000, 6500, 0 }, 800.f, EPOICategory::MediumTown },
        { "Fatal Fields", { 3500, 3750, 0 }, 500.f, EPOICategory::MediumTown },
        { "Loot Lake", { 3000, 2000, 0 }, 600.f, EPOICategory::MediumTown },
        { "Tilted Towers (New)", { 1250, 5250, 0 }, 600.f, EPOICategory::MediumTown },
        { "Polar Peak", { -250, 7500, 0 }, 700.f, EPOICategory::MediumTown },

        // Small Areas (Low Density)
        { "Haunted Hills", { -3500, 1000, 0 }, 400.f, EPOICategory::SmallArea },
        { "Greasy Grove", { -3500, -3500, 0 }, 450.f, EPOICategory::SmallArea },
        { "Shifty Shafts", { 1000, -1000, 0 }, 400.f, EPOICategory::SmallArea },
        { "Snobby Shores", { -4500, 500, 0 }, 500.f, EPOICategory::SmallArea },
        { "Happy Hamlet", { 500, 3000, 0 }, 450.f, EPOICategory::SmallArea },
        { "Lazy Lagoon", { 5500, -2500, 0 }, 500.f, EPOICategory::SmallArea },
        { "Junky Junction", { -2500, 0, 0 }, 400.f, EPOICategory::SmallArea },
        { "Pressure Plant", { 5000, 3000, 0 }, 500.f, EPOICategory::SmallArea },
    };

    static const int POICount = sizeof(POIs) / sizeof(POI);

    // Get weighted random POI based on bot count and category
    POI* GetWeightedRandomPOI() {
        auto Math = UKismetMathLibrary::GetDefaultObj();
        
        // Calculate weights based on category
        // Major cities: 50% chance
        // Medium towns: 35% chance
        // Small areas: 15% chance
        float RandomWeight = Math->RandomFloatInRange(0.f, 100.f);
        
        EPOICategory TargetCategory;
        if (RandomWeight < 50.f) {
            TargetCategory = EPOICategory::MajorCity;
        } else if (RandomWeight < 85.f) {
            TargetCategory = EPOICategory::MediumTown;
        } else {
            TargetCategory = EPOICategory::SmallArea;
        }

        // Collect POIs of target category
        std::vector<POI*> TargetPOIs;
        for (int i = 0; i < POICount; i++) {
            if (POIs[i].Category == TargetCategory) {
                TargetPOIs.push_back((POI*)&POIs[i]);
            }
        }

        if (TargetPOIs.empty()) {
            return nullptr;
        }

        // Pick random POI from target category
        int RandomIndex = Math->RandomIntegerInRange(0, TargetPOIs.size() - 1);
        return TargetPOIs[RandomIndex];
    }

    // Get POI by name
    POI* GetPOIByName(const std::string& Name) {
        for (int i = 0; i < POICount; i++) {
            if (POIs[i].Name == Name) {
                return (POI*)&POIs[i];
            }
        }
        return nullptr;
    }

    // Get random location within a POI
    FVector GetRandomLocationInPOI(POI* Poi) {
        if (!Poi) {
            return FVector();
        }

        auto Math = UKismetMathLibrary::GetDefaultObj();
        
        // Generate random point within POI radius
        float Angle = Math->RandomFloatInRange(0.f, 360.f);
        float Distance = Math->RandomFloatInRange(0.f, Poi->Radius * 0.8f); // Stay within 80% of radius
        
        FVector RandomOffset;
        RandomOffset.X = Math->Cos(Angle) * Distance;
        RandomOffset.Y = Math->Sin(Angle) * Distance;
        RandomOffset.Z = 0.f;
        
        return Poi->Center + RandomOffset;
    }

    // Check if a location is within any POI
    POI* GetPOIAtLocation(FVector Location) {
        for (int i = 0; i < POICount; i++) {
            float Distance = UKismetMathLibrary::GetDefaultObj()->Vector_Distance(Location, POIs[i].Center);
            if (Distance <= POIs[i].Radius) {
                return (POI*)&POIs[i];
            }
        }
        return nullptr;
    }

    // Get nearest POI to a location
    POI* GetNearestPOI(FVector Location) {
        POI* Nearest = nullptr;
        float NearestDistance = FLT_MAX;

        for (int i = 0; i < POICount; i++) {
            float Distance = UKismetMathLibrary::GetDefaultObj()->Vector_Distance(Location, POIs[i].Center);
            if (Distance < NearestDistance) {
                NearestDistance = Distance;
                Nearest = (POI*)&POIs[i];
            }
        }

        return Nearest;
    }
}
