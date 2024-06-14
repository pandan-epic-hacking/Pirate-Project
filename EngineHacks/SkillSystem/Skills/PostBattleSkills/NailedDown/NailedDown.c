#include "gbafe.h"

extern bool SkillTester(Unit* unit, u8 skillID);

extern u8 NailedDownID_Link;
extern u32 NailedDownBitOffset_Link;

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);

void NailedDown() {
	Unit* unit = gActiveUnit;
	
	if 	(	(gActionData.unitActionType == UNIT_ACTION_COMBAT) && 
			(gActionData.subjectIndex != gActionData.targetIndex) &&
			(SkillTester(unit,NailedDownID_Link))
		) {
			
		SetBit(GetUnitDebuffEntry(GetUnit(gActionData.targetIndex)), NailedDownBitOffset_Link);
		
	}
}

long long NailedDownMovPenalty(u8 stat, Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), NailedDownBitOffset_Link)) stat = 0;
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			Unit* unit;
		};
	} result;
	
	result.stat = stat;
	result.unit = unit;
	
	return result.asLongLong;
	
}

void ClearNailedDownPenalty() {
	int faction = gChapterData.currentPhase;
	if (faction == FACTION_BLUE) {
		faction = FACTION_RED;
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
			
			u32* entry = GetUnitDebuffEntry(curUnit);
			
			UnsetBit(entry, NailedDownBitOffset_Link);
			
			unitID++;
		}		
	}
}