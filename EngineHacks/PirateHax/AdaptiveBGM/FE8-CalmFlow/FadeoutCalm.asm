@ Hook at 0xD0AF8
@ If playing BGM is 0x3DD (Desire Below) don't fade out tracks 3-15.

.thumb

.equ MapBGM,						0x3006650 @ Start of second musicplayer, contains address to the map's BGM
.equ MusicID,						0x3DD @ Desire Below's ID, alter if necessary
.equ BGMSTRUCT,						0x2024E5C @ location of currently playing BGM (+0x4)

push	{r14}

@ Previously overwritten
Loop:
ldrb	r1,[r4]
mov		r0,r3
and		r0,r1
cmp		r0,#0x0
beq		NextIteration

@ Don't fade out tracks if music to play is map BGM and BGM is Desire Below and track is 2 or ... or 15.
ldr		r0,=MapBGM
cmp		r6,r0
bne		Fadeout			@ Fade-out if sound/music argument is not the map BGM or...
ldr		r0,=BGMSTRUCT
ldrh	r0,[r0,#0x4]
ldr		r7,CalmFlowList
push    {r1}
LoopStart:
ldrh	r1,[r7]
cmp		r1,#0
beq		FadeoutFromLoop @ Fade-out if BGM doesn't use this
cmp     r0,r1
beq     LoopExit
add     r7,r7,#4
b       LoopStart

LoopExit:
pop     {r1}
ldrh    r7,[r7,#2]
sub     r7,r7,#16
neg     r7,r7
cmp		r5,r7		
bgt		Fadeout			@ Fade-out if track is in the calm range
b		NextIteration

FadeoutFromLoop:
pop {r1}

Fadeout:
ldrh	r7,[r6,#0x28]
lsr		r0,r7,#0x2
strb	r0,[r4,#0x13]
mov		r0,r1
orr		r0,r2
strb	r0,[r4]

NextIteration:
sub		r5,#0x1
add		r4,#0x50
cmp		r5,#0x0
bgt		Loop

@return
pop		{r0}
bx		r0

.ltorg
.align

CalmFlowList:
@POIN CalmFlowList
