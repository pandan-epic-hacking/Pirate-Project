@ Hook at 0xD11D4
@ If playing BGM is 0x3DD (Desire Below) and m4aMPlayVolumeControl is called at certain outside-battle points, mute tracks 3-15

.thumb

.equ MapBGM,						0x3006650 @ Start of second musicplayer, contains address to the map's BGM
.equ MusicID,						0x3DD @ Desire Below's ID, alter if necessary
.equ BGMSTRUCT,						0x2024E5C @ location of currently playing BGM (+0x4)
.equ InitBattle,					0x804FE15 @ NewEkrBattle's call to Determine_battle_BGM (indicates new battle has initiated)
.equ Exception1,					0x8002777 @ Call to our caller (Something sound related)
.equ Exception2,					0x80027C5 @ Call to our caller (Something sound related)

@.equ gpEkrBattleDeamon,			0x203E0F8
@.equ Delete6C,						0x8002D6C




push	{r14}

ldr		r0,=MapBGM
mov		r9,r0

@ Previously overwritten
@ldr		r0, =gpEkrBattleDeamon
@ldr		r0, [r0]
@bl		Delete6C

@ Previously overwritten
Loop:
mov		r0,r7
and		r0,r5
cmp		r0,#0x0
beq		NextIteration
ldrb	r3,[r1]
mov		r0,r8
and		r0,r3
cmp		r0,#0x0
beq		NextIteration			@ Only increase volume if track is present

@ Mute tracks if music to play is map BGM and BGM is Desire Below and track is 3 or ... or 15 and some exception happened

MapBGMCheck:
cmp		r4,r9
bne		AdjustAudio				@ Normal behaviour if sound/music argument is not the map BGM

ldr		r0,=BGMSTRUCT
ldrh	r0,[r0,#0x4] @r0 = the song we're looking for in the table
push 	{r1,r2}
ldr 	r1,CalmFlowList
LoopStart:
ldrh 	r2,[r1]
cmp  	r2,#0
beq  	AdjustAudioFromLoop @ Normal behaviour if not CalmFlow song
cmp  	r0,r2
beq  	LoopExit
add  	r1,r1,#4
b    	LoopStart

LoopExit:
ldrh 	r1,[r1,#2]
mov  	r2,#16
sub  	r1,r2,r1
mov  	r10,r1
pop 	{r1,r2}

cmp		r2,r10
bgt		NextIteration			@ Don't mute if track is in calm range

@ Keep tracks muted if BGM switches
ldr		r0,=InitBattle
mov		r11,r0
ldr		r0,[r13,#0x44]			
cmp		r0,r11					@ Is a new battle being initialized?
beq		Mute					@ Keep Flow tracks muted

@ ExCheck1
ExCheck1:
ldr		r0,=Exception1
mov		r11,r0
ldr		r0,[r13,#0x28]			@ caller's caller
cmp		r0,r11					@ Exception1
bne		ExCheck2				@ If Exception1 didn't happen, check whether Exception2 happened
b		Mute					@ Mute audio

@ ExCheck2
ExCheck2:
ldr		r0,=Exception2
mov		r11,r0
ldr		r0,[r13,#0x28]			@ caller's caller
cmp		r0,r11					@ Exception2
bne		AdjustAudio				@ If Exception2 didn't happen, apply regular behaviour

Mute:
mov		r6,#0x0					@ Mute audio
b AdjustAudio

AdjustAudioFromLoop:
pop {r1,r2}

AdjustAudio:
strb	r6,[r1,#0x13]
mov		r0,r12
orr		r0,r3
strb	r0,[r1]

NextIteration:
sub		r2,#0x1
add		r1,#0x50
lsl		r5,r5,#0x1
cmp		r2,#0x0
bgt		Loop

@return
pop		{r0}
bx		r0

.ltorg
.align

CalmFlowList:
@POIN CalmFlowList
