#include "gbafe.h"

//function prototypes
void ChapterPreviewASMC(Proc* parent);
void FancyChapterPreviewMenu_OnInit(MenuProc* proc);
void FancyChapterPreviewMenu_OnEnd(MenuProc* proc);
int FancyChapterPreviewMenu_OnHover(MenuProc* proc);
u8 FancyChapterPreviewMenu_SelectYes(MenuProc* menu, MenuCommandProc* item);
u8 FancyChapterPreviewMenu_SelectNo(MenuProc* menu, MenuCommandProc* item);
void SetMinimapPosition();
void DrawMinimap_BG2(int chapterId, u16* vram, int palId);
void DrawMinimapInternal_BG2(u16* vram, int palId);


//EA literals
extern MenuDefinition FancyChapterPreviewMenu_Def;
extern u8* TextDrawnFlagLoc_Link;

struct ChapterPreviewEntry {
	u16 chapterNameTextID;
	u16 objectiveTextID;
	u16 unitsTextID;
	u16 lootTextID;
	u16 chapterConfirmTextID;
	
} typedef ChapterPreviewEntry;

extern ChapterPreviewEntry ChapterPreviewTable[];

//stuff in the reference file that isn't defined
extern void DrawMinimap(int chapterId, u16* vram, int palId);
extern void BG_SetPosition(u16 bg, u16 x, u16 y);
extern void ApplyMinimapGraphics(int);
extern void InitMapForMinimap(int chapterId);
extern u16* GetMinimapTerrainCellAt(int x, int y);

#define TILEMAP_INDEX(aX, aY) (0x20 * (aY) + (aX))
#define PlaySoundEffect(id) \
    if (gChapterData.muteSfxOption) \
        m4aSongNumStart((id))
