.thumb
.equ UnderhandedID, SkillTester+4

push {r4-r7, lr}
mov r4, r0 @attacker
mov r5, r1 @defender

@first we check that attacker has Underhanded
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @attacker data
ldr r1, UnderhandedID
.short 0xf800
cmp r0, #0
beq End

@okay now we check that attacker is at weapon triangle advantage
mov r0, #0x53
ldrb r0, [r4,r0] @this is the byte for attacker's wt hit bonus
cmp r0, #0x0
ble End @if wt hit bonus is <= 0, attacker doesn't have wta

@give crit
mov r0, #0x66
ldrh r1, [r4,r0]
add r1, #0x0F @add 15 crit
strh r1, [r4,r0]

@give damage = to enemy battle defense / 2
mov r0, #0x5A
ldrh r1, [r4,r0] @ loads attack of attacker
mov r2, #0x5C 
ldrh r2, [r5, r2] @ loads defense of defender
add r1, r2 
strh r1, [r4,r0] @ adds the second to the first and stores it

End:
pop {r4-r7, r15}
.align
.ltorg
SkillTester:
@Poin SkillTester
@WORD UnderhandedID
