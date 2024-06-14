@ Hook at 0xD0414
@ If playing BGM is 0x3DD (Desire Below) leave tracks 3-15 muted

.thumb

.equ MapBGM,						0x3006650 @ Start of second musicplayer, contains address to the map's BGM
.equ MusicID,						0x3DD @ Desire Below's ID, alter if necessary
.equ BGMSTRUCT,						0x2024E5C @ location of currently playing BGM (+0x4)

push	{r14}

@ Previously overwritten
mov		r0,#0x2
strb	r0,[r4,#0xF]

@ Mute tracks if music to play is map BGM and BGM is Desire Below and track is 3 or ... or 15
ldr		r0,[r13,#0x4]	@ load musicplayer (if this is 0x03006650, then map BGM is used)
ldr		r1,=MapBGM
cmp		r0,r1
bne		NoMute			@ Don't mute if sound/music argument is not the map BGM
ldr		r0,=BGMSTRUCT
ldrh	r0,[r0,#0x4]
ldr		r1,CalmFlowList
LoopStart:
ldrh    r2,[r1]
cmp     r2,#0
beq     NoMute @don't mute if not adaptive bgm
cmp     r0,r2
beq     LoopExit
add     r1,r1,#4
b       LoopStart

LoopExit:
ldrh	r2,[r1,#2]
mov		r1,#16
sub     r1,r1,r2
cmp		r5,r1
ble		Mute			@ Mute if track is in flow range

NoMute:
strb	r6,[r4,#0x13]
b		EndIf

Mute:
mov		r0,#0x0
strb	r0,[r4,#0x13]

EndIf:

@ Previously overwritten
mov		r0,#0x16
strb	r0,[r4,#0x19]
mov		r1,r4

@return
pop		{r0}
bx		r0

.ltorg
.align

CalmFlowList:
@POIN CalmFlowList
