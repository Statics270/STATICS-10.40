#!/usr/bin/env python3
"""
Fix PlayerBots.h compilation errors:
1. Fix the Emote() function - extract misplaced functions
2. Fix IsLocationOnRoof and FindRoofLandingLocation to use PC instead of bot->PC
3. Remove duplicate function definitions
"""

# Read the file
with open('/home/engine/project/PlayerBots.h', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# Track lines to remove (0-indexed)
lines_to_remove = set()

# Fix 1: Fix Emote() function (lines 218-492)
# Line 226 needs semicolon
# Lines 489-492 should be moved into Emote() and the orphan ; on 489 removed

# Fix line 226 - add semicolon
lines[226] = '            FGameplayAbilitySpec Spec{};\n'

# Remove line 489 (orphan ;)
lines_to_remove.add(488)  # 0-indexed

# Remove lines 490-492 (they'll be moved into Emote)
lines_to_remove.add(489)  # 0-indexed
lines_to_remove.add(490)  # 0-indexed
lines_to_remove.add(491)  # 0-indexed

# Insert the closing brace and the two lines from 490-491 after line 226
insert_after = 226  # 0-indexed
insert_lines = [
    '            AbilitySpecConstructor(&Spec, reinterpret_cast<UGameplayAbility*>(EmoteAbilityClass->DefaultObject), 1, -1, EmoteDef);\n',
    '            GiveAbilityAndActivateOnce(reinterpret_cast<AFortPlayerStateAthena*>(PC->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec);\n',
    '        }\n',
    '\n',
]

# Fix 2: Fix IsLocationOnRoof in PlayerBot to use PC instead of bot->PC (lines 436-437)
# This is the first IsLocationOnRoof inside the misplaced section
lines[436] = lines[436].replace('if (bot->PC)', 'if (PC)')
lines[437] = lines[437].replace('bool bHit = bot->PC->LineTraceSingle', 'bool bHit = PC->LineTraceSingle')

# Fix 3: Remove duplicate IsLocationOnRoof and FindRoofLandingLocation from PlayerBot
# First duplicate: lines 771-831 (0-indexed: 770-830)
for i in range(770, 831):
    lines_to_remove.add(i)

# Second duplicate: lines 851-911 (0-indexed: 850-910)
for i in range(850, 911):
    lines_to_remove.add(i)

# Fix 4: Remove duplicates from BotsBTService_InventoryManager
# First duplicate: lines 1076-1136 (0-indexed: 1075-1135)
for i in range(1075, 1136):
    lines_to_remove.add(i)

# Second duplicate: lines 1177-1237 (0-indexed: 1176-1236)
for i in range(1176, 1237):
    lines_to_remove.add(i)

# Third duplicate: lines 1400-1460 (0-indexed: 1399-1459)
for i in range(1399, 1460):
    lines_to_remove.add(i)

# Fix 5: Remove duplicate from BotsBTService_AIEvaluator
# Duplicate: lines 1998-2058 (0-indexed: 1997-2057)
for i in range(1997, 2058):
    lines_to_remove.add(i)

# Build the new file
new_lines = []
for i, line in enumerate(lines):
    if i in lines_to_remove:
        continue
    new_lines.append(line)
    if i == insert_after:
        new_lines.extend(insert_lines)

# Write the fixed file
with open('/home/engine/project/PlayerBots.h', 'w', encoding='utf-8') as f:
    f.writelines(new_lines)

print(f"Fixed PlayerBots.h")
print(f"Removed {len(lines_to_remove)} duplicate/misplaced lines")
print(f"Added {len(insert_lines)} lines to fix Emote()")
print(f"Total lines before: {len(lines)}")
print(f"Total lines after: {len(new_lines)}")
