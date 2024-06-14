@thumb
@org	$080726CA		;FE8U

	ldr	r6, [$08072700+4]  ;FE8U
	ldrb	r0, [r6, #0xE]

	;//マップ番号から、マップ設定のアドレスを返す関数 r0:マップ設定のアドレス r0:調べたいマップID:MAPCHAPTER
	bl	$08034618          ;FE8U
	ldrb	r1, [r6, #0xF]
	add	r0, #28
	lsl	r2, r1, #25
	bmi	$08072704          ;FE8U
	ldrh	r6, [r0, #2]
	lsl	r1, r1, #24
	bmi	$080726E2          ;FE8U
	ldrh	r6, [r0, #0]
	
	
