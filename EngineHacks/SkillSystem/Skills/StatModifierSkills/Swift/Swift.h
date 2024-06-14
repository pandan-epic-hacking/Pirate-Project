#include "gbafe.h"

extern u8 SwiftID_Link;
extern u16 SwiftHelpText_Link;
extern int SkillTester(struct Unit* unit, int id); 

extern struct SelectInfo const SelectInfo_Swift;
u8 SwiftUsability(const struct MenuItemDef* def, int number);
u8 SwiftEffect(struct MenuProc* menu, struct MenuItemProc* menuItem);
u8 SwiftSelection_OnSelect(ProcPtr proc, struct SelectTarget* target);
void SwiftSelection_OnInit(ProcPtr menu);
u8 SwiftSelection_OnChange(ProcPtr proc, struct SelectTarget* target);
void DoSwiftEffect(int targetUid);
void MakeSwiftTargetList(struct Unit* unit);
void TryAddUnitToSwiftTargetList(struct Unit* unit);

enum {
	UA_BLUE = 0x00,
	UA_GREEN = 0x40,
	UA_RED = 0x80,
};

extern int SwiftBitOffset_Link;
extern int SwiftBonus_Link;

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);
