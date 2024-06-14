#include "gbafe.h"

extern bool AuraSkillCheck(Unit* unit, int skillID, int allyOption, int maxRange);

extern u8 AnalyzeID_Link;
extern u8 AnalyzeRange_Link;
extern s8 WTHitBonus_Link;
extern s8 WTMtBonus_Link;

void Analyze(BattleUnit* bunitA, BattleUnit* bunitB) {
	if(AuraSkillCheck(&bunitA->unit, AnalyzeID_Link, 0x3, AnalyzeRange_Link)) {
		bunitA->wTriangleHitBonus = -1 * WTHitBonus_Link;
		bunitA->wTriangleDmgBonus = -1 * WTMtBonus_Link;
	}
	if(AuraSkillCheck(&bunitB->unit, AnalyzeID_Link, 0x3, AnalyzeRange_Link)) {
		bunitB->wTriangleHitBonus = -1 * WTHitBonus_Link;
		bunitB->wTriangleDmgBonus = -1 * WTMtBonus_Link;
	}
}
