#include "Swift.h"

struct SelectInfo const SelectInfo_Swift =
{
    .onInit = SwiftSelection_OnInit,
    .onSwitchIn = SwiftSelection_OnChange,
    .onSelect = SwiftSelection_OnSelect,
    .onCancel = GenericSelection_BackToUM,
};

u8 SwiftUsability(const struct MenuItemDef* def, int number) {
	if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }
    if (gBmSt.unk3D & 1) {
        return MENU_NOTSHOWN;
    }
	if (SkillTester(gActiveUnit, SwiftID_Link) == 0){ // dont use not for some magic reason
		return MENU_NOTSHOWN;
	}
    MakeSwiftTargetList(gActiveUnit);
    if (GetSelectTargetCount() == 0) {
        return MENU_NOTSHOWN;
    }
    return MENU_ENABLED;
}

u8 SwiftEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
	MakeSwiftTargetList(gActiveUnit);
    NewTargetSelection(&SelectInfo_Swift);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

u8 SwiftSelection_OnSelect(ProcPtr proc, struct SelectTarget* target) {
    gActionData.unitActionType = UNIT_ACTION_TAKE; //this should do nothing
    gActionData.targetIndex = target->uid;
    UnitFinalizeMovement(GetUnit(gActionData.targetIndex));
    DoSwiftEffect(target->uid);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

void SwiftSelection_OnInit(ProcPtr menu) {
    ClearBg0Bg1();
    StartSubtitleHelp(menu, GetStringFromIndex(SwiftHelpText_Link));
    return;
}

u8 SwiftSelection_OnChange(ProcPtr proc, struct SelectTarget* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    return 0;
}

void DoSwiftEffect(int targetUid) {
	struct Unit* targetUnit = GetUnit(targetUid);
	u32* entry = GetUnitDebuffEntry(targetUnit);
	SetBit(entry, SwiftBitOffset_Link);
}

void MakeSwiftTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;
    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    ForEachAdjacentUnit(x, y, TryAddUnitToSwiftTargetList);
    return;
}

void TryAddUnitToSwiftTargetList(struct Unit* unit) {
	if (AreUnitsAllied(gSubjectUnit->index, unit->index) == 0) {
        return;
    }
    AddTarget(unit->xPos, unit->yPos, unit->index, 0);
    return;
}

long long SwiftBoost(u8 stat, struct Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), SwiftBitOffset_Link)) stat += SwiftBonus_Link;

	union {
		long long asLongLong;
		struct {
			u32 stat;
			struct Unit* unit;
		};
	} result;
	
	result.stat = stat;
	result.unit = unit;
	
	return result.asLongLong;
}

void SwiftStartOfTurn() {	
	int faction = gPlaySt.faction;
	int unitID = faction+1;
	int maxCount;
	
	switch (faction) {
		case UA_BLUE:
		maxCount = 62;
		break;
		
		case UA_RED:
		maxCount = 50;
		break;
		
		case UA_GREEN:
		maxCount = 20;
		break;
	}
	
	while ((unitID - faction) < maxCount) {
		//get the unit unitID
		struct Unit* curUnit = GetUnit(unitID);
		
		//are they on the map?
		if (IsUnitOnField(curUnit)) {
			u32* entry = GetUnitDebuffEntry(curUnit);
			UnsetBit(entry, SwiftBitOffset_Link);
		}
		unitID++;
	}	
}
