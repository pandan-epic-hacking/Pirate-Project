.thumb

.macro _blh to, reg=r3
	ldr \reg, =\to
	mov lr, \reg
	.short 0xF800
.endm

@arguments:
	@r0: unit pointer
	@r1: item id
	@r2: min max range word
@retuns
	@r0: updated min max range word
.set GetWeaponType, 0x8017548

push 	{lr}
add 	sp, #-0x4
str 	r2, [sp]

ldr     r3,=0x203a4ec @attacker
ldr	r3, [r3]
ldrb	r3, [r3, #0x4]
ldr	r0, [r0]
ldrb	r0, [r0, #0x4]
cmp     r0,r3
bne     End @skip if unit isn't the attacker

mov 	r0, r1

@check that unit is wielding a bow
_blh GetWeaponType
cmp 	r0, #0x3 @bow
bne End



@get number of spaces moved
ldr r0,=0x203a968
ldrb r0,[r0]

@if value is obscene, read from a different byte
ldr r1,=0x203FFF0 @random free byte near end of RAM
cmp r0, #0xFF
bne GiveBonus
ldrb r0, [r1]

GiveBonus:
strb r0, [r1] @store normal number of tiles moved at end of RAM
mov r1, #0x2
swi #0x6 @stores quotient in r0
mov r3, r0
mov 	r2, sp
ldrh 	r0, [r2]
add 	r0, r3

@prevent the maximum range from going over 15
cmp 	r0, #0xF
bls NotOverMax
mov 	r0, #0xF
NotOverMax:
strh 	r0, [r2]

End:
ldr 	r0, [sp]
add 	sp, #0x4
pop 	{r3}
bx 	r3
.ltorg
.align
