	.arch armv8-a
	.file	"unloaded_loop.cpp"
	.text
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.align	2
	.global	main
	.type	main, %function
main:
.LFB1986:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	sub	sp, sp, #4016
	.cfi_def_cfa_offset 4032
	str	xzr, [sp, 1024]
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 4008]
	mov	x1, 0
	mov	w0, 1000
	str	w0, [sp, 4]
	str	wzr, [sp]
	b	.L2
.L3:
	ldrsw	x0, [sp]
	lsl	x0, x0, 2
	add	x1, sp, 8
	ldr	w0, [x1, x0]
	add	w2, w0, 1
	ldrsw	x0, [sp]
	lsl	x0, x0, 2
	add	x1, sp, 8
	str	w2, [x1, x0]
	ldr	w0, [sp]
	add	w0, w0, 1
	sxtw	x0, w0
	lsl	x0, x0, 2
	add	x1, sp, 8
	ldr	w0, [x1, x0]
	ldr	w1, [sp]
	add	w1, w1, 1
	add	w2, w0, 1
	sxtw	x0, w1
	lsl	x0, x0, 2
	add	x1, sp, 8
	str	w2, [x1, x0]
	ldr	w0, [sp]
	add	w0, w0, 2
	sxtw	x0, w0
	lsl	x0, x0, 2
	add	x1, sp, 8
	ldr	w0, [x1, x0]
	ldr	w1, [sp]
	add	w1, w1, 2
	add	w2, w0, 1
	sxtw	x0, w1
	lsl	x0, x0, 2
	add	x1, sp, 8
	str	w2, [x1, x0]
	ldr	w0, [sp]
	add	w0, w0, 3
	sxtw	x0, w0
	lsl	x0, x0, 2
	add	x1, sp, 8
	ldr	w0, [x1, x0]
	ldr	w1, [sp]
	add	w1, w1, 3
	add	w2, w0, 1
	sxtw	x0, w1
	lsl	x0, x0, 2
	add	x1, sp, 8
	str	w2, [x1, x0]
	ldr	w0, [sp]
	add	w0, w0, 4
	str	w0, [sp]
.L2:
	ldr	w0, [sp]
	cmp	w0, 999
	ble	.L3
	mov	w0, 0
	mov	w1, w0
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 4008]
	ldr	x2, [x0]
	subs	x3, x3, x2
	mov	x2, 0
	beq	.L5
	bl	__stack_chk_fail
.L5:
	mov	w0, w1
	add	sp, sp, 4016
	.cfi_def_cfa_offset 16
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1986:
	.size	main, .-main
	.section	.rodata
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE, %object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedIjEE:
	.byte	1
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedImEE, %object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedImEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedImEE:
	.byte	1
	.type	_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE, %object
	.size	_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE, 1
_ZNSt8__detail30__integer_to_chars_is_unsignedIyEE:
	.byte	1
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
