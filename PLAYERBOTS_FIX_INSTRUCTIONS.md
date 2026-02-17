# PlayerBots.h Compilation Errors - Required Fixes

## Summary
The PlayerBots.h file has multiple structural issues causing 100+ compilation errors:

### 1. CRITICAL: Broken Emote() Function (lines 218-492)
**Problem:**
- Line 226: `FGameplayAbilitySpec Spec{}` is missing a semicolon
- Lines 227-487: Five functions (BuildStructure, PerformBuildAction, ShouldBuild, IsLocationOnRoof, FindRoofLandingLocation) are incorrectly defined INSIDE the Emote() function
- Lines 489-492: Orphaned code that should be inside Emote()

**Fix Required:**
```cpp
// Line 226 should be:
FGameplayAbilitySpec Spec{};

// Then add these lines after line 226 (closing Emote properly):
            AbilitySpecConstructor(&Spec, reinterpret_cast<UGameplayAbility*>(EmoteAbilityClass->DefaultObject), 1, -1, EmoteDef);
            GiveAbilityAndActivateOnce(reinterpret_cast<AFortPlayerStateAthena*>(PC->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec);
        }

// Lines 227-487 should become separate member functions (already correct code, just wrong location)

// Remove line 489 (the orphan ;)

// Remove lines 490-492 (they were moved into Emote)
```

### 2. Fix IsLocationOnRoof Variable References
**Problem:** All IsLocationOnRoof functions reference `bot->PC` but should use `PC` (member variable)

**Fixes Required:**
- Line 436: Change `if (bot->PC)` to `if (PC)`
- Line 437: Change `bool bHit = bot->PC->LineTraceSingle` to `bool bHit = PC->LineTraceSingle`

### 3. Remove Duplicate Functions

**Duplicates in PlayerBot struct:**
- Remove lines 771-831 (IsLocationOnRoof and FindRoofLandingLocation - duplicate #1)
- Remove lines 851-911 (IsLocationOnRoof and FindRoofLandingLocation - duplicate #2)

**Duplicates in BotsBTService_InventoryManager class:**
- Remove lines 1076-1136 (IsLocationOnRoof and FindRoofLandingLocation - duplicate #1)
- Remove lines 1177-1237 (IsLocationOnRoof and FindRoofLandingLocation - duplicate #2)
- Remove lines 1400-1460 (IsLocationOnRoof and FindRoofLandingLocation - duplicate #3)
- Note: These don't belong in InventoryManager class at all

**Duplicates in BotsBTService_AIEvaluator class:**
- Remove lines 1998-2058 (IsLocationOnRoof and FindRoofLandingLocation)
- Note: These also don't belong in AIEvaluator class

## Summary of Changes

1. Fix Emote() function syntax (1 line changed, 3 lines added, 4 lines removed)
2. Fix IsLocationOnRoof variable references in PlayerBot (2 lines changed)
3. Remove duplicate IsLocationOnRoof/FindRoofLandingLocation from PlayerBot (122 lines removed)
4. Remove duplicates from BotsBTService_InventoryManager (182 lines removed)
5. Remove duplicates from BotsBTService_AIEvaluator (61 lines removed)

**Total: ~371 lines affected**

## Verification
After applying fixes:
- Emote() function should be syntactically correct
- BuildStructure, PerformBuildAction, ShouldBuild, IsLocationOnRoof, FindRoofLandingLocation should be separate PlayerBot member functions
- IsLocationOnRoof should reference `PC` not `bot->PC`
- No duplicate function definitions
- All functions properly scoped within their classes/structs
