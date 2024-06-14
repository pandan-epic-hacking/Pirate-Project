
    .thumb
    .align
    .equ EyeForAnEyeID, SkillTester+4

    @ r0 = attacker
    @ r1 = defender
    @ r2 = current buffer (battle hits)
    @ r3 = battle data (gBattleStats)

    @ this saves the damage taken by the defender into unit->support_exp[5]
    @ this should probably be last in proc loop
    @ I would have this be somewhere in BattleGenerateHitEffects but Break breaks(lol) any hooks in it

    push {r4-r5, lr}

    mov  r4, r1 @ var r4 = defender
    mov  r5, r3 @ var r5 = gBattleStats

    @ end if miss
    ldr  r1, [r2] @ r2 = battle hit
    mov  r2, #2
    tst  r1, r2
    bne  save_end

    @ check for skill just in case supports_exp[5] is also used for something else for other exclusive skills

    ldr  r1, EyeForAnEyeID @ arg r1 = skill id
    mov  r0, r4   @ arg r0 = unit

    ldr  r3, SkillTester
    mov  lr, r3
    .short 0xF800  @ bl lr

    cmp  r0, #0
    beq  save_end

    @ get damage for this hit
    mov  r0, #4
    ldsh r0, [r5, r0]

    @ check for zero/negative damage
    cmp  r0, #0
    ble  save_end

    @ get current saved damage bonus
    mov  r2, #0x37
    @ ldrb r1, [r4, r2]

    @ compare with existing damage bonus
    @ cmp  r1, r0
    @ bge  save_end

    @ store new hit
    lsr r0, #1 @ divides it by 2
    strb r0, [r4, r2]

save_end:
    pop  {r4-r5}
    pop  {r0}
    bx   r0

.align
.ltorg

SkillTester:
@ POIN SkillTester
@ WORD EyeForAnEyeID
