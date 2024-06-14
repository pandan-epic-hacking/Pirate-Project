#include "FrigidAura.h"

void FrigidAura() {
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
		Unit* curUnit = GetUnit(unitID);
		
		if (IsUnitOnField(curUnit)) {
			if (AuraSkillCheck(curUnit, FrigidAuraID_Link, 0x4, 0x1)) {
				curUnit->curHP = curUnit->curHP - (curUnit->maxHP * FrigidAuraDamage_Link / 100);
				if (curUnit->curHP <= 0) curUnit->curHP = 1;
			}
		}
		unitID++;
	}
}