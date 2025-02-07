[FORMAT "WCOFF"]    ; 制作目标文件的格式
[BITS 32]           ; 制作32位模式对应的机械语言

[FILE "naskfunc.nas"]        ; 源文件名
    GLOBAL _io_hlt         ; 程序中包含的函数名
    GLOBAL _print

[SECTION .text]    ; 以下是函数本体
_io_hlt:
    HLT
    RET