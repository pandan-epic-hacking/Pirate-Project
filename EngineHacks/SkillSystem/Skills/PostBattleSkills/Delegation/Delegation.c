#include "Delegation.h"

void ApplyDelegationBuff(Unit* unit) {
	
	if (AreAllegiancesAllied(gActiveUnit->index, unit->index)) {
		u32* entry = GetUnitDebuffEntry(unit);
		SetBit(entry, DelegationBitOffset_Link);
		StartBuffFx(unit, 0x1, 0);
	}
}

void Delegation() {
	Unit* unit = gActiveUnit;

	if((gActionData.unitActionType == UNIT_ACTION_COMBAT) && (gActionData.subjectIndex != gActionData.targetIndex) && SkillTester(unit, DelegationID_Link)) {
		BmMapFill(gMapRange, 0);
		MapIncInBoundedRange(unit->xPos, unit->yPos, GetItemMinRange(unit->items[0]), GetItemMaxRange(unit->items[0]));
		ForEachUnitInRange(ApplyDelegationBuff);
	}
}

void ClearDelegation() {
	Unit* unit = gActiveUnit;
	u32* entry = GetUnitDebuffEntry(unit);
	
	if((gActionData.unitActionType == UNIT_ACTION_COMBAT) && (gActionData.subjectIndex != gActionData.targetIndex) && CheckBit(entry, DelegationBitOffset_Link)) {
		UnsetBit(entry, DelegationBitOffset_Link);
	}
}
