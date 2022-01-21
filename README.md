# SntsRatTool
China TurboHUD plugin + executable

## Features

- wiggle
- auto aim cast mages
- bone armor macro

## Requirements

- land of the dead on left click
- force move on space
- skeletal mages on right click
- (optional) bone armor on key 3

## How to use

- download exe and plugin
- create snts folder in plugin folder of lightning mod and copy SntsToolAdapter.cs into it
- restart lightning mod
- always start exe after lightning mod is started
- always close exe first before starting or restarting lightning mod
- !!important!! disable all wiggle / auto cast skeletal mages / bone armor macros from other tools like godly or lightning mod intelligent macros

## Wiggle

- pressing force move or force stand still will stop wiggle temporarely

## Auto Aim Skeletal Mages
- cast on max essence
- mages will be refreshed when < 10 mages or remaining duration < 4 seconds
- 1. prio: rift guardian > blue = yellow > juggernaut > minions > trash
- 2. prio: closest target

## Bone Armor
- cast when elite or >= 3 trash monsters in 15 yards range

## Open Issues
- wiggle simulates left click -> standing on pylons while not using force move will click them
- spawning mages simulates right click -> might rightclick into chat or on portrait
- hiding rift progress bar under minimap will disable the tool

## Configuration
- edit the following entries in SntsToolAdapter.cs:
```
/* CONFIGURATION */
// bone armor disabled by default
private const bool ENABLE_BONE_ARMOR_MACRO = false;
// wiggle enabled by default
private const bool ENABLE_WIGGLE = true;
// auto aim enabled by default
private const bool ENABLE_AUTO_AIM = true;
// when 10 mages are up, refresh then when X seconds remaining
private const int AUTO_AIM_SECONDS_LEFT_TO_RECAST_MAGE = 4;
// scan range of monsters for auto aim
private const int AUTO_AIM_SCAN_RANGE_IN_INGAME_YARDS = 60;
/* END CONFIGURATION */
```
