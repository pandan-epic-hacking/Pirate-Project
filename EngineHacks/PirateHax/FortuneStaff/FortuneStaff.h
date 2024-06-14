#include "gbafe.h"

//stuff defined in the reference file but not the headers
extern void BattleInitItemEffect(Unit* unit, u8 itemIndex);
extern void BattleInitItemEffectTarget(Unit* unit);
extern void BattleApplyItemEffect(Proc* proc);
extern void BeginBattleAnimations();

extern Unit* gUnitSubject;

//stuff defined in EA
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);

extern u32 FortuneStaffBitOffset_Link;

//function prototypes
bool IsFortuneBitSet(Unit* unit);
void SetFortuneBit(Unit* unit);
void UnsetFortuneBit(Unit* unit);
void FortuneHitBoost(BattleUnit* bunitA, BattleUnit* bunitB);
void ClearFortuneBitEachTurn();
void AddUnitToTargetListIfUnfortunate(Unit* unit);
void MakeTargetListForFortune(Unit* unit, int item);
void FortuneUsabilityWrapper();
bool FortuneUsability(Unit* unit, u16 item);

