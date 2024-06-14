#include "gbafe.h"

extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern u8 DelegationBitOffset_Link;

void NewComputeBattleUnitDefense(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (GetItemAttributes(defender->weapon) & IA_MAGICDAMAGE)
        attacker->battleDefense = attacker->terrainResistance + attacker->unit.res;
    else if (GetItemAttributes(defender->weapon) & IA_MAGIC)
        attacker->battleDefense = attacker->terrainResistance + attacker->unit.res;
	else if (CheckBit(GetUnitDebuffEntry(defender), DelegationBitOffset_Link)) {
		if (attacker->unit.def > attacker->unit.res)
			attacker->battleDefense = attacker->terrainResistance + attacker->unit.res;
		else
			attacker->battleDefense = attacker->terrainDefense + attacker->unit.def;
	}
    else
        attacker->battleDefense = attacker->terrainDefense + attacker->unit.def;
}
