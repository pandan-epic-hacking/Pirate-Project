.thumb
.align

.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
.equ SeaShantyID, AuraSkillCheck+4

push	{lr}

@check if dead
ldrb	r0, [r4,#0x13]
cmp	r0, #0x00
beq	End

@check if waited
ldrb  r0, [r6,#0x11]  @action taken this turn
cmp r0, #0x1
beq End

@check if adjacent ally has Sea Shanty
ldr r0, AuraSkillCheck
mov lr, r0
mov r0, r4 @attacker
ldr r1, SeaShantyID
mov r2, #0 @units are of same allegiance
mov r3, #1 @max range of receiving bonus
.short 0xf800
cmp r0, #0
beq End

@ check for no move, if so don't do it
ldr	r0,=0x8019224	@mov getter
mov	lr, r0
mov	r0, r4
.short	0xF800
cmp r0, #0x2
blt End

SeaShantySkill:
@give unit ability to move after combat
ldr	r0, [r4,#0x0C]	@status bitfield
mov	r1, #0x02
mvn	r1, r1
and	r0, r1		@unset bit 0x2
mov	r1, #0x40	@set canto bit
orr	r0, r1
str	r0, [r4,#0x0C]

@canto amount = unit's move - how much they moved, so we change how much they moved to (unit's move - 2).
ldr	r0,=0x8019224	@mov getter
mov	lr, r0
mov	r0, r4
.short	0xF800
sub r0, #0x2

cmp r0, #0x0
bge BeforeEnd
mov r0, #0x0		@if r0 is LESS than 0, we set it to 0 so we're not passing in negative mov

BeforeEnd:
strb r0, [r6,#0x10]

End:
pop	{r0}
bx	r0
.ltorg
.align
AuraSkillCheck:
@POIN SkillTester
@WORD GridmasterID