.thumb
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
.equ MiracleID, SkillTester+4
.equ d100Result, 0x802a52c

@ r0 is attacker, r1 is defender, r2 is current buffer, r3 is battle data
push {r4-r7,lr}
mov r4, r0 @attacker
mov r5, r1 @defender
mov r6, r2 @battle buffer
mov r7, r3 @battle data
ldr     r0,[r2]           @r0 = battle buffer                @ 0802B40A 6800     
lsl     r0,r0,#0xD                @ 0802B40C 0340     
lsr     r0,r0,#0xD        @Without damage data                @ 0802B40E 0B40     
@ mov r1, #0xC0 @skill flag @it's a passive, regardless of skills
@ lsl r1, #8 @0xC000
mov r1, #2 @miss
tst r0, r1
bne End

@first of all if the attack isn't lethal, we don't need to proc miracle
ldrb r1, [r5,#0x13] @current hp
mov r0, #4
ldsh r0, [r7, r0]
cmp r0, r1
blt End @not gonna die

@then we check if unit has the Miraclemaker bit set
push {r0-r3}
mov r0,r5
bl GetUnitDebuffEntry 
ldr r1, =MiraclemakerBitOffset_Link
ldr r1, [r1]
bl CheckBit
cmp	r0,#0
bne GrantMiraclemaker @if yes, immediately grant effect
pop {r0-r3}

@if we don't have Miraclemaker bit, check if hp <= 50%
ldrb r0, [r5,#0x12] @max hp
ldrb r1, [r5,#0x13] @current hp
cmp r1, #1 @1hp left?
ble End
lsr r0, #1 @max/2
cmp r1, r0
ble End

@lastly check that defender has Miracle
ldr r0,SkillTester
mov lr, r0
mov r0, r5 @defender data
ldr r1,MiracleID
.short 0xf800
cmp r0, #0
beq End
b GrantNormalMiracle

@and set damage to currhp-1
GrantMiraclemaker:
pop {r0-r3}
GrantNormalMiracle:
ldrb r0, [r5,#0x13] @currhp
sub r0, #1
strh r0, [r7,#4] @final damage

@finally, we unset the miraclemaker bit if it procced

@if this isn't a real battle, skip this part
ldrh r0,[r7] @battle stats config
mov r1,#1
and r0,r1
cmp r0,#0
beq End

mov r0,r5
bl GetUnitDebuffEntry
ldr r1, =MiraclemakerBitOffset_Link
ldr r1, [r1]
bl UnsetBit

End:
pop {r4-r7}
pop {r15}

.align
.ltorg
SkillTester:
@POIN SkillTester
@WORD MiracleID
