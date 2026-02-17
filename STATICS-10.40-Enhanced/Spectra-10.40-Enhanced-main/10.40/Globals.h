#pragma once

namespace Globals {
    bool bIsProdServer = false;

    bool bCreativeEnabled = false;
    bool bSTWEnabled = false;
    bool bEventEnabled = false;

    bool bBotsEnabled = true;

    int MaxBotsToSpawn = 100;
    int MinPlayersForEarlyStart = 95;

    // Bot AI Configuration
    bool bBotBuildingEnabled = true;        // Enable bot building system
    bool bBotRoofLandingEnabled = true;     // Enable roof landing during skydiving
    bool bBotPOIBasedSpawning = true;       // Enable POI-based spawning
    
    // Bot Combat Configuration
    float BotAimAccuracy = 0.75f;          // Bot aim accuracy (0.0 - 1.0)
    float BotReactionTime = 0.25f;         // Bot reaction time in seconds
    float BotStrafeFrequency = 0.08f;      // How often bots strafe in combat
    int BotMaxBuildAttempts = 5;           // Max build attempts before giving up

    // Bot Looting Configuration
    bool bBotPrioritizeRoofChests = true;  // Prioritize chests on roofs
    float BotLootScanRange = 2000.f;       // Range to scan for loot
    float BotChestPriorityRange = 5000.f;  // Range to prioritize chests

    // Vehicle Configuration
    bool bVehiclesEnabled = true;          // Enable vehicle spawning
    float VehicleSpawnDelay = 2.0f;        // Delay after world load to spawn vehicles

    //REAL PLAYERS
    inline static int NextTeamIndex = 0;
    inline static int CurrentPlayersOnTeam = 0;
    inline static int MaxPlayersPerTeam = 1;

    //BOTS PLAYERS
    inline static int NextBotTeamIndex = 0;
    inline static int CurrentBotsOnTeam = 0;
}
