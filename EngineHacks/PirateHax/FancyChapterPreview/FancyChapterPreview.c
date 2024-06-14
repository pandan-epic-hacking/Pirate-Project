#include "FancyChapterPreview.h"

//this is an ASMC that will draw on the screen a preview of a chapter whose
//ID is given in s1, including a yes/no prompt
//the result of that prompt will be returned in sC
//this will be implemented as as menu, with the OnInit function
//drawing the preview itself

void ChapterPreviewASMC(Proc* parent) {
	//this ASMC starts a menu proc as a blocking child of the parent proc	
	StartMenuChild(&FancyChapterPreviewMenu_Def, parent);
		
}

void FancyChapterPreviewMenu_OnInit(MenuProc* proc) {
	//this function draws the minimap preview only
	u8 chID = gEventSlot[0x1];
	
	//draw the target chapter minimap (uses bg0, bg1)
	DrawMinimap_BG2(chID, (u16*)0x06004000, 3); //uses palette BGP3
	SetMinimapPosition();
	
	*TextDrawnFlagLoc_Link = 0;
	
	//make bg2, bg3 slightly transparent so they're darker
	SetColorEffectsFirstTarget(0, 0, 1, 1, 0);
    SetColorEffectsSecondTarget(0, 0, 1, 1, 1);

	//SetColorEffectBackdropSecondTarget(1);

	SetColorEffectsParameters(3, 16, 0, 8);

	gLCDIOBuffer.blendControl.target1_enableBg2 = 1;
    gLCDIOBuffer.blendControl.target1_enableBg3 = 1;
    gLCDIOBuffer.blendControl.target2_backdrop = 1;
}

int FancyChapterPreviewMenu_OnHover(MenuProc* proc) {		
	//this function draws all the BG0 stuff
	if (!gChapterData.muteSfxOption) m4aSongNumStart(0x66);

	if (*TextDrawnFlagLoc_Link == 1) return 0;
	*TextDrawnFlagLoc_Link = 1;

	u8 chID = gEventSlot[0x1];
	ChapterPreviewEntry entry = ChapterPreviewTable[chID];
	
	//prep strings from table
		
	TextHandle chNameHandle = {
		.tileIndexOffset = gpCurrentFont->tileNext,
		.tileWidth = 16
		};
		
	const char* textID1 = GetStringFromIndex(entry.chapterNameTextID);
	Text_Clear(&chNameHandle);
	Text_DrawString(&chNameHandle, textID1);
	Text_Display(&chNameHandle, &gBg0MapBuffer[TILEMAP_INDEX(6,0)]);
	
	TextHandle objectiveHandle = {
		.tileIndexOffset = gpCurrentFont->tileNext+16,
		.tileWidth = 24
		};
	
	const char* textID2 = GetStringFromIndex(entry.objectiveTextID);
	Text_Clear(&objectiveHandle);
	Text_DrawString(&objectiveHandle, textID2);
	Text_Display(&objectiveHandle, &gBg0MapBuffer[TILEMAP_INDEX(1,4)]);
	
	TextHandle unitsHandle = {
		.tileIndexOffset = gpCurrentFont->tileNext+40,
		.tileWidth = 24
		};
	
	const char* textID3 = GetStringFromIndex(entry.unitsTextID);
	Text_Clear(&unitsHandle);
	Text_DrawString(&unitsHandle, textID3);
	Text_Display(&unitsHandle, &gBg0MapBuffer[TILEMAP_INDEX(1,8)]);
	
	TextHandle lootHandle = {
		.tileIndexOffset = gpCurrentFont->tileNext+64,
		.tileWidth = 24
		};
	
	const char* textID4 = GetStringFromIndex(entry.lootTextID);
	Text_Clear(&lootHandle);
	Text_DrawString(&lootHandle, textID4);
	Text_Display(&lootHandle, &gBg0MapBuffer[TILEMAP_INDEX(1,12)]);
	
	TextHandle confirmHandle = {
		.tileIndexOffset = gpCurrentFont->tileNext+88,
		.tileWidth = 12
		};
	
	const char* textID5 = GetStringFromIndex(entry.chapterConfirmTextID);
	Text_Clear(&confirmHandle);
	Text_DrawString(&confirmHandle, textID5);
	Text_Display(&confirmHandle, &gBg0MapBuffer[TILEMAP_INDEX(11,16)]);
	
	return 0;
}
	


void FancyChapterPreviewMenu_OnEnd(MenuProc* proc) {
	//this function removes the entire chapter preview window
	
	//makes more sense to do this via events after the ASMC tbh 
	//so this function will be blank for now
}



u8 FancyChapterPreviewMenu_SelectYes(MenuProc* menu, MenuCommandProc* item) {
	//Yes selected, store true to sC
	gEventSlot[0xC] = 1;
	//end menu w/ confirm sound
	return ME_END | ME_PLAY_BEEP;
}



u8 FancyChapterPreviewMenu_SelectNo(MenuProc* menu, MenuCommandProc* item) {
	//No selected, store false to sC
	gEventSlot[0xC] = 0;
	//end menu w/ cancel sound
	return ME_END | ME_PLAY_BOOP;	
}




void SetMinimapPosition() {
	s16 xOffset = gEventSlot[0xB] & 0xFFFF;
	s16 yOffset = gEventSlot[0xB] >> 16;
	
	if (xOffset == -1) xOffset = 80;
	if (yOffset == -1) yOffset = -16;
	
    int x = (240 - (gMapSize.x * 4) + xOffset) >> 1;
    int y = (160 - (gMapSize.y * 4) + yOffset) >> 1;

    BG_SetPosition(2, -x, -y);

}



void DrawMinimap_BG2(int chapterId, u16* vram, int palId) {
    FillBgMap(gBg2MapBuffer, 0);

    InitMapForMinimap(chapterId);
    ApplyMinimapGraphics(palId);
    DrawMinimapInternal_BG2(vram, palId);

	EnableBgSyncByMask(0xFF);
    return;
}

void DrawMinimapInternal_BG2(u16* vram, int palId) {
    int iy;
    int ix;
    int chr;

    if (vram == 0) {
        vram = (void*)(0x06000000 + 0x20);
    }

    chr = ((u32)vram << 15) >> 20;

    if (palId < 0) {
        palId = 3;
    }

    for (iy = 0; iy < gMapSize.y; iy += 2) {
        for (ix = 0; ix < gMapSize.x; ix += 2) {
            u16* iterA = GetMinimapTerrainCellAt(ix, iy);
            u16* iterB = GetMinimapTerrainCellAt(ix + 1, iy);

            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;

            iterA = GetMinimapTerrainCellAt(ix, iy + 1);
            iterB = GetMinimapTerrainCellAt(ix + 1, iy + 1);

            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;
            *vram++ = *iterA;
            iterA += 2;
            *vram++ = *iterB;
            iterB += 2;

            gBg2MapBuffer[TILEMAP_INDEX((ix / 2), (iy / 2))] = TILEREF(chr, palId);

            chr++;
			
			//the rest of this pertains to drawing the dots over the map
			//which we don't want here (it would draw the wrong stuff)

/*             if ((gBmMapUnit[iy  ][ix  ] != 0) ||
                (gBmMapUnit[iy  ][ix+1] != 0) ||
                (gBmMapUnit[iy+1][ix  ] != 0) ||
                (gBmMapUnit[iy+1][ix+1] != 0)) {

                iterA = GetMinimapObjectCellAt(ix, iy);
                iterB = GetMinimapObjectCellAt(ix + 1, iy);

                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;

                iterA = GetMinimapObjectCellAt(ix, iy + 1);
                iterB = GetMinimapObjectCellAt(ix + 1, iy + 1);

                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;
                *vram++ = *iterA;
                iterA += 2;
                *vram++ = *iterB;
                iterB += 2;

                gBG0TilemapBuffer[TILEMAP_INDEX((ix / 2), (iy / 2))] = TILEREF(chr, (palId + 1));

                chr++;
            } */

        }
    }

    return;
}

