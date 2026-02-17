#!/usr/bin/env python3
"""
Fix PlayerBots.h compilation errors:
1. Extract misplaced functions from inside Emote()
2. Fix IsLocationOnRoof to use PC instead of bot->PC
3. Remove duplicate function definitions
"""

import re

# Read the file
with open('/home/engine/project/PlayerBots.h', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# Track line numbers for removing duplicates
lines_to_remove = set()
modified_lines = lines.copy()

# Fix 1: Fix the Emote() function - extract the misplaced functions
# The problem is around line 226 where Emote() has incomplete code and then functions defined inside
# Lines 489-492 need to be incorporated into Emote()

# Find the Emote function
emote_start = None
emote_end = None
for i, line in enumerate(lines):
    if 'void Emote()' in line and emote_start is None:
        emote_start = i
    # Look for the orphan lines at 489-492
    if i >= 488 and ';' in lines[i].strip() and i < 492:
        # This is line 489 with just ;
        if i == 488:
            emote_brace_line = i

# The fix: Line 226 should have semicolon, and we need to close Emote with the code from 490-491
# Remove line 489 (the orphan ;)
# Replace line 226 to close Emote properly with code from 490-491

# Fix line 226
if lines[226] == '            FGameplayAbilitySpec Spec{}\n':
    modified_lines[226] = '            FGameplayAbilitySpec Spec{};\n'

# Remove line 489 (the orphan ;)
lines_to_remove.add(488)  # Line 489 (0-indexed)

# Add closing brace to Emote after line 226 and add lines 490-491
# But first we need to insert the closing brace and the two lines from 490-491
# Actually, let's insert after the spec line
# Line 490: AbilitySpecConstructor(&Spec...
# Line 491: GiveAbilityAndActivateOnce...

# Remove lines 490-492 as they'll be moved
lines_to_remove.add(489)  # Line 490 (0-indexed)
lines_to_remove.add(490)  # Line 491 (0-indexed)
lines_to_remove.add(491)  # Line 492 (0-indexed)

# Insert the closing brace and the two lines after line 226
insert_after = 226  # 0-indexed for line 227
insert_lines = [
    '            AbilitySpecConstructor(&Spec, reinterpret_cast<UGameplayAbility*>(EmoteAbilityClass->DefaultObject), 1, -1, EmoteDef);\n',
    '            GiveAbilityAndActivateOnce(reinterpret_cast<AFortPlayerStateAthena*>(PC->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec);\n',
    '        }\n',
    '\n',
]

# Now we need to fix the IsLocationOnRoof functions to use PC instead of bot->PC
# The first IsLocationOnRoof is at lines 427-456 (inside Emote which is wrong)
# This should stay but change bot->PC to PC

for i, line in enumerate(modified_lines):
    if 'if (bot->PC)' in line and i < 500:  # First instance in PlayerBot
        modified_lines[i] = line.replace('if (bot->PC)', 'if (PC)')
    if 'bool bHit = bot->PC->LineTraceSingle' in line and i < 500:
        modified_lines[i] = line.replace('bool bHit = bot->PC->LineTraceSingle', 'bool bHit = PC->LineTraceSingle')

# Fix 2: Remove duplicate IsLocationOnRoof and FindRoofLandingLocation
# Duplicates in PlayerBot: lines 771-831, 851-911
# Duplicates in BotsBTService_InventoryManager: lines 1076-1136, 1177-1237, 1400-1460
# Duplicates in BotsBTService_AIEvaluator: lines 1998-2058

# Remove first duplicate in PlayerBot (lines 771-831)
for i in range(770, 831):  # 0-indexed for lines 771-831
    lines_to_remove.add(i)

# Remove second duplicate in PlayerBot (lines 851-911)
for i in range(850, 911):  # 0-indexed for lines 851-911
    lines_to_remove.add(i)

# Remove first duplicate in BotsBTService_InventoryManager (lines 1076-1136)
for i in range(1075, 1136):  # 0-indexed for lines 1076-1136
    lines_to_remove.add(i)

# Remove second duplicate in BotsBTService_InventoryManager (lines 1177-1237)
for i in range(1176, 1237):  # 0-indexed for lines 1177-1237
    lines_to_remove.add(i)

# Remove third duplicate in BotsBTService_InventoryManager (lines 1400-1460)
for i in range(1399, 1460):  # 0-indexed for lines 1400-1460
    lines_to_remove.add(i)

# Remove duplicate in BotsBTService_AIEvaluator (lines 1998-2058)
for i in range(1997, 2058):  # 0-indexed for lines 1998-2058
    lines_to_remove.add(i)

# Build the new file
new_lines = []
insert_idx = insert_after + 1  # Insert after line 226 (0-indexed)
for i, line in enumerate(modified_lines):
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
