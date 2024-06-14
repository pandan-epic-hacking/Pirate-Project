
s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleGetFollowUpOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
int GetBattleUnitHitCount(struct BattleUnit* attacker);
s8 BattleGenerateHit(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleRoll1RN(u16 threshold, s8 simResult);

int MapMain_SwitchPhases(void);
void ClearActiveFactionGrayedStates(void);
void SMS_UpdateFromGameData(void);
void SwitchGameplayPhase(void);
s8 RunPhaseSwitchEvents(void);

extern u8 CritthriftIDLink;