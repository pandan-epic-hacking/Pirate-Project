.thumb

.equ LockTouchID, SkillTester+4

@Check for TouchLock Skill 

push {r2} @The vanilla routine we jump back to needs this value

	ldr 		r2, [r2, #0x4]
	ldr 		r2, [r2, #0x28]			@ gets unit's class attributes
	mov 		r0, #0x1 			@ mounted aid attribute
	and		r2, r0
	cmp		r2, #0x1
	beq		NoPickSkill			@ if the unit has it, they cannot steal
	
	mov r2, r5
	
pop {r2}

	ldr r3, PickSkillBranch
	bx r3
		
	NoPickSkill:
		pop {r2}
		ldr r3, NoPickSkillBranch
			bx r3

.align

	PickSkillBranch:
.long 0x8023E9D

	NoPickSkillBranch:
.long 0x8023E95

.ltorg

	SkillTester:
		@POIN SkillTester
		@WORD LockTouchID
