#include "Dogfight.h"

u32 DogfightBonus(Unit* unit, u16 item, u32 minMaxRange) {
	//if we are gBattleTarget AND we have AS > opponent AS AND we have Dogfight, range is infinite
	if (
		( ( unit == &gBattleTarget ) || 
		  ( unit == &gBattleTarget.unit ) ) 
		  && (gBattleActor.battleSpeed < gBattleTarget.battleSpeed) ) 
			return (1 << 16 | 0xFF);
	return minMaxRange;
}

