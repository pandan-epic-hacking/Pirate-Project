@thumb
@org	$080726CA		;FE8U

	ldr	r6, [$08072700+4]  ;FE8U
	ldrb	r0, [r6, #0xE]

	;//�}�b�v�ԍ�����A�}�b�v�ݒ�̃A�h���X��Ԃ��֐� r0:�}�b�v�ݒ�̃A�h���X r0:���ׂ����}�b�vID:MAPCHAPTER
	bl	$08034618          ;FE8U
	ldrb	r1, [r6, #0xF]
	add	r0, #28
	lsl	r2, r1, #25
	bmi	$08072704          ;FE8U
	ldrh	r6, [r0, #2]
	lsl	r1, r1, #24
	bmi	$080726E2          ;FE8U
	ldrh	r6, [r0, #0]
	
	
