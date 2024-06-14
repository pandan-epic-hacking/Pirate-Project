.thumb
.align

.global battle_changer
.type battle_changer, %function

.equ GetROMChapterStruct, 0x8034619
.equ CheckEventId, 0x8083DA9

.macro blh to, reg=r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

@hook at 80726CA
battle_changer:
push {r0-r3}
ldr r0,=BossMusicActiveFlagID
ldrh r0,[r0]
blh CheckEventId
cmp r0,#0
bne RetBossMusic
pop {r0-r3}

mov r0, #0x4	@ let's check for flag 4 being set
blh CheckEventId
cmp r0,#0
beq NormalMusicCheck @ if it isn't set, just do the normal check

Flag4Check:
ldr r6, =0x202BCF0 @chapter data
ldrb r0,[r6,#0xE] @chapter ID
blh GetROMChapterStruct
add r0,#0x28 @offset of player phase music
ldrb r1,[r6,#0xF] @current phase

cmp r1,#0
bne CheckEnemyPhaseFlag4
ldrh r6,[r0] @load player phase battle bgm
b retNormal

CheckEnemyPhaseFlag4:
cmp r1,#0x80
bne OtherPhaseFlag4
ldrh r6,[r0,#2] @load enemy phase battle bgm
b retNormal

OtherPhaseFlag4:
ldrb r6,[r0,#0x4] @load npc phase battle bgm, used ldrb here because the offset is invalid otherwise
b retNormal

NormalMusicCheck:
ldr r6, =0x202BCF0 @chapter data
ldrb r0,[r6,#0xE] @chapter ID
blh GetROMChapterStruct
add r0,#28 @offset of player phase music
ldrb r1,[r6,#0xF] @current phase

cmp r1,#0
bne CheckEnemyPhase
ldrh r6,[r0] @load player phase battle bgm
b retNormal

CheckEnemyPhase:
cmp r1,#0x80
bne OtherPhase
ldrh r6,[r0,#2] @load enemy phase battle bgm
b retNormal

OtherPhase:
ldrh r6,[r0,#4] @load npc phase battle bgm
b retNormal

.ltorg
.align

RetBossMusic:
pop {r0-r3}
ldr r6,=BossMusicCurrentSongID
ldr r6,[r6]
ldrh r6,[r6]

retNormal:
ldr r0,=0x80726E3
bx r0

.ltorg
.align
