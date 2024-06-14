.thumb
.org 0x0
.equ PassID, SkillTester+4
@Bx'd to from 3003D28
@This function sets the Z flag if the moving unit can cross the other unit's tile, either because they're either both allied/npcs or enemies, or because the mover has Pass
push  {r0-r6,r14}   @actually necessary to push the scratch registers in this case
ldrb  r4,[r3,#0xA]  @allegiance byte of current unit
eor   r4,r7     @r7 is allegiance byte of unit on tile we are looking at
mov   r0,#0x80
tst   r0,r4
beq   GoBack      @if non-zero, then one character is an enemy and one is not. If zero, the z flag is set
ldr   r0,GetCharData
mov   r14,r0
ldrb  r0,[r3,#0xA]
.short  0xF800      @returns char data pointer of moving unit

@check for option and ability (copied shamelessly from canto code)
ldr	r5,[r0]		@load character data
cmp	r5,#0x00	@just in case there's no pointer (was doing weird things with generics without this)
beq	JumpLoad1
ldr	r5,[r5,#0x28]	@load character abilities
JumpLoad1:
ldr	r1,[r0,#0x04]	@load class data
cmp	r1,#0x00	@just in case there's no pointer
beq	JumpLoad2
ldr	r1,[r1,#0x28]	@load class abilities
JumpLoad2:
orr	r5,r1
mov	r1,#8		@flight+ bit
lsl	r1, #28
and	r5,r1
cmp	r5,r1

GoBack:
pop   {r0-r6}
pop   {r4}
mov   r14,r4
ldr   r4,GoBackAddress
bx    r4

.align
GetCharData:
.long 0x08019430
GoBackAddress:
.long 0x03003D34    @note that we need to switch back to arm
SkillTester:
@POIN SkillTester
@WORD PassID
