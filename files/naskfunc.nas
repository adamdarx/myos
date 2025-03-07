[FORMAT "WCOFF"]    ; 制作目标文件的格式
[INSTRSET "i486p"]  ; 使用到486为止的指令
[BITS 32]           ; 制作32位模式对应的机械语言

[FILE "naskfunc.nas"]        ; 源文件名
    GLOBAL _io_hlt, _io_cli, _io_sti, _io_stihlt
    GLOBAL _io_in8, _io_in16, _io_in32
    GLOBAL _io_out8, _io_out16, _io_out32
    GLOBAL _io_load_eflags, _io_store_eflags
    GLOBAL _load_gdtr, _load_idtr
    GLOBAL _asm_inthandler21, _asm_inthandler2c

[SECTION .text]    ; 以下是函数本体
_io_hlt:
    HLT
    RET

_io_cli:
    CLI
    RET

_io_sti:
    STI
    RET

_io_stihlt:
    STI
    HLT
    RET
    
_io_in8:
    MOV EDX, [ESP+4]
    MOV EAX, 0
    IN  AL, DX
    RET

_io_in16:
    MOV EDX, [ESP+4]
    MOV EAX, 0
    IN  AX, DX
    RET

_io_in32:
    MOV EDX, [ESP+4]
    MOV EAX, 0
    IN  EAX, DX
    RET

_io_out8:
    MOV EDX, [ESP+4]
    MOV AL, [ESP+8]
    OUT DX, AL
    RET

_io_out16:
    MOV EDX, [ESP+4]
    MOV AX, [ESP+8]
    OUT DX, AX
    RET

_io_out32:
    MOV EDX, [ESP+4]
    MOV EAX, [ESP+8]
    OUT DX, EAX
    RET

_io_load_eflags:
    PUSHFD
    POP EAX
    RET

_io_store_eflags:
    MOV EAX, [ESP+4]
    PUSH EAX
    POPFD
    RET

_load_gdtr:
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

    EXTERN _inthandler21, _inthandler2c

_asm_inthandler21:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX, ESP
    PUSH EAX
    MOV AX, SS
    MOV DS, AX
    MOV ES, AX
    CALL _inthandler21
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD

_asm_inthandler2c:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX, ESP
    PUSH EAX
    MOV AX, SS
    MOV DS, AX
    MOV ES, AX
    CALL _inthandler2c
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD