# STATICS-10.40-Enhanced

A complete enhanced version of the STATICS-10.40 Fortnite private server project with all planned bug fixes and AI enhancements integrated.

## Overview

This enhanced version of the Spectra 10.40 Fortnite private server includes major improvements to bot AI, vehicle spawning, and overall gameplay experience. The project maintains full compatibility with the existing hook system while adding substantial new features.

## Key Enhancements

### 1. Vehicles.h - COMPLETE REWRITE

**Fixed Issues:**
- Fixed vehicle replication with `SetReplicateMovement(true)` and proper physics initialization
- Added vehicle ownership setup to ensure proper networking
- Added proper vehicle spawning sequence with physics component initialization
- Enabled collision and physics simulation for all spawned vehicles
- Added logging for vehicle spawn locations and counts

**Changes:**
- Vehicles now properly replicate to all clients
- Physics simulation is correctly initialized
- Vehicles spawn with proper collision enabled

### 2. MapPOIs.h - NEW FILE

A comprehensive POI (Point of Interest) system for Chapter 1 Season 10:

**Features:**
```cpp
struct FPOIInfo {
    FString Name;
    FVector Location;
    EPOISize Category;
    int32 RecommendedBotCount;
};
```

**Major Cities (High bot density - 12-20 bots):**
- Tilted Towers
- Retail Row
- Pleasant Park
- Dusty Depot
- Paradise Palms
- Polar Peak
- Mega Mall

**Medium Towns (Medium density - 6-12 bots):**
- Salty Springs
- Lazy Links
- Fatal Fields
- Loot Lake
- Lonely Lodge
- Shifty Shafts
- Tomato Temple
- Wailing Woods
- Frosty Flights

**Small Areas (Low density - 2-6 bots):**
- Haunted Hills
- Greasy Grove
- Snobby Shores
- Happy Hamlet
- Lazy Lagoon
- Junky Junction
- Pressure Plant
- And more...

**Functions:**
- `GetWeightedRandomPOI()` - Returns a POI based on weighted probability (Major: 45%, Medium: 35%, Small: 20%)
- `GetRandomLocationInPOI()` - Generates a random spawn location within a POI's radius
- `GetPOIAtLocation()` - Checks if a location is within any POI
- `GetNearestPOI()` - Finds the closest POI to a location
- `GetPOIsByCategory()` - Returns all POIs of a specific category

### 3. PlayerBots.h - MAJOR ENHANCEMENTS

#### A. Fixed Teleportation Bug (lines ~1880-1900)
**Problem:** Bots would aggressively teleport when stuck, causing unnatural behavior.

**Solution:**
- Replaced aggressive teleportation with gradual unstuck logic
- Added `CheckIfStuck()` method that tracks position changes over time
- Added `TryGradualUnstuck()` method with progressive unstuck attempts:
  1. Try jumping
  2. Try crouch/uncrouch
  3. Try random direction movement
  4. Try aggressive forward movement
  5. Try launching into air
  6. Only teleport as absolute last resort after 15+ seconds
- Added obstacle detection with line traces before teleporting

#### B. Building System
**New enum `EBotBuildAction`:**
- `Defensive1x1` - Build 1x1 with roof for cover
- `RampRush` - Build ramps toward enemy
- `BoxUp` - Build around self for protection
- `EditPeek` - Build and edit to peek (reserved)
- `StairToZone` - Build stairs toward safe zone

**New Methods:**
- `BuildStructure(EBotBuildingType, Location, Rotation)` - Places building pieces
- `PerformBuildAction()` - Executes current build action
- `ShouldBuild()` - Determines if bot should build based on situation
- `EquipBuildingItem(EBotBuildingType)` - Equips appropriate building item

**Features:**
- Defensive building when under attack and low health
- Box building when healing under fire
- Ramp rushing toward enemies
- Stairs toward safe zone during storm

#### C. Enhanced Combat
**Improvements:**
- Improved aim accuracy with skill-based variance (`Globals::BotAimAccuracy`)
- Accuracy decreases when bot is stressed or moving
- Consistent strafing patterns during combat
- Periodic direction changes for strafing
- Crouch-peeking mechanics (random crouch/uncrouch during combat)
- Build cover when reloading/taking damage

#### D. Roof Landing System
**New Methods:**
- `IsLocationOnRoof(Location)` - Raycast to detect roof surfaces
- `FindRoofLandingLocation(BaseLocation)` - Finds best roof landing spot

**Features:**
- Bots prioritize landing on building roofs during skydiving
- Searches for nearby buildings within 1200 units
- Calculates score based on height and distance
- Classic C1S10 meta: roof chest camping support

#### E. Enhanced Warmup Behavior
**New Features:**
- Dance every 5-15 seconds (was 10s fixed)
- Better player approach logic
- Actual weapon firing in FightOtherPlayers mode
- "Dance party" clustering behavior (`DanceParty` warmup choice)
- Weighted random warmup choice selection
- `NextEmoteTime` tracking for consistent emote timing

**Warmup Choices (Weighted):**
- Emote (25%)
- ApproachPlayersAndEmote (20%)
- LookAtRandomPlayers (15%)
- PickaxeOtherPlayers (15%)
- FightOtherPlayers (15%)
- DanceParty (10%)

#### F. Improved Looting
**Features:**
- Prioritize roof chests with distance weighting
- `IsLocationOnRoof()` check for chest priority
- House entry/clearing logic
- Better weapon/healing priority

### 4. Bots.h - MODIFIED

**Changes:**
- Integrated POI-based spawn selection
- `SpawnPlayerBot()` now uses `MapPOIs::GetWeightedRandomPOI()` when `Globals::bBotPOIBasedSpawning` is enabled
- Spawns more bots at Major POIs, fewer at Small POIs
- Maintains backward compatibility with foundation-based spawning

### 5. ServerBotManager.h - MODIFIED

**Changes:**
- Added building materials to bot startup inventory when `Globals::bBotBuildingEnabled` is true
- Added dances to warmup inventory (all available dances)
- Initialize `NextEmoteTime` for warmup behavior

### 6. AIController.h - MODIFIED

**Changes:**
- Added building response when bumped during combat
- Bots attempt to box up when bumped while under attack

### 7. Globals.h - MODIFIED

**New Configuration Variables:**
```cpp
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
```

## File Structure

```
STATICS-10.40-Enhanced/
├── Spectra-10.40-Enhanced-main/
│   ├── README.md (this file)
│   ├── 10.40.sln (Visual Studio solution)
│   └── 10.40/
│       ├── 10.40.vcxproj
│       ├── 10.40.vcxproj.filters
│       ├── dllmain.cpp
│       ├── framework.h
│       ├── Globals.h (MODIFIED)
│       ├── Vehicles.h (FIXED)
│       ├── PlayerBots.h (MAJOR ENHANCEMENTS)
│       ├── Bots.h (MODIFIED)
│       ├── ServerBotManager.h (MODIFIED)
│       ├── AIController.h (MODIFIED)
│       ├── GameMode.h (existing)
│       ├── MapPOIs.h (NEW)
│       ├── BotNames.h
│       ├── AbilitySystemComponent.h
│       ├── BuildingActor.h
│       ├── BuildingContainer.h
│       ├── Controller.h
│       ├── FortInventory.h
│       ├── FortPickup.h
│       ├── FortWeapon.h
│       ├── Looting.h
│       ├── Misc.h
│       ├── Net.h
│       ├── NetDriver.h
│       ├── Pawn.h
│       ├── PE.h
│       ├── QuestManager.h
│       ├── SDK/ (Unreal Engine SDK)
│       └── minhook/ (Hooking library)
```

## Building the Project

1. Open `10.40.sln` in Visual Studio 2022
2. Ensure you're using the v143 toolset (Visual Studio 2022)
3. Select either Debug or Release configuration
4. Build the solution (Ctrl+Shift+B)
5. The output DLL will be in `x64/Release/` or `x64/Debug/`

## Configuration

All enhancements can be configured through `Globals.h`. Key settings:

```cpp
// Enable/disable features
Globals::bBotBuildingEnabled = true;        // Bot building
Globals::bBotRoofLandingEnabled = true;     // Roof landing
Globals::bBotPOIBasedSpawning = true;       // POI-based spawning
Globals::bVehiclesEnabled = true;           // Vehicle spawning

// Combat tuning
Globals::BotAimAccuracy = 0.75f;           // 0.0-1.0, higher = more accurate
Globals::BotReactionTime = 0.25f;          // Seconds before reacting
Globals::BotMaxBuildAttempts = 5;          // Build attempts before giving up
```

## Technical Details

### Compatibility
- Target: Fortnite version 10.40 (Chapter 1 Season 10)
- Platform: Windows (x86/x64 DLL)
- Build System: MSBuild/Visual Studio 2022 (v143 toolset)
- Language: C++17/20
- Hooking: MinHook library

### SDK Integration
All new code uses existing SDK structures:
- `AFortPlayerPawnAthena` for bot pawns
- `AFortAthenaAIBotController` for bot controllers
- `ABuildingSMActor` for building pieces
- `UFortBuildingItemDefinition` for building items
- `UAbilitySystemComponent` for emotes and abilities

### Behavior Tree Service Pattern
The existing behavior tree service pattern is preserved:
- `BotsBTService_InventoryManager` - Inventory management
- `BotsBTService_Healing` - Healing logic
- `BotsBTService_AIEvaluator` - AI evaluation and state tracking
- `BotsBTService_Warmup` - Warmup behavior
- `BotsBTService_AIDropZone` - Drop zone and skydiving
- `BotsBTService_Loot` - Looting behavior
- `BotsBTService_Combat` - Combat behavior

## Known Issues

None currently identified. All enhancements have been designed to maintain backward compatibility.

## Credits

Based on the original Spectra 10.40 project.

## License

Not for public distribution. For private server use only.
