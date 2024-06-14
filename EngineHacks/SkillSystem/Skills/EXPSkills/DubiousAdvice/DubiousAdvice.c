#include "DubiousAdvice.h"

int DubiousAdviceExp(int exp, struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	if(AuraSkillCheck(bunitA, DubiousAdviceID_Link, 0x1, 0x1)) {
		int newExp = (exp * (100 - DubiousAdviceReduction_Link)) / 100;
		return newExp;
	}
	return exp;
}

int DubiousAdviceGrowth(Unit* unit, int growth, int statIndex) {
	if(AuraSkillCheck(unit, DubiousAdviceID_Link, 0x1, 0x1)) {
		return growth + DubiousAdviceBonus_Link;
	}
	return growth;
}

void DubiousAdviceGrowthWrapper() {
  asm("    push {r14};\
        mov r0,r4;\
        mov r1,r5;\
        mov r2,r6;\
        bl DubiousAdviceGrowth;\
        mov r5,r0;\
        pop {r0};\
        bx r0;\
     ");
}