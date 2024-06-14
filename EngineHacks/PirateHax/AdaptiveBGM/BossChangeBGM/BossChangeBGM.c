#include "BossChangeBGM.h"

void BossChangeBGM() {
	//songID is the song that we're to be fading out
	
	//is does either battler have boss music?
	int i = 0;
	
	while (true) {
		//if the charID is this, it's the list terminator
		if (BossMusicTable[i].unitID == 0xFFFFFFFF) return;
		//if the charID is either of these, this entry is currently in use
		if (BossMusicTable[i].unitID == gBattleActor.unit.pCharacterData->number || BossMusicTable[i].unitID == gBattleTarget.unit.pCharacterData->number) break;
		i++;
	}

	Sound_PlaySongSmoothCommon(BossMusicTable[i].songID, 1, 0);
	
	SetEventId(BossMusicActiveFlagID);
	*BossMusicCurrentSongID = BossMusicTable[i].songID;
	
	;

}


//! FE8U = 0x08015FC8
int GetCurrentMapMusicIndexAddition(void) {
		//doing it this way because I don't want 2 source files
	asm("	.thumb;\
			push {r4-r7,r14};\
			ldr r0,=BossMusicActiveFlagID;\
			ldrh r0,[r0];\
			ldr r1,=CheckEventId;\
			mov r14,r1;\
			.short 0xF800;\
			cmp r0,#1;\
			bne DoNormalFunction;\
			ldr r0,=BossMusicCurrentSongID;\
			ldr r0,[r0];\
			ldrh r0,[r0];\
			pop {r4-r7};\
			pop {r1};\
			bx r1;\
			.ltorg;\
			.align;\
			DoNormalFunction: ;\
			mov r0,#4;\
			ldr r1,=CheckEventId;\
			mov r14,r1;\
			.short 0xF800;\
			ldr r1,=0x8015FD3;\
			bx r1;\
			.ltorg;\
			.align;\
	");
}

//the proc that handles the battle entrance animation with anims on
const ProcCode New_ekrBattleStarting[] = {
    PROC_SET_NAME("ekrBattleStarting"),
	PROC_CALL_ROUTINE(BossChangeBGM),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8055CF0),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8055DF4),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8055F00),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8055FA0),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8055FE8),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8056024),
    PROC_LOOP_ROUTINE(ekrBattleStarting_8056078),
    PROC_END
};

BossMusicEntry* GetCharBossMusicEntry(u8 charID) {
	int i = 0;
	while (true) {
		//if the unitID is this, it's the list terminator
		if (BossMusicTable[i].unitID == 0xFFFFFFFF) break;
		//if the unitID is charID, this entry is currently in use
		if (BossMusicTable[i].unitID == charID) {
			//turn off boss music flag
			return &BossMusicTable[i];
		}
		i++;
	}
	return 0;
}

//hooks the function that handles death quotes with anims on
//0x80835A8
void New_ShouldDisplayDeathQuoteForChar() {
	asm("	push {r4,r14};\
			mov r4,r0;\
			bl GetCharBossMusicEntry;\
			cmp r0,#0;\
			beq SkipBossFlag;\
			\
			ldr r0,=BossMusicActiveFlagID;\
			ldrh r0,[r0];\
			ldr r1,=UnsetEventId;\
			mov r14,r1;\
			.short 0xF800;\
			\
			SkipBossFlag:\
			mov r0,r4;\
			ldr r1,=0x80846E5;\
			mov r14,r1;\
			.short 0xF800;\
			ldr r1,=0x80835B5;\
			bx r1;\
	");
}

void New_MapAnimBattleQuoteHandler() {
	if (gMapAnimData.actorCount_maybe == 2) {
		
		BossMusicEntry* entry = GetCharBossMusicEntry(UNIT_CHAR_ID(gMapAnimData.actor[0].unit));
		if (entry) {
			SetEventId(BossMusicActiveFlagID);
			*BossMusicCurrentSongID = entry->songID;
			Sound_PlaySongSmoothCommon(entry->songID, 4, 0);
		}
		else {
			entry = GetCharBossMusicEntry(UNIT_CHAR_ID(gMapAnimData.actor[1].unit));
			if (entry) {
				SetEventId(BossMusicActiveFlagID);
				*BossMusicCurrentSongID = entry->songID;
				Sound_PlaySongSmoothCommon(entry->songID, 4, 0);
			}
		}
		
		
		CallBattleQuoteEventsIfAny(
			UNIT_CHAR_ID(gMapAnimData.actor[0].unit), UNIT_CHAR_ID(gMapAnimData.actor[1].unit));
	}
}

void New_MapAnimDeathQuoteHandler() {
	
	
	
	
    int actorNum = -1;
    switch (gMapAnimData.actorCount_maybe) {
    case 2:
        if (gMapAnimData.actor[1].hp_cur == 0)
            actorNum = 1;
        // fallthrough

    case 1:
        if (gMapAnimData.actor[0].hp_cur == 0)
            actorNum = 0;
        break;
    } // switch (gMapAnimData.actorCount_maybe)

    if (actorNum != -1) {
        int charid = UNIT_CHAR_ID(gMapAnimData.actor[actorNum].unit);

        switch (charid) {
        case 0x1: //lua
            if (CheckEventId(0x65)) // TODO: flag constants
                UnsetEventId(0x65); // TODO: flag constants
            break;
        }
		
		BossMusicEntry* entry = GetCharBossMusicEntry(charid);
		if (entry) {
			UnsetEventId(BossMusicActiveFlagID);
		}

        if (ShouldDisplayDeathQuoteForChar(charid)) {
            MapAnim_EndBattleInfoBox();
            DisplayDeathQuoteForChar(charid);
        }
    }
}
