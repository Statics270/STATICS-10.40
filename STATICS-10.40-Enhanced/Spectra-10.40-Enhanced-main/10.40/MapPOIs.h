#pragma once
#include "framework.h"

namespace MapPOIs {
    // POI Size Categories for weighted bot spawning
    enum class EPOISize {
        Major,      // High bot density: Tilted Towers, Retail Row, Pleasant Park
        Medium,     // Medium density: Salty Springs, Dusty Depot, Lazy Links
        Small,      // Low density: Random houses, camps, outposts
        MAX
    };

    // Structure to hold comprehensive POI data
    struct FPOIInfo {
        FString Name;
        FVector Location;
        EPOISize Category;
        int32 RecommendedBotCount;
        float Radius;
    };

    // Chapter 1 Season 10 - Complete POI Database
    // Approximate coordinates for Athena_Terrain map
    static const FPOIInfo POIDatabase[] = {
        // MAJOR CITIES - High Bot Density (12-20 bots recommended)
        { L"Tilted Towers",      { 1250,  3250,  200 },  EPOISize::Major,  18, 800.f },
        { L"Retail Row",         { 4250,  4750,  200 },  EPOISize::Major,  16, 750.f },
        { L"Pleasant Park",      { -1750, 1250,  200 },  EPOISize::Major,  16, 850.f },
        { L"Dusty Depot",        { 3500,  1000,  200 },  EPOISize::Major,  14, 600.f },
        { L"Paradise Palms",     { 2000,  6500,  200 },  EPOISize::Major,  14, 900.f },
        { L"Polar Peak",         { -250,  7500,  400 },  EPOISize::Major,  14, 800.f },
        { L"Mega Mall",          { 5500,  3250,  200 },  EPOISize::Major,  16, 750.f },

        // MEDIUM TOWNS - Medium Bot Density (6-12 bots recommended)
        { L"Salty Springs",      { -100,  -450,  200 },  EPOISize::Medium, 10, 500.f },
        { L"Lazy Links",         { 2500,  -4000, 200 },  EPOISize::Medium, 10, 700.f },
        { L"Fatal Fields",       { 3500,  3750,  200 },  EPOISize::Medium, 10, 600.f },
        { L"Loot Lake",          { 3000,  2000,  150 },  EPOISize::Medium, 10, 700.f },
        { L"Lonely Lodge",       { 5250,  1750,  200 },  EPOISize::Medium, 8,  650.f },
        { L"Shifty Shafts",      { 1000,  -1000, 100 },  EPOISize::Medium, 8,  450.f },
        { L"Tomato Temple",      { 4500,  -500,  200 },  EPOISize::Medium, 8,  550.f },
        { L"Wailing Woods",      { 5250,  -2000, 200 },  EPOISize::Medium, 6,  800.f },
        { L"Frosty Flights",     { -2500, -5000, 400 },  EPOISize::Medium, 8,  700.f },

        // SMALL AREAS - Low Bot Density (2-6 bots recommended)
        { L"Haunted Hills",      { -3500, 1000,  200 },  EPOISize::Small,  6,  450.f },
        { L"Greasy Grove",       { -3500, -3500, 200 },  EPOISize::Small,  6,  500.f },
        { L"Snobby Shores",      { -4500, 500,   200 },  EPOISize::Small,  6,  550.f },
        { L"Happy Hamlet",       { 500,   3000,  400 },  EPOISize::Small,  5,  500.f },
        { L"Lazy Lagoon",        { 5500,  -2500, 200 },  EPOISize::Small,  5,  550.f },
        { L"Junky Junction",     { -2500, 0,     200 },  EPOISize::Small,  5,  450.f },
        { L"Pressure Plant",     { 5000,  3000,  300 },  EPOISize::Small,  4,  550.f },
        { L"Sunny Steps",        { 6000,  500,   300 },  EPOISize::Small,  4,  500.f },
        { L"Starry Suburbs",     { -1500, -1500, 200 },  EPOISize::Small,  4,  400.f },
    };

    static const int32 POICount = sizeof(POIDatabase) / sizeof(FPOIInfo);

    // Get recommended bot count for a POI category
    inline int32 GetRecommendedBotCountForCategory(EPOISize Category) {
        switch (Category) {
            case EPOISize::Major:
                return UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(12, 20);
            case EPOISize::Medium:
                return UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(6, 12);
            case EPOISize::Small:
                return UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(2, 6);
            default:
                return 4;
        }
    }

    // Get random POI with weighted probability
    inline const FPOIInfo* GetWeightedRandomPOI() {
        auto Math = UKismetMathLibrary::GetDefaultObj();
        
        // Weighted selection: Major 45%, Medium 35%, Small 20%
        float RandomWeight = Math->RandomFloatInRange(0.f, 100.f);
        EPOISize TargetCategory;
        
        if (RandomWeight < 45.f) {
            TargetCategory = EPOISize::Major;
        } else if (RandomWeight < 80.f) {
            TargetCategory = EPOISize::Medium;
        } else {
            TargetCategory = EPOISize::Small;
        }

        // Collect POIs of target category
        std::vector<const FPOIInfo*> TargetPOIs;
        for (int32 i = 0; i < POICount; i++) {
            if (POIDatabase[i].Category == TargetCategory) {
                TargetPOIs.push_back(&POIDatabase[i]);
            }
        }

        if (TargetPOIs.empty()) {
            return &POIDatabase[0]; // Fallback to first POI
        }

        // Pick random POI from target category
        int32 RandomIndex = Math->RandomIntegerInRange(0, TargetPOIs.size() - 1);
        return TargetPOIs[RandomIndex];
    }

    // Get POI by name
    inline const FPOIInfo* GetPOIByName(const FString& Name) {
        for (int32 i = 0; i < POICount; i++) {
            if (POIDatabase[i].Name == Name) {
                return &POIDatabase[i];
            }
        }
        return nullptr;
    }

    // Get random location within a POI radius
    inline FVector GetRandomLocationInPOI(const FPOIInfo* POI) {
        if (!POI) {
            return FVector();
        }

        auto Math = UKismetMathLibrary::GetDefaultObj();
        
        // Generate random point within POI radius using polar coordinates
        float Angle = Math->RandomFloatInRange(0.f, 360.f);
        float Distance = Math->RandomFloatInRange(0.f, POI->Radius * 0.85f); // Stay within 85% of radius
        
        FVector RandomOffset;
        RandomOffset.X = Math->Cos(Angle * 3.14159f / 180.f) * Distance;
        RandomOffset.Y = Math->Sin(Angle * 3.14159f / 180.f) * Distance;
        RandomOffset.Z = 0.f;
        
        return POI->Location + RandomOffset;
    }

    // Check if location is within any POI
    inline const FPOIInfo* GetPOIAtLocation(FVector Location) {
        for (int32 i = 0; i < POICount; i++) {
            float Distance = UKismetMathLibrary::GetDefaultObj()->Vector_Distance(Location, POIDatabase[i].Location);
            if (Distance <= POIDatabase[i].Radius) {
                return &POIDatabase[i];
            }
        }
        return nullptr;
    }

    // Get nearest POI to a location
    inline const FPOIInfo* GetNearestPOI(FVector Location) {
        const FPOIInfo* Nearest = nullptr;
        float NearestDistance = FLT_MAX;

        for (int32 i = 0; i < POICount; i++) {
            float Distance = UKismetMathLibrary::GetDefaultObj()->Vector_Distance(Location, POIDatabase[i].Location);
            if (Distance < NearestDistance) {
                NearestDistance = Distance;
                Nearest = &POIDatabase[i];
            }
        }

        return Nearest;
    }

    // Get all POIs of a specific category
    inline std::vector<const FPOIInfo*> GetPOIsByCategory(EPOISize Category) {
        std::vector<const FPOIInfo*> Result;
        for (int32 i = 0; i < POICount; i++) {
            if (POIDatabase[i].Category == Category) {
                Result.push_back(&POIDatabase[i]);
            }
        }
        return Result;
    }
}
