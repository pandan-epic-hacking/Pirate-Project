#include "gbafe.h"

struct BossMusicEntry {
	u32 unitID;
	u32 songID;
} typedef BossMusicEntry;

struct DeathQuoteEntry {
    /* 00 */ u16 pid;
    /* 02 */ u8 route;
    /* 03 */ u8 chapter;
    /* 04 */ u16 flag;
    /* 06 */ u16 msg;
    /* 08 */ u32 event;
} typedef DeathQuoteEntry;

extern BossMusicEntry* BossMusicTable;
extern u16 BossMusicActiveFlagID;
extern u16* BossMusicCurrentSongID;

extern bool CheckEventId(u16 flag);
extern void SetEventId(u16 flag);
extern void UnsetEventId(u16 flag);
extern DeathQuoteEntry* GetCharDeathQuoteEntry(u8 charID);
extern void CallBattleQuoteEventsIfAny(u8 charID1, u8 charID2);
extern void DisplayDeathQuoteForChar(u8 charID);
extern bool ShouldDisplayDeathQuoteForChar(u8 charID);
extern void MapAnim_EndBattleInfoBox();

extern void ekrBattleStarting_8055CF0();
extern void ekrBattleStarting_8055DF4();
extern void ekrBattleStarting_8055F00();
extern void ekrBattleStarting_8055FA0();
extern void ekrBattleStarting_8055FE8();
extern void ekrBattleStarting_8056024();
extern void ekrBattleStarting_8056078();

#define UNIT_CHAR_ID(aUnit) ((aUnit)->pCharacterData->number)
#define STRUCT_PAD(from, to) unsigned char _pad_ ## from[(to) - (from)]

struct MapAnimActorState {
    /* 00 */ struct Unit * unit;
    /* 04 */ struct BattleUnit * bu;
    /* 08 */ struct MUProc * mu;
    /* 0C */ u8 hp_max;
    /* 0D */ u8 hp_cur;
    /* 0E */ u16 hp_displayed_q4;
    /* 10 */ u8 hp_info_x;
    /* 11 */ u8 hp_info_y;
    /* 12 */ STRUCT_PAD(0x12, 0x14);
};

struct MapAnimState {
    /* 00 */ struct MapAnimActorState actor[4];

    /* 50 */ struct BattleHit * pCurrentRound;
    /* 54 */ const struct ProcCmd * specialProcScr;
    /* 58 */ u8 subjectActorId;
    /* 59 */ u8 targetActorId;
    /* 5A */ u16 hitAttributes;
    /* 5C */ u8 hitInfo;
    /* 5D */ s8 hitDamage;
    /* 5E */ u8 actorCount_maybe;
    /* 5F */ u8 hp_changing;
    /* 60 */ u8 u60;
    /* 61 */ u8 u61;
    /* 62 */ u8 u62;
};

extern struct MapAnimState gMapAnimData;

