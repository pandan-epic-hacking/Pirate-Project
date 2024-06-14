.thumb
.align

.global MakeMusicAvastASMC
.type MakeMusicAvastASMC, %function

.global MakeMusicAhoyASMC
.type MakeMusicAhoyASMC, %function

.global EnableMusicTracksASMC
.type EnableMusicTracksASMC, %function

.global DisableMusicTracksASMC
.type DisableMusicTracksASMC, %function

.macro blh to, reg=r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.equ gMPlayInfo_BGM2, 	0x3006650 @loc of map music player info
.equ sSoundStatus, 		0x2024E5C 	@loc of currently playing song IDs
.equ gEventSlot2, 		0x30004C0

MakeMusicAvastASMC:
@ we are enabling tracks according to the CalmFlow list entry for the current map music
push {r4-r7,r14}

ldr  r7,=CalmFlowSongList
ldr  r6,=sSoundStatus
ldrh r6,[r6,#4] @song ID

@find song ID's entry in CalmFlowSongList

MakeAvast_LoopStart:
ldrh r0,[r7] @song ID
cmp r0,#0
beq MakeAvast_End @not on the list, so do nothing
cmp r0,r6
beq MakeAvast_LoopExit @if the same, exit loop
add r7,#4
b MakeAvast_LoopStart

.ltorg
.align

MakeAvast_LoopExit:
ldrh r5,[r7,#2] @ # of tracks to skip

ldr  r0,=gMPlayInfo_BGM2
ldrh r4,[r0,#0x8]	@ # of tracks in song
mov  r1,r4
ldr  r0,[r0,#0x2C]  @ address of track 0
sub  r5,r4,r5 		@# of tracks to enable -> r5

MakeAvast_Loop2Start:
cmp	 r1,r5
bgt	 MakeAvast_LoopReset
ldrb r2,[r0]
mov	 r3,#0x80
and	 r3,r2
cmp	 r3,#0x0
beq	 MakeAvast_LoopReset

mov		r3,#0x3			@Indicate sound level needs to be adjusted
orr		r2,r3
strb	r2,[r0]

mov		r3,#0x40		@Adjust to level 40
strb	r3,[r0,#0x13]

MakeAvast_LoopReset:
sub		r1,#1
add		r0,#0x50		@Next track
cmp		r1,#0
bgt		MakeAvast_Loop2Start

MakeAvast_End:
pop {r4-r7}
pop {r0}
bx r0

.ltorg
.align

MakeMusicAhoyASMC:
@ we are disabling tracks according to the CalmFlow list entry for the current map music
push {r4-r7,r14}

ldr  r7,=CalmFlowSongList
ldr  r6,=sSoundStatus
ldrh r6,[r6,#4] @song ID

@find song ID's entry in CalmFlowSongList

MakeAhoy_LoopStart:
ldrh r0,[r7] @song ID
cmp r0,#0
beq MakeAhoy_End @not on the list, so do nothing
cmp r0,r6
beq MakeAhoy_LoopExit @if the same, exit loop
add r7,#4
b MakeAhoy_LoopStart

.ltorg
.align

MakeAhoy_LoopExit:
ldrh r5,[r7,#2] @ # of tracks to skip

ldr  r0,=gMPlayInfo_BGM2
ldrh r4,[r0,#0x8]	@ # of tracks in song
mov  r1,r4
ldr  r0,[r0,#0x2C]  @ address of track 0
sub  r5,r4,r5 		@# of tracks to enable -> r5

MakeAhoy_Loop2Start:
cmp	 r1,r5
bgt	 MakeAhoy_LoopReset
ldrb r2,[r0]
mov	 r3,#0x80
and	 r3,r2
cmp	 r3,#0x0
beq	 MakeAhoy_LoopReset

mov		r3,#0x3			@Indicate sound level needs to be adjusted
orr		r2,r3
strb	r2,[r0]

mov		r3,#0			@Adjust to level 0
strb	r3,[r0,#0x13]

MakeAhoy_LoopReset:
sub		r1,#1
add		r0,#0x50		@Next track
cmp		r1,#0
bgt		MakeAhoy_Loop2Start

MakeAhoy_End:
pop {r4-r7}
pop {r0}
bx r0

.ltorg
.align


EnableMusicTracksASMC:
push {r4-r7,r14}
ldr r5,=gEventSlot2
ldr r5,[r5] @ # of tracks to skip

ldr  r0,=gMPlayInfo_BGM2
ldrh r4,[r0,#0x8]	@ # of tracks in song
mov  r1,r4
ldr  r0,[r0,#0x2C]  @ address of track 0
sub  r5,r4,r5 		@# of tracks to enable -> r5

EnableTracks_LoopStart:
cmp	 r1,r5
bgt	 EnableTracks_LoopReset
ldrb r2,[r0]
mov	 r3,#0x80
and	 r3,r2
cmp	 r3,#0x0
beq	 EnableTracks_LoopReset

mov		r3,#0x3			@Indicate sound level needs to be adjusted
orr		r2,r3
strb	r2,[r0]

mov		r3,#0x40		@Adjust to level 40
strb	r3,[r0,#0x13]

EnableTracks_LoopReset:
sub		r1,#1
add		r0,#0x50		@Next track
cmp		r1,#0
bgt		EnableTracks_LoopStart

EnableTracks_End:
pop {r4-r7}
pop {r0}
bx r0

.ltorg
.align

DisableMusicTracksASMC:
push {r4-r7,r14}
ldr r5,=gEventSlot2
ldr r5,[r5] @ # of tracks to skip

ldr  r0,=gMPlayInfo_BGM2
ldrh r4,[r0,#0x8]	@ # of tracks in song
mov  r1,r4
ldr  r0,[r0,#0x2C]  @ address of track 0
sub  r5,r4,r5 		@# of tracks to enable -> r5

DisableTracks_LoopStart:
cmp	 r1,r5
bgt	 DisableTracks_LoopReset
ldrb r2,[r0]
mov	 r3,#0x80
and	 r3,r2
cmp	 r3,#0x0
beq	 DisableTracks_LoopReset

mov		r3,#0x3			@Indicate sound level needs to be adjusted
orr		r2,r3
strb	r2,[r0]

mov		r3,#0x40		@Adjust to level 40
strb	r3,[r0,#0x13]

DisableTracks_LoopReset:
sub		r1,#1
add		r0,#0x50		@Next track
cmp		r1,#0
bgt		DisableTracks_LoopStart

DisableTracks_End:
pop {r4-r7}
pop {r0}
bx r0

.ltorg
.align

