#include "gbafe.h"

extern int SkillTester(struct Unit* unit, int id); 

extern int DelegationID_Link;
extern int DelegationBitOffset_Link;

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);

extern void StartBuffFx(Unit* unit, u8 rallyBits, u8 range);