#include "gbafe.h"

#include "HubFlagChecks.h"

void CheckIfHubVisitedASMC(){
    bool visitedHub = CheckEventId(0x83) | CheckEventId(0x8B) | CheckEventId(0x8c) | CheckEventId(0x8d) | CheckEventId(0x8e) | CheckEventId(0x8f);
    if (visitedHub != 0){
        gEventSlot[0xC] = 1; //ORs all flags, if one is set then visitedHub = 1 and we have visited
    }
    else{
        gEventSlot[0xC] = 0; //if not, return 0
    }
}

void CheckIfHubChaptersCompletedASMC(){
    bool completedChapters = CheckEventId(0x83) & CheckEventId(0x8B) & CheckEventId(0x8c) & CheckEventId(0x8d) & CheckEventId(0x8e) & CheckEventId(0x8f);
    if (completedChapters != 0){
        gEventSlot[0xC] = 1; //ANDs all flags, if all are set then completedChapters = 1 
    }
    else{
        gEventSlot[0xC] = 0; //if not, return 0
    }
}

void SetHubChapterFlagASMC(){
    int flagId = gEventSlot[0x1];
    if (flagId == 1){
        SetEventId(0x83);
    }
    else if (flagId == 2){
        SetEventId(0x8B);
    }
    else if (flagId == 3){
        SetEventId(0x8c);
    }
    else if (flagId == 4){
        SetEventId(0x8d);
    }
    else if (flagId == 5){
        SetEventId(0x8e);
    }
    else if (flagId == 6){
        SetEventId(0x8f);
    }
    else{

    }
    UnsetEventId(0xA0); //unset this bc we are no longer in a hub
}

void UnsetAllHubChapterFlagsASMC(){
    UnsetEventId(0x83);
    UnsetEventId(0x8b);
    UnsetEventId(0x8c);
    UnsetEventId(0x8d);
    UnsetEventId(0x8e);
    UnsetEventId(0x8f);
}


bool LuaIsInHub(Unit* unit){
    if (CheckEventId(0xA0)){ 
        return true; //give 15 move
    }
    return false;
}

int ReturnNumberOfHubChaptersVisited(){
    int hubChapters = (CheckEventId(0x83) + CheckEventId(0x8B) + CheckEventId(0x8c) + CheckEventId(0x8d) + CheckEventId(0x8e) + CheckEventId(0x8f) - 1);
    
    if (gChapterData.chapterIndex == 1){ //in Hub A, add 1
        hubChapters++;
    }

    if (hubChapters <= 0){
        return 0;
    }
    else{
        return hubChapters;
    }
}

void CheckIfOneHubAChapterCompletedASMC(){
    int numOfChapters = ReturnNumberOfHubChaptersVisited();
    if (numOfChapters >= 1){
        gEventSlot[0xC] = 1;
    }
    else{
        gEventSlot[0xC] = 0;
    }
}

void CheckHowManyHubChaptersCompletedASMC(){
    gEventSlot[0xC] = ReturnNumberOfHubChaptersVisited();
}

void CalculateYodsenPriceASMC(){
    int basePrice = 3000;
    int discount = ReturnNumberOfHubChaptersVisited() * 500;
    gEventSlot[0xC] = basePrice - discount;
}

void GetTextFromMemorySlot1ASMC(){
    int text = gEventSlot[0x1];
    SetTalkNumber(text); //sets the talk number to be the value in s1
}