#include "gbafe.h"
#include "Skill/Skill.h"
#include "Skill/Shuffle.h"

struct SelectInfo const gSelectInfo_Shuffle =
{
    .onInit = ShuffleSelection_OnInit,
    .onSwitchIn = ShuffleSelection_OnChange,
    .onSelect = ShuffleSelection_OnSelect,
    .onCancel = GenericSelection_BackToUM,
};

u8 ShuffleUsability(const struct MenuItemDef* def, int number) {
	if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }
    if (gBmSt.unk3D & 1) {
        return MENU_NOTSHOWN;
    }
	if (gSkillTester(gActiveUnit, gShuffleId) == 0){ // dont use not for some magic reason
		return MENU_NOTSHOWN;
	}
    MakeShuffleTargetList(gActiveUnit);
    if (GetSelectTargetCount() == 0) {
        return MENU_NOTSHOWN;
    }
    return MENU_ENABLED;
}

u8 ShuffleEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
	MakeShuffleTargetList(gActiveUnit);
    NewTargetSelection(&gSelectInfo_Shuffle);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

u8 ShuffleSelection_OnSelect(ProcPtr proc, struct SelectTarget* target) {
    gActionData.unitActionType = UNIT_ACTION_TAKE; //this should do nothing
    gActionData.targetIndex = target->uid;
    UnitFinalizeMovement(GetUnit(gActionData.targetIndex));
    DoShuffleEffect(target->uid);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

void ShuffleSelection_OnInit(ProcPtr menu) {
    ClearBg0Bg1();
    StartSubtitleHelp(menu, GetStringFromIndex(gShuffleHelpText));
    return;
}

u8 ShuffleSelection_OnChange(ProcPtr proc, struct SelectTarget* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    return 0;
}

void DoShuffleEffect(int targetUid) {
	struct Unit* targetUnit = GetUnit(targetUid);
	int slot = NextRN_N(GetUnitUsableWeaponsCount(targetUnit)-1) + 1;
    slot = GetSlotOfNthUsableWeapon(targetUnit, slot);
    int tempItem = targetUnit->items[0];
    targetUnit->items[0] = targetUnit->items[slot];
    targetUnit->items[slot] = tempItem;
}

void MakeShuffleTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;
    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    ForEachAdjacentUnit(x, y, TryAddUnitToShuffleTargetList);
    return;
}

void TryAddUnitToShuffleTargetList(struct Unit* unit) {
	if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }
    if (GetUnitUsableWeaponsCount(unit) < 2){
        return;
    }
    AddTarget(unit->xPos, unit->yPos, unit->index, 0);
    return;
}

int GetUnitUsableWeaponsCount(struct Unit* unit){
    int usableWeapons = 0;
    for (int i = 0; (i < UNIT_ITEM_COUNT) && unit->items[i]; ++i){
        if(CanUnitUseWeapon(unit, GetItemIndex(unit->items[i]))){
            usableWeapons++;
        }
    }
    return usableWeapons;
}

int GetSlotOfNthUsableWeapon(struct Unit* unit, int n){
    for (int i = 0; (i < UNIT_ITEM_COUNT) && unit->items[i]; ++i){
        if(CanUnitUseWeapon(unit, GetItemIndex(unit->items[i]))){
            n--;
        }
        if(n < 0){
            return i;
        }
    }
    return -1; // err
}
