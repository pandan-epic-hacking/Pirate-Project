#include "FortuneStaff.h"

bool IsFortuneBitSet(Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void SetFortuneBit(Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void UnsetFortuneBit(Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void FortuneHitBoost(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (IsFortuneBitSet(&bunitA->unit)) bunitA->battleHitRate += 30;
}

void ClearFortuneBitEachTurn() {
	//this runs in start of turn calc loop
	//for each unit of the current phase, look for one with the bit set
	
	int faction = gChapterData.currentPhase;
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
		Unit* curUnit = GetUnit(unitID);
		
		//clear Fortune bit
		UnsetFortuneBit(curUnit);
		
		unitID++;
	}	
}

void ExecFortuneStaff(Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetFortuneBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

void AddUnitToTargetListIfUnfortunate(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)
		&& !IsFortuneBitSet(unit)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

void MakeTargetListForFortune(Unit* unit, int item) {
	int x = unit->xPos;
	int y = unit->yPos;
	gUnitSubject = unit;
	
	BmMapFill(gMapRange, 0);
	MapIncInBoundedRange(x, y, GetItemMinRange(item), GetItemMaxRange(item));
	
	ForEachUnitInRange(AddUnitToTargetListIfUnfortunate);
}

void FortuneUsabilityWrapper() {
	asm("mov r0,r4;	\
		 mov r1,r5; \
		 bl FortuneUsability; \
		 pop {r4,r5}; \
		 pop {r1}; \
         bx r1; \
	");	
}

bool FortuneUsability(Unit* unit, u16 item) {
	MakeTargetListForFortune(unit, item);
	return GetTargetListSize() != 0;
}

void FortuneEffectWrapper() {
	asm(" 	mov r0,r6; \
			bl ExecFortuneStaff; \
			ldr r0,=#0x802FF77; \
			bx r0; \
	");
	
}

