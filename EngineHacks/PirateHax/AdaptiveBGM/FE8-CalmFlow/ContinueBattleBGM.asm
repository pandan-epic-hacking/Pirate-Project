@
@@Call  08073F4C    FE8J
@Call 08071A68    FE8U
@
@r0 Nazo
@
@
@r4 Change SongID
@
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm

.thumb

.equ MapBGM,						0x3006650 @ Start of second musicplayer, contains address to the map's BGM
.equ MusicID,						0x3DD @ Desire Below's ID, alter if necessary
.equ BossBGMFunction, CalmFlowList+4

@only the value in r0 is needed, r1-r3 are not
@let's pass it to another function that will return the relevant thing

@ldr r3, BossBGMFunction
@mov lr, r3
@.short 0xf800


@ldr   r3,=0x02024E5C   @ FE8J (BGMSTRUCT@BGM.音楽関係のフラグ1 )
ldr   r3,=0x02024E5C  @ FE8U (BGMSTRUCT@BGM.音楽関係のフラグ1 )

ldrh  r3,[r3,#0x4]   @      (BGMSTRUCT@BGM.再生しているBGM )
cmp   r3,r4
bne   SwitchBGM

ldr	  r4,CalmFlowList
LoopStart:
ldrh  r0,[r4]
cmp   r0,#0
beq   Exit
cmp   r3,r0
beq   LoopEnd
add   r4,r4,#4
b     LoopStart

LoopEnd:
ldrh  r6,[r4,#2] @r6 = # of tracks to avoid



	@Disable/Enable tracks for Flow version of song
	ldr 	r0,=MapBGM
	ldrh	r4,[r0,#0x8]	@trackcount
	ldr 	r0,[r0,#0x2C]	@Address to track 0
	mov		r1,r4			@trackiterator
	ldrh    r3,[r3,#2]		@ channel offset
	sub		r5,r4,r6		@apply # of tracks to avoid
	
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
	
	mov		r3,#0x40		@Adjust to level 40
	strb	r3,[r0,#0x13]
	
	NextIteration:
	sub		r1,#0x1
	add		r0,#0x50		@Next track
	cmp		r1,#0x0
	bgt		Loop
	
	b	Exit


	SwitchBGM:
	@@mov r0 ,r2      @不要
	@blh 0x0800223c    @FE8J SomethingSoundRelated_80022EC
	blh	0x080022ec   @FE8U SomethingSoundRelated_80022EC

	mov r0 ,r4
	@blh 0x08002570    @FE8J
	blh 0x08002620   @FE8U

Exit:
pop {r4}
pop {r0}
bx r0

.ltorg
.align

CalmFlowList:
@POIN CalmFlowList
@POIN BossBGMFunction

