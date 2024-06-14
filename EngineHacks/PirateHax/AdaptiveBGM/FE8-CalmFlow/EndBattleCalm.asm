@ Hook at 0x4FD90
@ At battle end, if MAP BGM is 0x3DD (Desire Below), mute tracks 3-15

.thumb

.equ MapBGM,						0x3006650 @ Start of second musicplayer, contains address to the map's BGM
.equ MusicID,						0x3DD @ Desire Below's ID, alter if necessary
.equ BGMSTRUCT,						0x2024E5C @ location of currently playing BGM (+0x4)

.equ gpEkrBattleDeamon,				0x203E0F8
.equ Delete6C,						0x8002D6D


@push	{r14}

@ Previously overwritten
ldr		r0,=gpEkrBattleDeamon
ldr		r0,[r0]
ldr		r1,=Delete6C
bl		Call_r1
b		Next
Call_r1:
bx		r1

Next:
@ Mute Flow tracks if BGM is Desire Below
ldr		r0,=BGMSTRUCT
ldrh	r0,[r0,#0x4]
ldr		r6,CalmFlowList
LoopStart:
ldrh    r1,[r6]
cmp     r1,#0
beq     Exit
cmp     r0,r1
beq     LoopExit
add     r6,r6,#4
b       LoopStart

LoopExit:
@Disable/Enable tracks for Flow version of song
ldr 	r0,=MapBGM
ldrh	r7,[r0,#0x8]	@trackcount
ldr 	r0,[r0,#0x2C]	@Address to track 0
mov		r1,r7			@trackiterator
ldrh    r6,[r6,#2]		@ channel offset
sub		r5,r7,r6		@ skip specified # of tracks
	
Loop:
	cmp		r1,r5
	bgt		NextIteration
	
	ldrb	r2,[r0]
	mov		r3,#0x80
	and		r3,r2
	cmp		r3,#0x0
	beq		NextIteration	@Only increase volume if track is present
	
	mov		r3,#0x3			@Indicate sound level needs to be adjusted
	orr		r2,r3
	strb	r2,[r0]
	
	mov		r3,#0x0			@Adjust to level 0
	strb	r3,[r0,#0x13]
	
	NextIteration:
	sub		r1,#0x1
	add		r0,#0x50		@Next track
	cmp		r1,#0x0
bgt		Loop

Exit:
pop {r0}
bx r0

.ltorg
.align

CalmFlowList:
@POIN CalmFlowList
